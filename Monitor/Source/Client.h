#pragma once

#include "Base.h"
#include <Rso/Net/Client.h>

namespace rso::monitor
{
	using namespace base;
	using namespace net;

	class CClient : public net::CClient
	{
	public:
		using TCallbackFunc = function<void(CStream&)>;
		using TRecvFunc = function<void(const CKey&, EScProto, CStream&)>;

	private:
		CNamePort _ServerNamePort;
		CKey _ServerKey;
		TLinkFunc _LinkFunc;
		SCsLogin _LoginInfo;
		TRecvFunc _RecvFunc;

		void _LinkS(const CKey& Key_);
		void _RecvS(const CKey& Key_, CStream& Stream_);

	public:
		CClient(
			TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_, const CNamePort& ServerNamePort_);
		bool Login(const SCsLogin& LoginInfo_);
		void Logout(void);
		// To Server
		template<typename... TProtos>
		void ServerUserProto(const TProtos&... Protos_)
		{
			Send(_ServerKey, SCsHeader(ECsProto::UserProto, list<SKey>()), std::forward<TProtos>(Protos_)...);
		}
		// To Agent
		void AgentFileSend(const list<SKey>& AgentKeys_, const SSaFileSend& Proto_);
		void AgentKeepAlive(const list<SKey>& AgentKeys_, const SSaKeepAlive& Proto_);
		void AgentRunProcess(const list<SKey>& AgentKeys_, const SSaRunProcess& Proto_);
		void AgentKillProcess(const list<SKey>& AgentKeys_, const SSaKillProcess& Proto_);
		void AgentShellCommand(const list<SKey>& AgentKeys_, const SSaShellCommand& Proto_);
		template<typename... TProtos>
		void AgentUserProto(const list<SKey>& AgentKeys_, const TProtos&... Protos_)
		{
			Send(_ServerKey, SCsHeader(ECsProto::ToAgent, AgentKeys_), std::move(CStream() << SSaHeader(ESaProto::UserProto)).Send(std::forward<TProtos>(Protos_)...));
		}
		// To Proc
		void ProcStop(const list<SKey>& AgentKeys_, const SApStop& Proto_);
		void ProcMessage(const list<SKey>& AgentKeys_, const SApMessage& Proto_);
		template<typename... TProtos>
		void ProcUserProto(const list<SKey>& AgentKeys_, const TProtos&... Protos_)
		{
			Send(_ServerKey, SCsHeader(ECsProto::ToAgent, AgentKeys_), std::move(CStream() << SSaHeader(ESaProto::ToApp) << (CStream() << SApHeader(EApProto::UserProto)).Send(std::forward<TProtos>(Protos_)...)));
		}
	};
}
