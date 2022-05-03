#pragma once

#include "Base.h"
#include <Rso/Net/ClientKeepConnect.h>
#include <Rso/GameUtil/OptionJson.h>
#include <Rso/Win/Win.h>
#include <Rso/Win/Cmd.h>

namespace rso::monitor
{
	using namespace net;
	using namespace gameutil;
	using namespace win;

	// TODO Proc������ ���� ������¥ ������.
	//- Proc������ ���� ������¥ ������.
	//- ��ġ
	//- ���� ����
	//- ��Ÿ ���
	//- ���� ����

	// * Update Ư�� �ּҷκ��� ���� �ε�
	// * SetDynamicOption C->S �� �����ǵ��� ���� �ɼ��� ����ǵ���.
	// invalid �ϸ� ���ϰ��� �ȵǵ���

	class CAgent
	{
	private:
		using TFileSendFunc = function<void(const wstring& FileName_, CStream& Stream_)>;

		CPeriod<seconds> _KeepalivePeriod{ seconds(10) };
		TCallbackUserProto _CallbackUserProto;
		wstring _AppWorkPath;
		wstring _AppFilePath;
		wstring _DynamicOptionFilePath;
		CNamePort _ServerNamePort;
		CNamePort _ProcNamePort;
		unique_ptr<CClientKeepConnect> _NetS;
		unique_ptr<CClientKeepConnect> _NetP;
		SAgentValue _Agent;
		unique_ptr<SProcValue> _Proc;
		CCmd _ShellCmd;

		void _ShellOutputCallback(const string& Output_);
		void _RunProcess(void);
		void _KillProcess(void);
		void _ShellCommand(const wstring& Command_);
		void _LinkS(const CKey& Key_);
		void _LinkFailS(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLinkS(const CKey& Key_, ENetRet NetRet_);
		void _RecvS(const CKey& Key_, CStream& Stream_);
		void _RecvSaFileSend(const CKey& ClientKey_, CStream& Stream_);
		void _RecvSaKeepAlive(const CKey& ClientKey_, CStream& Stream_);
		void _RecvSaRunProcess(const CKey& ClientKey_, CStream& Stream_);
		void _RecvSaKillProcess(const CKey& ClientKey_, CStream& Stream_);
		void _RecvSaShellCommand(const CKey& ClientKey_, CStream& Stream_);
		void _RecvSaToApp(const CKey& ClientKey_, CStream& Stream_);
		void _LinkP(const CKey& Key_);
		void _LinkFailP(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLinkP(const CKey& Key_, ENetRet NetRet_);
		void _RecvP(const CKey& Key_, CStream& Stream_);
		void _RecvPaProcOn(const CKey& Key_, CStream& Stream_);
		void _RecvPaStat(const CKey& Key_, CStream& Stream_);
		void _RecvPaNotifyToClient(const CKey& Key_, CStream& Stream_);

	public:
		CAgent(
			TCallbackUserProto CallbackUserProto_,
			const wstring& Name_,
			const wstring& AppWorkPath_,
			const wstring& AppFileName_,
			const wstring& DynamicOptionFilePath_,
			const CNamePort& ServerNamePort_,
			const CNamePort& ProcNamePort_,
			const wstring& Stat_);
		void SetStat(const SKeyData& KeyData_);
		void SendNotifyToClient(const CKey& Key_, const wstring& Msg_);
		void Proc(void);
	};
}
