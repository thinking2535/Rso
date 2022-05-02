#include "Client.h"

namespace rso::monitor
{
	void CClient::_LinkS(const CKey& Key_)
	{
		_ServerKey = Key_;
		_LinkFunc(Key_);
		Send(Key_.PeerNum, SCsHeader(ECsProto::Login, list<SKey>()), _LoginInfo);
	}
	void CClient::_RecvS(const CKey& Key_, CStream& Stream_)
	{
		SScHeader Header;
		Stream_ >> Header;
		_RecvFunc(Key_, Header.Proto, Stream_);
	}
	CClient::CClient(
		TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_, const CNamePort& ServerNamePort_) :
		net::CClient(
			std::bind(&CClient::_LinkS, this, _1), LinkFailFunc_, UnLinkFunc_, std::bind(&CClient::_RecvS, this, _1, _2),
			false, 1024000, 1024000,
			milliseconds(20000), milliseconds(9000), 1, 5),
		_ServerNamePort(ServerNamePort_),
		_LinkFunc(LinkFunc_),
		_RecvFunc(RecvFunc_)
	{
	}
	bool CClient::Login(const SCsLogin& LoginInfo_)
	{
		_LoginInfo = LoginInfo_;

		if (!Connect(_ServerNamePort))
			return false;

		return true;
	}
	void CClient::Logout(void)
	{
		CloseAll();
	}

	// To Agent
	void CClient::AgentFileSend(const list<SKey>& AgentKeys_, const SSaFileSend& Proto_)
	{
		Send(_ServerKey, SCsHeader(ECsProto::ToAgent, AgentKeys_), std::move(CStream() << SSaHeader(ESaProto::FileSend) << Proto_));
	}
	void CClient::AgentKeepAlive(const list<SKey>& AgentKeys_, const SSaKeepAlive& Proto_)
	{
		Send(_ServerKey, SCsHeader(ECsProto::ToAgent, AgentKeys_), std::move(CStream() << SSaHeader(ESaProto::KeepAlive) << Proto_));
	}
	void CClient::AgentRunProcess(const list<SKey>& AgentKeys_, const SSaRunProcess& Proto_)
	{
		Send(_ServerKey, SCsHeader(ECsProto::ToAgent, AgentKeys_), std::move(CStream() << SSaHeader(ESaProto::RunProcess) << Proto_));
	}
	void CClient::AgentKillProcess(const list<SKey>& AgentKeys_, const SSaKillProcess& Proto_)
	{
		Send(_ServerKey, SCsHeader(ECsProto::ToAgent, AgentKeys_), std::move(CStream() << SSaHeader(ESaProto::KillProcess) << Proto_));
	}
	void CClient::AgentShellCommand(const list<SKey>& AgentKeys_, const SSaShellCommand& Proto_)
	{
		Send(_ServerKey, SCsHeader(ECsProto::ToAgent, AgentKeys_), std::move(CStream() << SSaHeader(ESaProto::ShellCommand) << Proto_));
	}
	// To Proc
	void CClient::ProcStop(const list<SKey>& AgentKeys_, const SApStop& Proto_)
	{
		Send(_ServerKey, SCsHeader(ECsProto::ToAgent, AgentKeys_), std::move(CStream() << SSaHeader(ESaProto::ToApp) << (CStream() << SApHeader(EApProto::Stop) << Proto_)));
	}
	void CClient::ProcMessage(const list<SKey>& AgentKeys_, const SApMessage& Proto_)
	{
		Send(_ServerKey, SCsHeader(ECsProto::ToAgent, AgentKeys_), std::move(CStream() << SSaHeader(ESaProto::ToApp) << (CStream() << SApHeader(EApProto::Message) << Proto_)));
	}
}
