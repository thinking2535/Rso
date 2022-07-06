#pragma once

#include "WSA.h"
#include "Binder.h"
#include <Rso/Base/Thread.h>
#include <Rso/Base/LFQueue.h>
#include <Rso/Base/Period.h>

namespace rso::net
{
	using namespace base;

	// if InsertPeer() called, LinkFunc can be called autometically after UnLinkFunc called 
	class CUDP : public CWSA, public CBinder
	{
		const seconds c_UnLinkTimeOutDuration = seconds(10);
		const seconds c_PingDuration = seconds(7);

	public:
		struct SRecvData
		{
			TIPEndPoint IPEndPoint;
			CStream Stream;
		};

		using TLinkFunc = function<void(TIPEndPoint IPEndPoint_)>;
		using TUnLinkFunc = function<void(TIPEndPoint IPEndPoint_)>;
		using TRecvFunc = function<void(SRecvData& Data_)>;

	private:
		using _TQueue = CLFQueueB<SRecvData>;
		struct _SPeer
		{
			TIPEndPoint IPEndPoint;
			bool IsLinked = false;
			TTime LastReceivedTime = system_clock::now();

			_SPeer(TIPEndPoint IPEndPoint_) :
				IPEndPoint(IPEndPoint_)
			{
			}
		};
		using _TPeers = map<CNamePort, _SPeer>;
		using _TPeersIt = typename _TPeers::iterator;

		void _Receiver(const volatile bool* Exit_);

		CNamePort _BindNamePort; // for recv Addr (v6, v4)
		TLinkFunc _LinkFunc;
		TUnLinkFunc _UnLinkFunc;
		TRecvFunc _RecvFunc;
		CStream _SendStream;
		_TQueue _Queue;
		TSize _BlockSize = 0;
		_TPeers _Peers;
		CPeriod<seconds> _PeriodPingCheck{ seconds(3) };
		CThread _ReceivThread;

		template<typename... TProtos>
		bool _Send(TIPEndPoint IPEndPoint_, const TProtos&... Protos_)
		{
			_SendStream.clear();

			try
			{
				_SendStream.Send(Protos_...);
			}
			catch (...)
			{
				return false;
			}

			try
			{
				if (sendto(Socket(), _SendStream.buff(), (int32)_SendStream.size(), 0, IPEndPoint_->GetSockAddrPtr(), IPEndPoint_->GetAddrLen()) == SOCKET_ERROR)
					return false;
			}
			catch (...)
			{
				return false;
			}

			return true;
		}

	public:
		CUDP(
			EAddressFamily AddressFamily_, const CNamePort& BindNamePort_,
			TLinkFunc LinkFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
			bool NoDelay_, TSize SocketBuffSize_, TSize BlockSize_);
		virtual ~CUDP();
		void InsertPeer(const CNamePort& NamePort_);
		void ErasePeer(const CNamePort& NamePort_);
		void ClearPeer(void);
		void Proc(void);
		template<typename... TProtos>
		bool Send(TIPEndPoint IPEndPoint_, const TProtos&... Protos_)
		{
			return _Send(IPEndPoint_, EPacketType::User, Protos_...);
		}
		template<typename... TProtos>
		bool Send(const CNamePort& NamePort_, const TProtos&... Protos_)
		{
			return _Send(GetIPEndPoint(GetAddressFamily(), NamePort_, false), EPacketType::User, Protos_...);
		}
	};
}