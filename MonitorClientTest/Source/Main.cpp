#include "stdafx.h"

#include <Rso/Monitor/Client.h>
#include <Rso/Json/Json.h>
#include <Rso/Log/Log.h>
#include <Rso/Util/Input.h>
#include <iostream>
#include <string>
#include <map>
#include <chrono>

using namespace std;
using namespace rso;
using namespace monitor;
using namespace json;
using namespace log;
using namespace util;
using namespace chrono;

unique_ptr<monitor::CClient> g_Client;
CLog g_Log(Target_Console | Target_File, EPeriod::Day, L"Log/", L"Log", ELocale::Korean);
CInput g_Input;
set<CKey> g_AgentKeys;

#define LOG(...) g_Log.Push(StringFormat(__VA_ARGS__))

void Link(const CKey& Key_)
{
	LOG(L"Link Key[%d]", Key_.PeerNum);
}
void LinkFail(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	LOG(L"LinkFail PeerNum[%d]", PeerNum_);
}
void Unlink(const CKey& Key_, ENetRet NetRet_)
{
	g_AgentKeys.clear();
	LOG(L"Unlink Key[%d]", Key_.PeerNum);
}
void Recv(const CKey& Key_, EScProto Proto_, CStream& Stream_)
{
	if (Proto_ == EScProto::AgentOn)
	{
		SScAgentOn Proto;
		Stream_ >> Proto;

		g_AgentKeys.emplace(Proto.Key);

		LOG(L"Recv ScAgentOn");
		LOG(L" Agent Key[%d]", Proto.Key.PeerNum);

	}
	else if (Proto_ == EScProto::AgentOff)
	{
		SScAgentOff Proto;
		Stream_ >> Proto;

		g_AgentKeys.erase(Proto.Key);

		LOG(L"ScAgentOff Key[%d]", Proto.Key.PeerNum);
	}
	else if (Proto_ == EScProto::ProcOn)
	{
		SScProcOn Proto;
		Stream_ >> Proto;

		LOG(L"ScProcOn AgentKey[%d] ProcName[%s] ProcStat[%s]", Proto.Key.PeerNum, Proto.Proc.Name, Proto.Proc.Stat);
	}
	else if (Proto_ == EScProto::ProcOff)
	{
		SScProcOff Proto;
		Stream_ >> Proto;

		LOG(L"ScProcOff Key[%d]", Proto.Key.PeerNum);
	}
	else if (Proto_ == EScProto::AgentInfos)
	{
		SScAgentInfos Proto;
		Stream_ >> Proto;

		LOG(L"Recv ScAgentOn");

		for (auto& Info : Proto.Infos)
		{
			g_AgentKeys.emplace(Info.Key);
			LOG(L" Agent Key[%d] Stat[%s] ProcName[%s] ProcStat[%s]", Info.Key.PeerNum, Info.Agent.Stat, Info.Proc.Name, Info.Proc.Stat);
		}
	}
	else if (Proto_ == EScProto::AgentOption)
	{
		SScAgentOption Proto;
		Stream_ >> Proto;

		LOG(L"ScProcOption AgentKey[%d]", Proto.AgentKey.PeerNum);
	}
	else if (Proto_ == EScProto::AgentStat)
	{
		SScAgentStat Proto;
		Stream_ >> Proto;

		LOG(L"ScProcStat AgentKey[%d], KeyData[%s %s]", Proto.AgentKey.PeerNum, Proto.KeyData.Key, Proto.KeyData.Data);
	}
	else if (Proto_ == EScProto::ProcStat)
	{
		SScProcStat Proto;
		Stream_ >> Proto;

		LOG(L"ScProcStat AgentKey[%d], KeyData[%s %s]", Proto.AgentKey.PeerNum, Proto.KeyData.Key, Proto.KeyData.Data);
	}
	else if (Proto_ == EScProto::Notify)
	{
		SScNotify Proto;
		Stream_ >> Proto;

		wcout << Proto.Msg << endl;
	}
}
list<SKey> GetAgentKeys(void)
{
	list<SKey> Keys;
	for (auto& i : g_AgentKeys)
		Keys.emplace_back(i);
	return Keys;
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));

		CJson Setup(CStream(L"Setup.ini"));

		g_Client.reset(new monitor::CClient(
			Link, LinkFail, Unlink, Recv,
			CNamePort(Setup["ServerIP"].asString(), Setup["ServerPort"].asInt())));

		LOG(L"Started");

		while (true)
		{
			auto* Char = g_Input.Get();

			if (Char)
			{
				switch (Char->Event.KeyEvent.uChar.AsciiChar)
				{
				case 'l':
				{
					wstring ID;
					wstring PW;
					wcin >> ID >> PW;
					g_Client->Login(SCsLogin(ID, PW));

				}
					break;

				case 'o':
					g_Client->Logout();
					break;

				case 'r':
					g_Client->AgentRunProcess(GetAgentKeys(), SSaRunProcess());
					break;

				case 'k':
					g_Client->AgentKillProcess(GetAgentKeys(), SSaKillProcess());
					break;

				case 's':
					g_Client->ProcStop(GetAgentKeys(), SApStop(L"will stop", 2));
					break;

				case 'm':
					g_Client->ProcMessage(GetAgentKeys(), SApMessage(L"message"));
					break;
				}
			}

			g_Client->Proc();
			this_thread::sleep_for(milliseconds(1));
		}
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
	}

	return 0;
}
