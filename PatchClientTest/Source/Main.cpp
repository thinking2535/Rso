#include "stdafx.h"
#include <Rso/Patch/Client.h>
#include <Rso/Util/Input.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace rso;
using namespace util;
using namespace patch;

CInput g_Input;
unique_ptr<patch::CClient> g_Net;

void Link(const CKey& Key_)
{
	wcout << L"Link" << endl;
}
void LinkFail(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	wcout << L"LinkFail NetRet : " << int32(NetRet_) << endl;
}
void UnLink(const CKey& Key_, ENetRet NetRet_)
{
	wcout << L"UnLink NetRet : " << int32(NetRet_) << endl;
}
void DownloadChanged(const string& ObjectName_, DWORD Received_, DWORD Total_)
{

}
void DownloadCompleted(EHttpRet Ret_, const string& ObjectName_)
{

}
int _tmain(int argc, _TCHAR* argv[])
{
	g_Net.reset(new patch::CClient(
		L"DataFile.bin", "madplanet-patch.s3.ca-central-1.amazonaws.com", true, "data",
		Link, LinkFail, UnLink, DownloadChanged, DownloadCompleted));

	while (true)
	{
		g_Net->Proc();

		auto* InputKey = g_Input.Get();
		if (InputKey)
		{
			switch (InputKey->Event.KeyEvent.uChar.AsciiChar)
			{
			case 'c':
				if (!g_Net->Connect(L"BalanceData", CNamePort("127.0.0.1", 34000)))
				{
					cout << "Connect Fail" << endl;
					return 0;
				}
				break;
			}
		}

		this_thread::sleep_for(milliseconds(1));
	}

	return 0;
}
