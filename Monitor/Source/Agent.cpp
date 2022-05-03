#include "Agent.h"

namespace rso::monitor
{
	void CAgent::_ShellOutputCallback(const string& Output_)
	{
		// cmd proc 
		SendNotifyToClient(CKey(), MBSToWCS(Output_, ELocale::Korean));
	}
	void CAgent::_RunProcess(void)
	{
		if (GetPIDWithProcessName(_AppFilePath) == 0)
		{
			if ((int64)ShellExecute(NULL, L"open", _AppFilePath.c_str(), NULL, _AppWorkPath.c_str(), SW_HIDE) <= 32)
			{
				// Error
			}
		}
	}
	void CAgent::_KillProcess(void)
	{
		auto ProcessID = GetPIDWithProcessName(_AppFilePath);
		if (ProcessID == 0)
			return;

		TerminateProcessWithPID(ProcessID, 0);
	}
	void CAgent::_ShellCommand(const wstring& Command_)
	{
		try
		{
			_ShellCmd.Input(CCmd::SInObject{ Command_, _AppWorkPath });
		}
		catch (...)
		{
		}
	}
	void CAgent::_LinkS(const CKey& Key_)
	{
		_NetS->Send(Key_.PeerNum, SAsHeader(EAsProto::AgentOn), SAsAgentOn(_Agent.GetSAgent()));
		if (_Proc)
			_NetS->SendAll(SAsHeader(EAsProto::ProcOn), SAsProcOn(_Proc->GetSProc()));
	}
	void CAgent::_LinkFailS(TPeerCnt /*PeerNum_*/, ENetRet /*NetRet_*/)
	{
	}
	void CAgent::_UnLinkS(const CKey& /*Key_*/, ENetRet /*NetRet_*/)
	{
	}
	void CAgent::_RecvS(const CKey& /*Key_*/, CStream& Stream_)
	{
		CStream Stream;
		Stream_ >> Stream;

		SKey ClientKey;
		Stream_ >> ClientKey;

		SSaHeader Header;
		Stream >> Header;

		switch (Header.Proto)
		{
		case ESaProto::FileSend: return _RecvSaFileSend(ClientKey, Stream);
		case ESaProto::KeepAlive: return _RecvSaKeepAlive(ClientKey, Stream);
		case ESaProto::RunProcess: return _RecvSaRunProcess(ClientKey, Stream);
		case ESaProto::KillProcess: return _RecvSaKillProcess(ClientKey, Stream);
		case ESaProto::ShellCommand: return _RecvSaShellCommand(ClientKey, Stream);
		case ESaProto::UserProto: return _CallbackUserProto(ClientKey, Stream);
		case ESaProto::ToApp: return _RecvSaToApp(ClientKey, Stream);
		default: THROWEX();
		}
	}
	void CAgent::_RecvSaFileSend(const CKey& /*ClientKey_*/, CStream& Stream_)
	{
		SSaFileSend Proto;
		Stream_ >> Proto;

		for (auto& File : Proto.Files)
		{
			create_directories(_AppWorkPath + path(File.PathName).parent_path().wstring());
			File.Stream.SaveFile(_AppWorkPath + File.PathName);
		}
	}
	void CAgent::_RecvSaKeepAlive(const CKey& /*ClientKey_*/, CStream& Stream_)
	{
		SSaKeepAlive Proto;
		Stream_ >> Proto;

		_Agent.Option.KeepAlive = Proto.On;
		COptionJson<SAgentOption>(_DynamicOptionFilePath, _Agent.Option);

		_NetS->SendAll(SAsHeader(EAsProto::AgentOption), SAsAgentOption(_Agent.Option));
	}
	void CAgent::_RecvSaRunProcess(const CKey& /*ClientKey_*/, CStream& /*Stream_*/)
	{
		//SSaRunProcess Proto;
		//Stream_ >> Proto;

		_RunProcess();
	}
	void CAgent::_RecvSaKillProcess(const CKey& /*ClientKey_*/, CStream& /*Stream_*/)
	{
		//SSaKillProcess Proto;
		//Stream_ >> Proto;

		_KillProcess();
	}
	void CAgent::_RecvSaShellCommand(const CKey& /*ClientKey_*/, CStream& Stream_)
	{
		SSaShellCommand Proto;
		Stream_ >> Proto;

		_ShellCommand(Proto.Command);
	}
	void CAgent::_RecvSaToApp(const CKey& ClientKey_, CStream& Stream_)
	{
		CStream Stream;
		Stream_ >> Stream;

		_NetP->SendAll(Stream, ClientKey_);
	}
	void CAgent::_LinkP(const CKey& /*Key_*/)
	{
	}
	void CAgent::_LinkFailP(TPeerCnt /*PeerNum_*/, ENetRet /*NetRet_*/)
	{
	}
	void CAgent::_UnLinkP(const CKey& /*Key_*/, ENetRet /*NetRet_*/)
	{
		if (_Proc)
		{
			_Proc.reset();
			_NetS->SendAll(SAsHeader(EAsProto::ProcOff), SAsProcOff());
		}
	}
	void CAgent::_RecvP(const CKey& Key_, CStream& Stream_)
	{
		SPaHeader Header;
		Stream_ >> Header;

		switch (Header.Proto)
		{
		case EPaProto::ProcOn: return _RecvPaProcOn(Key_, Stream_);
		case EPaProto::Stat: return _RecvPaStat(Key_, Stream_);
		case EPaProto::NotifyToClient: return _RecvPaNotifyToClient(Key_, Stream_);
		default: THROWEX();
		}
	}
	void CAgent::_RecvPaProcOn(const CKey& /*Key_*/, CStream& Stream_)
	{
		SPaProcOn Proto;
		Stream_ >> Proto;

		_Proc.reset(new SProcValue(Proto));
		_NetS->SendAll(SAsHeader(EAsProto::ProcOn), SAsProcOn(Proto));
	}
	void CAgent::_RecvPaStat(const CKey& /*Key_*/, CStream& Stream_)
	{
		SPaStat Proto;
		Stream_ >> Proto;

		_Proc->Stat[Proto.Key] = Proto.Data;

		_NetS->SendAll(SAsHeader(EAsProto::ProcStat), SAsProcStat(Proto));
	}
	void CAgent::_RecvPaNotifyToClient(const CKey& /*Key_*/, CStream& Stream_)
	{
		SNotifyToClient Proto;
		Stream_ >> Proto;
		_NetS->SendAll(SAsHeader(EAsProto::NotifyToClient), Proto);
	}
	CAgent::CAgent(
		TCallbackUserProto CallbackUserProto_,
		const wstring& Name_,
		const wstring& AppWorkPath_,
		const wstring& AppFileName_,
		const wstring& DynamicOptionFilePath_,
		const CNamePort& ServerNamePort_,
		const CNamePort& ProcNamePort_,
		const wstring& Stat_) :
		_CallbackUserProto(CallbackUserProto_),
		_AppWorkPath(AppWorkPath_),
		_AppFilePath(AppWorkPath_ + AppFileName_),
		_DynamicOptionFilePath(DynamicOptionFilePath_),
		_ServerNamePort(ServerNamePort_),
		_ProcNamePort(ProcNamePort_),
		_ShellCmd(std::bind(&CAgent::_ShellOutputCallback, this, _1)),
		_Agent(SAgent(Name_, *COptionJson<SAgentOption>(DynamicOptionFilePath_, false), Stat_))
	{
		_NetS.reset(new CClientKeepConnect(
			std::bind(&CAgent::_LinkS, this, _1),
			std::bind(&CAgent::_LinkFailS, this, _1, _2),
			std::bind(&CAgent::_UnLinkS, this, _1, _2),
			std::bind(&CAgent::_RecvS, this, _1, _2),
			false, 1024000, 1024000,
			milliseconds(20000), milliseconds(9000), 1, 5,
			milliseconds(3000)));

		if (_ServerNamePort)
			_NetS->Connect(_ServerNamePort);

		_NetP.reset(new CClientKeepConnect(
			std::bind(&CAgent::_LinkP, this, _1),
			std::bind(&CAgent::_LinkFailP, this, _1, _2),
			std::bind(&CAgent::_UnLinkP, this, _1, _2),
			std::bind(&CAgent::_RecvP, this, _1, _2),
			false, 1024000, 1024000,
			milliseconds(20000), milliseconds(9000), 1, 5,
			milliseconds(3000)));

		if (_ProcNamePort)
			_NetP->Connect(_ProcNamePort);
	}
	void CAgent::SetStat(const SKeyData& KeyData_)
	{
		_Agent.Stat[KeyData_.Key] = KeyData_.Data;

		_NetS->SendAll(SAsHeader(EAsProto::AgentStat), SAsAgentStat(KeyData_));
	}
	void CAgent::SendNotifyToClient(const CKey& Key_, const wstring& Msg_)
	{
		_NetS->SendAll(SAsHeader(EAsProto::NotifyToClient), SNotifyToClient(Key_, Msg_));
	}
	void CAgent::Proc(void)
	{
		_NetS->Proc();
		_NetP->Proc();
		_ShellCmd.Proc();

		if (_KeepalivePeriod.CheckAndNextLoose() && !_NetP->IsConnecting(0))
		{
			if (_Agent.Option.KeepAlive)
				_RunProcess();
		}
	}
}
