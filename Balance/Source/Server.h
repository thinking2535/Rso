#pragma once

#include "Base.h"
#include <Rso/Net/Client.h>
#include <Rso/Net/Server.h>
#include <Rso/Base/ListVoid.h>

namespace rso
{
	using namespace net;

	namespace balance
	{
		class CServer : public INet
		{
			struct _SChild : public SServer
			{
				SNamePort ChildBindNamePort;
				TPeerCnt ServerCntStandby = 0;	// UdNewChild, DuNewChild 사이에 임시로 저장하는 값
												// 실제 자식이 할당되기 전에 부모의 값을 증가시키는 이유는 짧은 시간안에 연달아 할당이 될 경우 동일한 자식에 여러번 할당 되는 것을 방지하기 위함이고
												// Capacity.ServerCnt 로 처리하지 않고 별도의 변수(ServerCntStandby)를 두는 이유는
												// 실제 할당 전에 DuCapacity 를 받아서 ServerCnt 가 롤백 되어버리는 것을 방지하기 위함
				_SChild(const SServer& Server_, const SNamePort& ChildBindNamePort_) noexcept :
					SServer(Server_), ChildBindNamePort(ChildBindNamePort_)
				{
				}
				TPeerCnt GetServerCntAll(void) const noexcept
				{
					return Capacity.ServerCnt + ServerCntStandby;
				}
			};

			using _TChilds = map<TPeerCnt, _SChild>; // CList 로 하면 Master서버에서 Child가 갑자기 몰리면 CKey의 PeerNum 이 _ChildCntMax를 넘어버려서 emplace_at에 의해 공간이 더 잡히게 됨. 
													// 간헐적으로 DuCapacity를 보내는데 _Childs을 find 하므로 속도는 별로 중요하지 않으므로 그냥 map 으로
			using _TChildsIt = _TChilds::iterator;
			using _TChildsStandbyTimer = multimap<TTime, CNamePort>;
			using _TChildsStandbyTimerIt = _TChildsStandbyTimer::iterator;
			struct _SChildStandby
			{
				TSessionCode SessionCode = 0;
				_TChildsStandbyTimerIt ChildsStandbyTimerIt;

				_SChildStandby(TSessionCode SessionCode_, _TChildsStandbyTimerIt ChildsStandbyTimerIt_) noexcept :
					SessionCode(SessionCode_), ChildsStandbyTimerIt(ChildsStandbyTimerIt_)
				{
				}
			};
			using _TChildsStandby = map<CNamePort, _SChildStandby>;
			using _TChildsStandbyIt = _TChildsStandby::iterator;

			CNamePort _MasterNamePort;
			TSessionCode _SessionCounter = 0;
			TSessionCode _SessionCode = 0;
			TPeerCnt _ChildCntMax = 0; // 0+ : 부모가 유효하거나 부모가 없는 Master
			TPeerCnt _ErrorCnt = 0;
			CNamePort _ParentNamePort; // 부모와 접속이 끊긴 후 이 값이 유효하면 해당 부모로 접속 시도함(즉, 현 부모와 끊겼는데 이 변수가 유효하면 새로운 부모를 할당받았다는 의미)
			CNamePort _ClientBindNamePortPub;
			CPeriod<seconds> _PeriodCapacity{ seconds(1) };
			CPeriod<seconds> _PeriodReconnect{ seconds(5) };
			SServer _Parent;
			_TChilds _Childs; // container uses 's'
			_TChildsStandby _ChildsStandby; // 인가된 자식만 접속 허용하도록 SessionCode 를 저장한 컨테이너(당 서버에 할당하지 않더라도 당서버에 접속허용만을 체크하기 위함)
			_TChildsStandbyTimer _ChildsStandbyTimer; // 인가된 자식이 시간내에 접속하지 않을 경우 _ChildsStandby 에서 지우기 위함

			TPeerCnt _ReceivedCapacityCnt = 0; // 자식으로부터 DuCapacity 를 모두 받으면 부모에 전달하기 위함 (네트워크 상태에 따라 부정확 할 수 있으나 오차는 증가하지 않고 빠른 방법)

			TLinkFunc _LinkFuncU;
			TLinkFailFunc _LinkFailFuncU;
			TUnLinkFunc _UnLinkFuncU;
			TRecvFunc _RecvFuncU;
			TRecvFunc _RecvFuncUBroadCast;
			TLinkFunc _LinkFuncD;
			TLinkFailFunc _LinkFailFuncD;
			TUnLinkFunc _UnLinkFuncD;
			TRecvFunc _RecvFuncD;
			TLinkFunc _LinkFuncC;
			TUnLinkFunc _UnLinkFuncC;
			TRecvFunc _RecvFuncC;
			net::CClient _NetU; // Keep Connect 를 사용하지 않는 이유는 부모의 NamePort 가 고정이 아니기 때문
			net::CServer _NetD;
			net::CServer _NetC;

