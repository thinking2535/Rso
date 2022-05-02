#include "stdafx.h"

unique_ptr<patch::CMaster> g_Net;
map<wstring, wstring> g_Accounts;

void Link(const CKey& Key_)
{
	wcout << L"Link PeerNum : " << Key_.PeerNum << endl;
}
void UnLink(const CKey& Key_, ENetRet NetRet_)
{
	wcout << L"UnLink PeerNum : " << Key_.PeerNum << L" NetRet : " << (int32)NetRet_ << endl;
}
void Login(const CKey& Key_, const SAmLogin& Proto_)
{
	auto itAccount = g_Accounts.find(Proto_.ID);
	if (itAccount == g_Accounts.end() || itAccount->second != Proto_.PW)
	{
		LOG(L"Invalid ID or PW");
		g_Net->CloseAgent(Key_.PeerNum);
		return;
	}

	g_Net->Certify(Key_.PeerNum, Proto_.Version);
	wcout << L"Login ID : " << Proto_.ID << " PW : " << Proto_.PW << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	LOG(L"Initialize Option");
	COptionJson<SOption> Option(L"Option.ini", true);
	for (auto& i : Option->Accounts)
		g_Accounts.emplace(i.ID, i.PW);

	wstring ProcName(Option->Title);
	SetConsoleTitle(ProcName.c_str());

	LOG(L"Initialize Network");
	g_Net.reset(new patch::CMaster(
		EAddressFamily::INET, Option->DataFile, Option->ChildCntMax, 0, CNamePort(Option->ChildBindNamePort), CNamePort(Option->ClientBindNamePort), CNamePort(Option->ClientBindNamePortPub),
		Link, UnLink, Login, CNamePort(Option->AgentBindNamePort)));

	LOG(L"Running");
	while (true)
	{
		g_Net->Proc();
		this_thread::sleep_for(milliseconds(1));
	}

	return 0;
}
