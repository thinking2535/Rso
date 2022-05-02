#include "stdafx.h"


void LinkS(const CKey& Key_, const CIPPort& IPPort_)
{
	DLOG(GetFormat(L"Link PeerNum:%d IP:%s Port:%d", Key_.PeerNum, IPPort_.IPWString(), IPPort_.Port).c_str());
}
void UnLinkS(const CKey& Key_, ENetRet NetRet_, const wstring& Msg_)
{
	DLOG(GetFormat(L"UnLink PeerNum:%d Msg:%s", Key_.PeerNum, Msg_).c_str());
}
void RecvS(const CKey& Key_, CStream& Stream_)
{
	LOG(GetFormat(L"Recv PeerNum:%d", Key_.PeerNum).c_str());
}
int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));

		LOG(L"Initializing");

		CJson OptionValue(CStream(L"Option.ini"));

		wstring ProcName(MBSToWCS(
			OptionValue["Title"].asString() +
			" ClientBindIP:" + OptionValue["ClientBindIP"].asString() +
			" ClientBindPort:" + OptionValue["ClientBindPort"].asString()));
		SetConsoleTitle(ProcName.c_str());

		lc::CAllocator Net(
			seconds(OptionValue["AllocateDelaySec"].asInt()),
			CIPPort(OptionValue["ServerBindIP"].asString(), OptionValue["ServerBindPort"].asInt()),
			OptionValue["NetCThreadCnt"].asInt(),
			CIPPort(OptionValue["ClientBindIP"].asString(), OptionValue["ClientBindPort"].asInt()),
			LinkS, UnLinkS, RecvS);

		while (true)
		{
			Net.Proc();
			this_thread::sleep_for(milliseconds(1));
		}
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
	}

	return 0;
}
