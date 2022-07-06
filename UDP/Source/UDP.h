#pragma once

#include <Rso/Net/WSA.h>
#include <Rso/Net/Binder.h>

#pragma comment(lib, "Ws2_32.lib" )
#pragma comment( lib, "Rso/UDP_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include "Protocol.h"
#include <WinSock2.h>
#include <functional>
#include <Rso/Base/List.h>
#include <Rso/Base/Thread.h>
#include <Rso/Base/LFQueue.h>


namespace rso
{
	namespace udp
	{
		using namespace std;
		using namespace std::placeholders;
		using namespace net;

		using TRecvNotify = function<void(const SOCKADDR_IN&, CStream& Stream_)>;

		class CUDP : public CWSA, public CBinder
		{
		private:
			struct _SRecvData
			{
				SOCKADDR_IN Addr;
				CStream Stream;
			};

			struct SPeerID
			{
				TLongIP IP{ 0 };
				TPort	Port;
				int64	Time{ 0 };
				int16	Counter{ 0 };

				SPeerID(TLongIP IP_, TPort Port_, int64 Time_, int16 Counter_) :
					IP(IP_), Port(Port_), Time(Time_), Counter(Counter_) {}

				bool operator < (const SPeerID& PeerID_) const
				{
					if (IP < PeerID_.IP) return true;
					else if (IP > PeerID_.IP) return false;
					else if (Port < PeerID_.Port) return true;
					else if (Port > PeerID_.Port) return false;
					else if (Time < PeerID_.Time) return true;
					else if (Time > PeerID_.Time) return false;
					else if (Counter < PeerID_.Counter) return true;
					return false;
				}
			};

			struct SProtocol
			{
				vector<bool> Checker;
				CStream	Stream;
				int16	ReceivedPacketCnt{ 0 };
				TTime	LastRecvTime;

				void Clear(void)
				{
					Checker.clear();
					Stream.clear();
					ReceivedPacketCnt = 0;
				}
				void init(void)
				{
					Checker.clear();
					Stream.clear();
					ReceivedPacketCnt = 0;
					LastRecvTime = TTime();
				}
				bool Recv(const SHeader& Header_, CStream& Stream_)
				{
					if (Checker[Header_.PacketNum])
						return false;

					auto* Dst = Stream.data();
					auto* Src = Stream_.buff();

					memcpy(&Dst[Header_.Index], Src, Stream_.size());

					++ReceivedPacketCnt;

					if (ReceivedPacketCnt >= Header_.PacketCnt)
					{
						Stream.SetHead(0);
						Stream.SetTail(Header_.TotalSize);
						return true;
					}

					Checker[Header_.PacketNum] = true;

					return false;
				}
			};

			using _TBlockSize = uint16;
			using _TProtosPool = CListB<SProtocol>;
			using _TProtosPoolIt = _TProtosPool::iterator;
			using _TProtos = map<SPeerID, _TProtosPoolIt>;
			using _TProtosIt = _TProtos::iterator;
			using _TQueue = CLFQueueB<_SRecvData>;

			TRecvNotify _RecvNotify;
			_TProtosPool _ProtosPool;
			_TProtos _Protos;
			TTime _LastSentTime;
			TCounter _Counter{ 0 };
			SHeader _Header;
			CStream _SendStreamBlock;
			CStream _SendStream;
			unique_ptr<CThread> _ReceivThread;
			TSpan _WaitSpanForFinal{ 1000 };
			_TQueue _Queue;
			_TBlockSize	_BlockSize{ 900 };
			TSize _HeaderSize{ 0 };	// ProtoGetSize 로 처리할것.
			TSpan _TimeOutSpan;
			void _Receiver(const volatile bool* Exit_);

			void _Erase(_TProtosIt It_)
			{
				It_->second->Clear();
				_ProtosPool.erase(It_->second);
				_Protos.erase(It_);
			}
			void _RecvDone(const SOCKADDR_IN& Addr_, CStream& Stream_, const _TProtosIt& ProtosIt_)
			{
				_RecvNotify(Addr_, Stream_);
				_Erase(ProtosIt_);
			}

		public:
			CUDP(EAddressFamily AddressFamily_, const CNamePort& BindInfo_, TRecvNotify RecvNotify_, TSpan TimeOutSpan_, bool NoDelay_, int SocketBuffSize_, _TBlockSize BlockSize_);
			template<typename TProto>
			bool Send(const CNamePort& NamePort_, const TProto& Proto_)
			{
				_SendStream.clear();

                try
                {
                    _SendStream << Proto_;
                }
                catch(...)
                {
                    return false;
                }

				auto Time = system_clock::now();
				if (Time == _LastSentTime)
				{
					++_Counter;
				}
				else
				{
					_LastSentTime = Time;
					_Counter = 0;
				}

				TPacketCnt PacketCnt = static_cast<TPacketCnt>((_SendStream.size() + _BlockSize - 1) / _BlockSize);
				auto TotalSize = (int32)_SendStream.size();

				auto pIPEndPoint = GetIPEndPoint(GetAddressFamily(), NamePort_, false);

				try
				{
					for (TSize i = 0; _SendStream.size() > 0; ++i)
					{
						_SendStreamBlock.clear();

						_SendStreamBlock << SHeader(Time.time_since_epoch().count(), _Counter, PacketCnt, static_cast<TPacketCnt>(i), TotalSize, static_cast<int32>(i * _BlockSize));
						auto MoveSize = (_SendStream.size() < _BlockSize ? _SendStream.size() : _BlockSize);
						_SendStreamBlock.Push(_SendStream.buff(), MoveSize);
						_SendStream.PopSize(MoveSize);

						if (sendto(Socket(), _SendStreamBlock.buff(), (int)_SendStreamBlock.size(), 0, pIPEndPoint->GetSockAddrPtr(), pIPEndPoint->GetAddrLen()) == SOCKET_ERROR)
							return false;
					}
				}
				catch (...)
				{
					return false;
				}

				return true;
			}
			void Proc(void);
		};
	}
}