			bool _IsReady(void) const noexcept;
			inline SCapacity _GetCapacity(void) const
			{
				return SCapacity(_NetC.GetPeerCnt(), 1);
			}
			void _LinkU(const CKey& Key_);
			void _LinkFailU(TPeerCnt PeerNum_, ENetRet NetRet_);
			void _UnLinkU(const CKey& Key_, ENetRet NetRet_);
			void _RecvU(const CKey& Key_, CStream& Stream_);
			void _RecvUdParentOn(const CKey& Key_, CStream& Stream_);
			void _RecvUdNewChild(const CKey& Key_, CStream& Stream_);
			void _RecvUdNewParent(const CKey& Key_, CStream& Stream_);
			void _RecvUdCapacity(const CKey& Key_, CStream& Stream_);
			void _RecvUdBroadCast(const CKey& Key_, CStream& Stream_);
			void _LinkD(const CKey& Key_) noexcept;
			void _UnLinkD(const CKey& Key_, ENetRet NetRet_);
			void _RecvD(const CKey& Key_, CStream& Stream_);
			void _RecvDuChildOn(const CKey& Key_, CStream& Stream_);
			void _RecvDuNewChild(_TChildsIt Child_, CStream& Stream_);
			void _RecvDuNewChildFail(_TChildsIt Child_, CStream& Stream_);
			void _RecvDuCapacity(_TChildsIt Child_, CStream& Stream_);
			void _LinkC(const CKey& Key_);
			void _UnLinkC(const CKey& Key_, ENetRet NetRet_);
			void _RecvC(const CKey& Key_, CStream& Stream_);
			void _RecvCsConnect(const CKey& Key_, CStream& Stream_);
			void _SendUdCapacity(const SCapacity& Capacity_);

		public:
			CServer(
				EAddressFamily AddressFamily_,
				TLinkFunc LinkFuncU_, TLinkFailFunc LinkFailFuncU_, TUnLinkFunc UnLinkFuncU_, TRecvFunc RecvFuncU_, TRecvFunc RecvFuncUBroadCast_,
				TLinkFunc LinkFuncD_, TUnLinkFunc UnLinkFuncD_, TRecvFunc RecvFuncD_,
				TLinkFunc LinkFuncC_, TUnLinkFunc UnLinkFuncC_, TRecvFunc RecvFuncC_,
				const CNamePort& MasterNamePort_, TPeerCnt ChildCntMax_, TPeerCnt ErrorCnt_, size_t NetCThreadCnt_, const CNamePort& ChildBindNamePort_, const CNamePort& ClientBindNamePort_, const CNamePort& ClientBindNamePortPub_);
			virtual ~CServer() {}
			inline bool IsLinked(TPeerCnt PeerNum_) const override
			{
				return _NetC.IsLinked(PeerNum_);
			}
			inline void Close(TPeerCnt PeerNum_) override
			{
				_NetC.Close(PeerNum_);
			}
			inline bool Close(const CKey& Key_) override
			{
				return _NetC.Close(Key_);
			}
			inline void CloseAll(void) override
			{
				_NetC.CloseAll();
			}
			inline void WillClose(TPeerCnt PeerNum_, const milliseconds& WaitMilliseconds_) override
			{
				_NetC.WillClose(PeerNum_, WaitMilliseconds_);
			}
			inline bool WillClose(const CKey& Key_, const milliseconds& WaitMilliseconds_) override
			{
				return _NetC.WillClose(Key_, WaitMilliseconds_);
			}
			inline TPeerCnt GetPeerCnt(void) const override
			{
				return _NetC.GetPeerCnt();
			}
			inline milliseconds Latency(TPeerCnt PeerNum_) const override
			{
				return _NetC.Latency(PeerNum_);
			}
			void Proc(void) override;
			inline void CloseU(TPeerCnt PeerNum_)
			{
				_NetU.Close(PeerNum_);
			}
			inline bool CloseU(const CKey& Key_)
			{
				return _NetU.Close(Key_);
			}
			template<typename... TProtos>
			void BroadCast(const TProtos&... Protos_) // to my children recursively
			{
				_NetD.SendAll(SHeader(EProto::UdBroadCast), CStream().Send(Protos_...));
			}
			template<typename... TProtos>
			inline void Send(TPeerCnt PeerNum_, const TProtos&... Protos_)
			{
				_NetC.Send(PeerNum_, SHeader(EProto::ScUserProto), Protos_...);
			}
			template<typename... TProtos>
			inline void Send(const CKey& Key_, const TProtos&... Protos_)
			{
				_NetC.Send(Key_, SHeader(EProto::ScUserProto), Protos_...);
			}
			template<typename... TProtos>
			inline void SendAll(const TProtos&... Protos_)
			{
				_NetC.SendAll(SHeader(EProto::ScUserProto), Protos_...);
			}
			template<typename... TProtos>
			inline void SendD(TPeerCnt PeerNum_, const TProtos&... Protos_)
			{
				_NetD.Send(PeerNum_, SHeader(EProto::UdUserProto), Protos_...);
			}
			template<typename... TProtos>
			inline void SendD(const CKey& Key_, const TProtos&... Protos_)
			{
				_NetD.Send(Key_, SHeader(EProto::UdUserProto), Protos_...);
			}
			template<typename... TProtos>
			inline void SendAllD(const TProtos&... Protos_)
			{
				_NetD.SendAll(SHeader(EProto::UdUserProto), Protos_...);
			}
		};
	}
}