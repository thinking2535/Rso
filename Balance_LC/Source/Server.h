#pragma once

#include "Base.h"
#include "Protocol.h"
#include <Rso/Base/Base.h>
#include <functional>
#include <Rso/Net/Server.h>
#include <Rso/Net/ClientKeepConnect.h>

namespace rso
{
	namespace balance_lc
	{
		using namespace net;

		class CServer
		{
			CPeriod<milliseconds> _SyncDelay{ milliseconds(1000) };
			SNamePort _ClientBindNamePortPub;
			int32 _ClientCnt = 0;
			CClientKeepConnect _NetA;
			net::CServer _NetC;
			TLinkFunc _LinkFuncA;
			TLinkFunc _LinkFuncC;
			TUnLinkFunc _UnLinkFuncC;
			TRecvFunc _RecvFuncC;

			void _LinkA(const CKey& Key_);
			void _LinkC(const CKey& Key_);
			void _UnLinkC(const CKey& Key_, ENetRet NetRet_);
			void _RecvC(const CKey& Key_, CStream& Stream_);

		public:
			CServer(
				EAddressFamily AddressFamily_,
				TLinkFunc LinkFuncA_, TLinkFailFunc LinkFailFuncA_, TUnLinkFunc UnLinkFuncA_, TRecvFunc RecvFuncA_,
				TLinkFunc LinkFuncC_, TUnLinkFunc UnLinkFuncC_, TRecvFunc RecvFuncC_,
				const CNamePort& AllocatorNamePort_, size_t NetCThreadCnt_, const CNamePort& ClientBindNamePort_, const CNamePort& ClientBindNamePortPub_);
			template<typename... _TScProto>
			inline void SendA(TPeerCnt PeerNum_, const _TScProto&... Protos_)
			{
				_NetA.Send(PeerNum_, EProto::SaUserProto, Protos_...);
			}
			template<typename... _TScProto>
			inline void SendA(const CKey& Key_, const _TScProto&... Protos_)
			{
				_NetA.Send(Key_, EProto::SaUserProto, Protos_...);
			}
			template<typename... _TScProto>
			inline void SendAllA(const _TScProto&... Protos_)
			{
				_NetA.SendAll(EProto::SaUserProto, Protos_...);
			}
			template<typename... _TScProto>
			inline void SendAllExceptA(const CKey& Key_, const _TScProto&... Protos_)
			{
				_NetA.SendAllExcept(Key_, EProto::SaUserProto, Protos_...);
			}
			inline void CloseA(TPeerCnt PeerNum_)
			{
				_NetA.Close(PeerNum_);
			}
			inline void CloseA(const CKey& Key_)
			{
				_NetA.Close(Key_);
			}
			inline void CloseAllA(void)
			{
				_NetA.CloseAll();
			}
			template<typename... _TScProto>
			inline void Send(TPeerCnt PeerNum_, const _TScProto&... Protos_)
			{
				_NetC.Send(PeerNum_, EProto::ScUserProto, Protos_...);
			}
			template<typename... _TScProto>
			inline void Send(const CKey& Key_, const _TScProto&... Protos_)
			{
				_NetC.Send(Key_, EProto::ScUserProto, Protos_...);
			}
			template<typename... _TScProto>
			inline void SendAll(const _TScProto&... Protos_)
			{
				_NetC.SendAll(EProto::ScUserProto, Protos_...);
			}
			template<typename... _TScProto>
			inline void SendAllExcept(const CKey& Key_, const _TScProto&... Protos_)
			{
				_NetC.SendAllExcept(Key_, EProto::ScUserProto, Protos_...);
			}
			inline void Close(TPeerCnt PeerNum_)
			{
				_NetC.Close(PeerNum_);
			}
			inline void Close(const CKey& Key_)
			{
				_NetC.Close(Key_);
			}
			inline void CloseAll(void)
			{
				_NetC.CloseAll();
			}
			inline void WillClose(TPeerCnt PeerNum_, const milliseconds& WaitMilliseconds_)
			{
				_NetC.WillClose(PeerNum_, WaitMilliseconds_);
			}
			inline void WillClose(const CKey& Key_, const milliseconds& WaitMilliseconds_)
			{
				_NetC.WillClose(Key_, WaitMilliseconds_);
			}
			void Proc(void);
			inline milliseconds Latency(TPeerCnt PeerNum_) const
			{
				return _NetC.Latency(PeerNum_);
			}
			inline void Certify(TPeerCnt PeerNum_)
			{
				_NetC.Certify(PeerNum_);
			}
			inline bool Certify(const CKey& Key_)
			{
				return _NetC.Certify(Key_);
			}
			inline const CNamePort& GetAllocatorNamePort(TPeerCnt PeerNum_) const
			{
				return _NetA.GetNamePort(PeerNum_);
			}
			inline const CNamePort& GetClientNamePort(TPeerCnt PeerNum_) const
			{
				return _NetC.GetNamePort(PeerNum_);
			}
		};
	}
}
