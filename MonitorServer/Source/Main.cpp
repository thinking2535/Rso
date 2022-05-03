#include "stdafx.h"

void CallbackUserProto(const CKey& ClientKey_, CStream& Stream_)
{
}
void LinkAgent(const CKey& Key_)
{
	LOG(L"LinkAgent Key[%d]", Key_.PeerNum);
}
void UnlinkAgent(const CKey& Key_, ENetRet NetRet_)
{
	LOG(L"UnlinkAgent Key[%d]", Key_.PeerNum);
}
void LinkC(const CKey& Key_)
{
	LOG(L"LinkC Key[%d]", Key_.PeerNum);
}
void UnlinkC(const CKey& Key_, ENetRet NetRet_)
{
	LOG(L"UnlinkC Key[%d]", Key_.PeerNum);
}
void TryLogin(const CKey& Key_, CStream& Stream_)
{
	try
	{
		SCsLogin Proto;
		Stream_ >> Proto;

		LOG(L"TryLogin Key[%d] ID[%s] PW[%s]", Key_.PeerNum, Proto.ID, Proto.PW);

		auto it = g_Account.find(Proto.ID);
		if (it == g_Account.end())
			throw(L"Invalid User");

		if (it->second != Proto.PW)
			throw(L"Invalid Password");

		g_Server->Certify(Key_.PeerNum); // 디비 쿼리결과를 받아서 처리한다면 Key 를 파라미터로

		LOG(L"Certified");
	}
	catch (const wchar_t* Msg_)
	{
		LOG(Msg_);
	}
	catch (...)
	{
		LOG(L"");
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));
		COptionJson<SOption> Option(L"Option.ini", false);

		wstring ProcName(MBSToWCS(
			Option->Title) +
			L" MonitorClientBindPort:" + to_wstring(Option->MonitorClientBindPort));
		SetConsoleTitle(ProcName.c_str());

		for (auto& i : Option->Accounts)
			g_Account.emplace(i.ID, i.PW);

		g_Server.reset(new monitor::CServer(
			EAddressFamily::INET,
			LinkAgent, UnlinkAgent,
			LinkC, UnlinkC, TryLogin, CallbackUserProto,
			CNamePort(Option->MonitorClientBindPort), CNamePort(Option->MonitorAgentBindPort)));

		LOG(L"Started");

		while (true)
		{
			g_Server->Proc();
			this_thread::sleep_for(milliseconds(1));
		}
	}
	catch (const exception& Exception_)
	{
		LOG(MBSToWCS(Exception_.what()).c_str());
	}

	return 0;
}
