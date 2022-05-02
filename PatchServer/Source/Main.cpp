#include "stdafx.h"

unique_ptr<patch::CServer> g_Net;

int _tmain(int argc, _TCHAR* argv[])
{
	LOG(L"Initialize Option");
	COptionJson<SOption> Option(L"Option.ini", false);

	wstring ProcName(Option->Title);
	SetConsoleTitle(ProcName.c_str());

	LOG(L"Initialize Network");
	g_Net.reset(new patch::CServer(
		EAddressFamily::INET, CNamePort(Option->MasterNamePort), Option->ChildCntMax, 100, CNamePort(Option->ChildBindNamePort), CNamePort(Option->ClientBindNamePort), CNamePort(Option->ClientBindNamePortPub)));

	LOG(L"Running");
	while (true)
	{
		g_Net->Proc();
		this_thread::sleep_for(milliseconds(1));
	}

	return 0;
}
