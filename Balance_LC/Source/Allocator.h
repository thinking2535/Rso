#pragma once

#include "Base.h"
#include "Protocol.h"
#include <Rso/Base/Base.h>
#include <functional>
#include <Rso/Net/Server.h>
#include <Rso/Base/ListVoid.h>

namespace rso
{
	namespace balance_lc
	{
		using namespace net;

		class CAllocator
		{
			const milliseconds c_EndTimeDuration = milliseconds(1000); // _SServer::EndTimes 만료주기

			// SServer::EndTimes  :  ClientCnt를 Server로 부터 받지 않고 CAllocator 자체적으로 빠르게 임시처리하기위한 컨테이너로, 빠르게 둘 이상의 클라가 접속한 경우 같은 서버로 할당시키지 않기 위함
			using _TServers = CList<SServer>;
			using _TServersIt = typename _TServers::iterator;

			_TServers _Servers;
			CServer _NetS;
			CServer _NetC;
			TLinkFunc _LinkFuncS;
			TUnLinkFunc _UnLinkFuncS;
			TRecvFunc _RecvFuncS;

			void _LinkS(const CKey& Key_);
			void _UnLinkS(const CKey& Key_, ENetRet NetRet_);
			void _RecvS(const CKey& Key_, CStream& Stream_);
			void _RecvSaServerOn(const CKey& Key_, CStream& Stream_);
			void _RecvSaClientCnt(_TServersIt itServer_, CStream& Stream_);

			void _LinkC(const CKey& Key_);
			void _UnLinkC(const CKey& Key_, ENetRet NetRet_S);
			void _RecvC(const CKey& Key_, CStream& Stream_);

		public:
			CAllocator(
				EAddressFamily AddressFamily_,
				TLinkFunc LinkFuncS_, TUnLinkFunc UnLinkFuncS_, TRecvFunc RecvFuncS_,
				const CNamePort& ServerBindNamePort_, size_t NetCThreadCnt_, const CNamePort& ClientBindNamePort_);
			inline void Close(TPeerCnt PeerNum_)
			{
				_NetS.Close(PeerNum_);
			}
			inline void Close(const CKey& Key_)
			{
				_NetS.Close(Key_);
			}
			inline void CloseAll(void)
			{
				_NetS.CloseAll();
			}
			void Proc(void);
			template<typename... _TScProto>
			inline void Send(TPeerCnt PeerNum_, const _TScProto&... Protos_)
			{
				_NetS.Send(PeerNum_, EProto::AsUserProto, Protos_...);
			}
			template<typename... _TScProto>
			inline void Send(const CKey& Key_, const _TScProto&... Protos_)
			{
				_NetS.Send(Key_, EProto::AsUserProto, Protos_...);
			}
			template<typename... _TScProto>
			inline void SendAll(const _TScProto&... Protos_)
			{
				_NetS.SendAll(EProto::AsUserProto, Protos_...);
			}
			template<typename... _TScProto>
			inline void SendAllExcept(const CKey& Key_, const _TScProto&... Protos_)
			{
				_NetS.SendAllExcept(Key_, EProto::AsUserProto, Protos_...);
			}
			inline const CNamePort& GetServerNamePort(TPeerCnt PeerNum_) const
			{
				return _NetS.GetNamePort(PeerNum_);
			}
			inline const CNamePort& GetClientNamePort(TPeerCnt PeerNum_) const
			{
				return _NetC.GetNamePort(PeerNum_);
			}
		};
	}
}
