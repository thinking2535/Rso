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
				TPeerCnt ServerCntStandby = 0;	// UdNewChild, DuNewChild ���̿� �ӽ÷� �����ϴ� ��
												// ���� �ڽ��� �Ҵ�Ǳ� ���� �θ��� ���� ������Ű�� ������ ª�� �ð��ȿ� ���޾� �Ҵ��� �� ��� ������ �ڽĿ� ������ �Ҵ� �Ǵ� ���� �����ϱ� �����̰�
												// Capacity.ServerCnt �� ó������ �ʰ� ������ ����(ServerCntStandby)�� �δ� ������
												// ���� �Ҵ� ���� DuCapacity �� �޾Ƽ� ServerCnt �� �ѹ� �Ǿ������ ���� �����ϱ� ����
				_SChild(const SServer& Server_, const SNamePort& ChildBindNamePort_) noexcept :
					SServer(Server_), ChildBindNamePort(ChildBindNamePort_)
				{
				}
				TPeerCnt GetServerCntAll(void) const noexcept
				{
					return Capacity.ServerCnt + ServerCntStandby;
				}
			};

			using _TChilds = map<TPeerCnt, _SChild>; // CList �� �ϸ� Master�������� Child�� ���ڱ� ������ CKey�� PeerNum �� _ChildCntMax�� �Ѿ������ emplace_at�� ���� ������ �� ������ ��. 
													// ���������� DuCapacity�� �����µ� _Childs�� find �ϹǷ� �ӵ��� ���� �߿����� �����Ƿ� �׳� map ����
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
			TPeerCnt _ChildCntMax = 0; // 0+ : �θ� ��ȿ�ϰų� �θ� ���� Master
			TPeerCnt _ErrorCnt = 0;
			CNamePort _ParentNamePort; // �θ�� ������ ���� �� �� ���� ��ȿ�ϸ� �ش� �θ�� ���� �õ���(��, �� �θ�� ����µ� �� ������ ��ȿ�ϸ� ���ο� �θ� �Ҵ�޾Ҵٴ� �ǹ�)
			CNamePort _ClientBindNamePortPub;
			CPeriod<seconds> _PeriodCapacity{ seconds(1) };
			CPeriod<seconds> _PeriodReconnect{ seconds(5) };
			SServer _Parent;
			_TChilds _Childs; // container uses 's'
			_TChildsStandby _ChildsStandby; // �ΰ��� �ڽĸ� ���� ����ϵ��� SessionCode �� ������ �����̳�(�� ������ �Ҵ����� �ʴ��� �缭���� ������븸�� üũ�ϱ� ����)
			_TChildsStandbyTimer _ChildsStandbyTimer; // �ΰ��� �ڽ��� �ð����� �������� ���� ��� _ChildsStandby ���� ����� ����

			TPeerCnt _ReceivedCapacityCnt = 0; // �ڽ����κ��� DuCapacity �� ��� ������ �θ� �����ϱ� ���� (��Ʈ��ũ ���¿� ���� ����Ȯ �� �� ������ ������ �������� �ʰ� ���� ���)

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
			net::CClient _NetU; // Keep Connect �� ������� �ʴ� ������ �θ��� NamePort �� ������ �ƴϱ� ����
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