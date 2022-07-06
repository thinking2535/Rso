#include "UDP.h"

namespace rso
{
	namespace udp
	{
		// TODO : ProtoGetSize 로 검색 하여 적용할것.
		//Proto에 GetSize 함수 만들것
		//	컨테이너나, string 이 들어 있으면 - 1 을 반환 하도록(Member 를 이용해서 구할것인가, StdName 을 이용해서 구할 것인가 ? )
		//	위 적용 후 UDP 헤더, Net, Node 등의 sizeof(SHeader)를 대체할것.RsoCS에서도 c_HeaderSize ? 도 대체할것.

		void CUDP::_Receiver(const volatile bool* Exit_)
		{
			INT	AddrSize = sizeof(SOCKADDR_IN);

			while (*Exit_ == false)
			{
				try
				{
					auto* pRecvData = _Queue.new_buf();

					pRecvData->Stream.clear();
					if (pRecvData->Stream.Capacity() == 0)
					{
						try
						{
							pRecvData->Stream.Reserve(_BlockSize + _HeaderSize);	// use ProtoGetSize
						}
						catch (...)
						{
							this_thread::sleep_for(TSpan(1000));	// Not Enough Memory or etc.
							continue;
						}
					}

					auto Return = recvfrom(Socket(), pRecvData->Stream.data(), (int)pRecvData->Stream.Capacity(), 0, (SOCKADDR*)&pRecvData->Addr, &AddrSize);
					if (Return == SOCKET_ERROR)
					{
						this_thread::sleep_for(TSpan(30));
						continue;
					}

					pRecvData->Stream.SetHead(0);
					pRecvData->Stream.SetTail(Return);

					_Queue.push();
				}
				catch (...)
				{
					this_thread::sleep_for(TSpan(1000));	// Not Enough Memory or etc.
				}
			}
		}
		CUDP::CUDP(EAddressFamily AddressFamily_, const CNamePort& BindInfo_, TRecvNotify RecvNotify_, TSpan TimeOutSpan_, bool NoDelay_, int SocketBuffSize_, _TBlockSize BlockSize_) :
			CBinder(CSocket(AddressFamily_, EIPProto::UDP, NoDelay_, SocketBuffSize_, SocketBuffSize_), AddressFamily_, BindInfo_, false),
			_RecvNotify(RecvNotify_), _TimeOutSpan(TimeOutSpan_)
		{
			if (BlockSize_ > 0)
				_BlockSize = BlockSize_;

			///////////////////////////////////////////
			// ProtoGetSize 로 처리할것.
			CStream Stm;
            Stm << SHeader();
			_HeaderSize = decltype(_HeaderSize)(Stm.size());
			///////////////////////////////////////////

			_ReceivThread.reset(new CThread(std::bind(&CUDP::_Receiver, this, _1)));
		}
		void CUDP::Proc(void)
		{
			auto Now = system_clock::now();

			for (auto* pRecvData = _Queue.get();
				pRecvData;
				pRecvData = _Queue.get())
			{
                try
                {
                    pRecvData->Stream >> _Header;

                    if (_Header.PacketCnt <= 0 ||
                        _Header.PacketNum < 0 ||
                        _Header.PacketNum >= _Header.PacketCnt ||
                        _Header.TotalSize < 0 ||
                        _Header.Index < 0 ||
                        _Header.Index >= _Header.TotalSize ||
                        _Header.Index + pRecvData->Stream.size() > static_cast<size_t>(_Header.TotalSize))
                        throw 0;

                    if (_Header.PacketCnt == 1)
                    {
                        _RecvNotify(pRecvData->Addr, pRecvData->Stream);
                        throw 0;
                    }

                    auto ibProto = _Protos.emplace(SPeerID(pRecvData->Addr.sin_addr.S_un.S_addr, pRecvData->Addr.sin_port, _Header.Time, _Header.Counter), _TProtosPoolIt());
                    if (ibProto.second)
                    {
						try
						{
							ibProto.first->second = _ProtosPool.emplace();

							try
							{
								ibProto.first->second->Checker.resize(_Header.PacketCnt);
								ibProto.first->second->Stream.Reserve(_Header.TotalSize);
							}
							catch (...)
							{
								_Erase(ibProto.first);
								throw;
							}

							ibProto.first->second->Recv(_Header, pRecvData->Stream);
						}
						catch (...)
						{
							_Protos.erase(ibProto.first);
							throw;
						}
                    }
                    else
                    {
                        if (static_cast<size_t>(_Header.PacketCnt) > ibProto.first->second->Checker.size() ||
                            static_cast<size_t>(_Header.TotalSize) > ibProto.first->second->Stream.Capacity())
                        {
                            _Erase(ibProto.first);
                            throw 0;
                        }

                        if (ibProto.first->second->Recv(_Header, pRecvData->Stream))
                        {
                            _RecvDone(pRecvData->Addr, ibProto.first->second->Stream, ibProto.first);
                            throw 0;
                        }
                    }

                    ibProto.first->second->LastRecvTime = Now;
                }
                catch (...)
                {
                }

				pRecvData->Stream.clear();
				_Queue.pop();
			}

			for (auto it = _Protos.begin(); it != _Protos.end(); )
			{
				auto itDel = it;
				++it;

				if ((Now - itDel->second->LastRecvTime) > _TimeOutSpan)
					_Erase(itDel);
			}
		}
	}
}
