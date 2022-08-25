#include "stdafx.h"

void DBCallback(TOutObject& OutObject_)
{
}
void LinkM(const CKey& Key_)
{
	LOG(StringFormat(L"LinkM PeerNum:%d", Key_.PeerNum).c_str());
}
void UnLinkM(const CKey& Key_, ENetRet NetRet_)
{
	LOG(StringFormat(L"UnLinkM PeerNum:%d", Key_.PeerNum).c_str());
}
void RecvM(const CKey& Key_, CStream& Stream_)
{
	LOG(StringFormat(L"RecvM PeerNum:%d", Key_.PeerNum).c_str());
}
void LinkC(const CKey& Key_)
{
	DLOG(StringFormat(L"LinkC PeerNum:%d", Key_.PeerNum).c_str());
}
void UnLinkC(const CKey& Key_, ENetRet NetRet_)
{
	DLOG(StringFormat(L"UnLinkC PeerNum:%d", Key_.PeerNum).c_str());
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));

		LOG(L"Initializing");

		COptionJson<SOption> Option(L"Option.ini", true);
		COptionJson<SDBOption> DBOption(L"DBOption.ini", true);

		wstring ProcName(MBSToWCS(
			Option->Title +
			" ClientBindPort:" + to_string(Option->ClientBindPort)));
		SetConsoleTitle(ProcName.c_str());

		CAuth Auth(
			EAddressFamily::INET,
			[](const SCaCreate& CaCreate_) { return EGameRet::Ok; },
			LinkM, UnLinkM, RecvM, LinkC, UnLinkC, DBCallback,
			*DBOption,
			CNamePort(Option->MasterBindPort),
			Option->NetCThreadCnt,
			CNamePort(Option->ClientBindPort));

		Auth.Start();

		while (true)
		{
			Auth.Proc();
			this_thread::sleep_for(milliseconds(1));
		}
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
	}

	return 0;
}
