#include "Server.h"

namespace rso::monitor
{
	void CServer::_LinkA(const CKey& Key_)
	{
		_LinkAFunc(Key_);
	}
	void CServer::_UnLinkA(const CKey& Key_, ENetRet NetRet_)
	{
		if (_Agents.erase(Key_) > 0)
			_NetC->SendAll(SScHeader(EScProto::AgentOff), SScAgentOff(Key_));

		_UnLinkAFunc(Key_, NetRet_);
	}
	void CServer::_RecvA(const CKey& Key_, CStream& Stream_)
	{
		try
		{
			SAsHeader Header;
			Stream_ >> Header;

			switch (Header.Proto)
			{
			case EAsProto::AgentOn: return _RecvAsAgentOn(Key_, Stream_);
			case EAsProto::ProcOn: return _RecvAsProcOn(Key_, Stream_);
			case EAsProto::ProcOff: return _RecvAsProcOff(Key_, Stream_);
			case EAsProto::AgentOption: return _RecvAsAgentOption(Key_, Stream_);
			case EAsProto::AgentStat: return _RecvAsAgentStat(Key_, Stream_);
			case EAsProto::ProcStat: return _RecvAsProcStat(Key_, Stream_);
			case EAsProto::NotifyToClient: return _RecvAsNotifyToClient(Key_, Stream_);
			default: THROWEX();
			}
		}
		catch (...)
		{
			_NetA->Close(Key_.PeerNum);
		}
	}
	void CServer::_RecvAsAgentOn(const CKey& Key_, CStream& Stream_)
	{
		SAsAgentOn Proto;
		Stream_ >> Proto;

		auto ib = _Agents.emplace(Key_, _SAgent(Proto));
		if (ib.second == false)
			THROWEX();

		_NetA->Certify(Key_.PeerNum);
		_NetC->SendAll(SScHeader(EScProto::AgentOn), SScAgentOn(Proto, Key_));
	}
	void CServer::_RecvAsProcOn(const CKey& Key_, CStream& Stream_)
	{
		auto itAgent = _Agents.find(Key_);
		if (itAgent == _Agents.end())
			THROWEX();

		SAsProcOn Proto;
		Stream_ >> Proto;

		itAgent->second.Proc = Proto;
		_NetC->SendAll(SScHeader(EScProto::ProcOn), SScProcOn(Key_, Proto));
	}
	void CServer::_RecvAsProcOff(const CKey& Key_, CStream& Stream_)
	{
		auto itAgent = _Agents.find(Key_);
		if (itAgent == _Agents.end())
			THROWEX();

		SAsProcOff Proto;
		Stream_ >> Proto;

		itAgent->second.Proc.clear();
		_NetC->SendAll(SScHeader(EScProto::ProcOff), SScProcOff(Key_));
	}
	void CServer::_RecvAsAgentOption(const CKey& Key_, CStream& Stream_)
	{
		auto itAgent = _Agents.find(Key_);
		if (itAgent == _Agents.end())
			THROWEX();

		SAsAgentOption Proto;
		Stream_ >> Proto;

		itAgent->second.Agent.Option = Proto.Option;
		_NetC->SendAll(SScHeader(EScProto::AgentOption), SScAgentOption(Key_, Proto.Option));
	}
	void CServer::_RecvAsAgentStat(const CKey& Key_, CStream& Stream_)
	{
		auto itAgent = _Agents.find(Key_);
		if (itAgent == _Agents.end())
			THROWEX();

		SAsAgentStat Proto;
		Stream_ >> Proto;

		itAgent->second.Agent.Stat[Proto.Key] = Proto.Data;
		_NetC->SendAll(SScHeader(EScProto::AgentStat), SScAgentStat(Key_, Proto));
	}
	void CServer::_RecvAsProcStat(const CKey& Key_, CStream& Stream_)
	{
		auto itAgent = _Agents.find(Key_);
		if (itAgent == _Agents.end())
			THROWEX();

		SAsProcStat Proto;
		Stream_ >> Proto;

		itAgent->second.Proc.Stat[Proto.Key] = Proto.Data;
		_NetC->SendAll(SScHeader(EScProto::ProcStat), SScProcStat(Key_, Proto));
	}
	void CServer::_RecvAsNotifyToClient(const CKey& /*Key_*/, CStream& Stream_)
	{
		SNotifyToClient Proto;
		Stream_ >> Proto;

		// cmd proc 특정 모니터 클라에게만 보내야 하지만 ShellCommand pipe 결과 처리 작업 필요 
		// _NetC->Send(Key_, SScHeader(EScProto::Notify), SScNotify(Msg_));
		_NetC->SendAll(SScHeader(EScProto::Notify), SScNotify(Proto.Msg));
	}
	void CServer::_RecvC(const CKey& Key_, CStream& Stream_)
	{
		SCsHeader Header;
		Stream_ >> Header;

		switch (Header.Proto)
		{
		case ECsProto::Login: return _RecvCsLogin(Key_, Stream_);
		case ECsProto::UserProto: return _CallbackUserProto(Key_, Stream_);
		case ECsProto::ToAgent: return _RecvCsToAgent(Key_, Header.AgentKeys, Stream_);
		default: THROWEX();
		}
	}
	void CServer::_RecvCsLogin(const CKey& Key_, CStream& Stream_)
	{
		_TryLoginFunc(Key_, Stream_);
	}
	void CServer::_RecvCsToAgent(const CKey& Key_, const list<SKey>& AgentKeys_, CStream& Stream_)
	{
		CStream Stream;
		Stream_ >> Stream;

		for (auto& k : AgentKeys_)
			_NetA->Send(k, Stream, Key_);
	}
	CServer::CServer(
		EAddressFamily AddressFamily_,
		TLinkFunc LinkAFunc_, TUnLinkFunc UnLinkAFunc_,
		TLinkFunc LinkCFunc_, TUnLinkFunc UnLinkCFunc_, TCallbackFunc TryLoginFunc_, TCallbackUserProto CallbackUserProto_,
		const CNamePort& ClientNamePort_, const CNamePort& AgentNamePort_) :
		_ClientNamePort(ClientNamePort_),
		_LinkAFunc(LinkAFunc_),
		_UnLinkAFunc(UnLinkAFunc_),
		_TryLoginFunc(TryLoginFunc_),
		_CallbackUserProto(CallbackUserProto_)
	{
		_NetC.reset(new net::CServer(
			AddressFamily_,
			LinkCFunc_, UnLinkCFunc_, std::bind(&CServer::_RecvC, this, _1, _2),
			false, 10240000, 10240000,
			milliseconds(20000), milliseconds(9000), 1, 0,
			ClientNamePort_, SOMAXCONN));

		_NetA.reset(new net::CServer(
			AddressFamily_,
			std::bind(&CServer::_LinkA, this, _1),
			std::bind(&CServer::_UnLinkA, this, _1, _2),
			std::bind(&CServer::_RecvA, this, _1, _2),
			false, 10240000, 10240000,
			milliseconds(20000), milliseconds(9000), 1, 0,
			AgentNamePort_, SOMAXCONN));
	}
	void CServer::_Certify(TPeerCnt PeerNum_)
	{
		SScAgentInfos Proto;

		for (auto& Agent : _Agents)
			Proto.Infos.emplace_back(SAgentInfo(Agent.first, Agent.second.Agent.GetSAgent(), Agent.second.Proc.GetSProc()));

		_NetC->Send(PeerNum_, SScHeader(EScProto::AgentInfos), Proto);
	}
	void CServer::Certify(TPeerCnt PeerNum_)
	{
		_NetC->Certify(PeerNum_);
		_Certify(PeerNum_);
	}
	void CServer::Certify(const CKey& Key_)
	{
		if (!_NetC->Certify(Key_))
			return;

		_Certify(Key_.PeerNum);
	}
	void CServer::Proc(void)
	{
		_NetC->Proc();
		_NetA->Proc();
	}
}
