#include "stdafx.h"

void LinkU(const CKey& Key_)
{
	DLOG(StringFormat(L"LinkU PeerNum:%d", Key_.PeerNum).c_str());
}
void LinkFailU(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	DLOG(StringFormat(L"LinkFailU PeerNum:%d", PeerNum_).c_str());
}
void UnLinkU(const CKey& Key_, ENetRet NetRet_)
{
	DLOG(StringFormat(L"UnLinkU PeerNum:%d", Key_.PeerNum).c_str());
}
void RecvU(const CKey& Key_, CStream& Stream_)
{
	LOG(StringFormat(L"RecvU PeerNum:%d", Key_.PeerNum).c_str());
}
void RecvUBroadCast(const CKey& Key_, CStream& Stream_)
{
	LOG(StringFormat(L"RecvUBroadCast PeerNum:%d", Key_.PeerNum).c_str());
}


void LinkD(const CKey& Key_)
{
	DLOG(StringFormat(L"LinkD PeerNum:%d", Key_.PeerNum).c_str());
}
void UnLinkD(const CKey& Key_, ENetRet NetRet_)
{
	DLOG(StringFormat(L"UnLinkD PeerNum:%d", Key_.PeerNum).c_str());
}
void RecvD(const CKey& Key_, CStream& Stream_)
{
	LOG(StringFormat(L"RecvD PeerNum:%d", Key_.PeerNum).c_str());
}

void LinkC(const CKey& Key_)
{
	DLOG(StringFormat(L"LinkC PeerNum:%d", Key_.PeerNum).c_str());
}
void UnLinkC(const CKey& Key_, ENetRet NetRet_)
{
	DLOG(StringFormat(L"UnLinkC PeerNum:%d", Key_.PeerNum).c_str());
}
void RecvC(const CKey& Key_, CStream& Stream_)
{
	LOG(StringFormat(L"RecvC PeerNum:%d", Key_.PeerNum).c_str());
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));

		LOG(L"Initializing");

		TPeerCnt ChildCntMax = 5;
		TPeerCnt ErrorCnt = 5;
		g_Net.reset(new TServer(
			EAddressFamily::INET, LinkU, LinkFailU, UnLinkU, RecvU, RecvUBroadCast, LinkD, UnLinkD, RecvD, LinkC, UnLinkC, RecvC,
			CNamePort(), ChildCntMax, ErrorCnt, 1, CNamePort("127.0.0.1", (TPort)30000), CNamePort("127.0.0.1", (TPort)40000), CNamePort("127.0.0.1", (TPort)40000)));

		size_t NetCnt = 100;
		TPort ChildBindPort = 30001;
		TPort ClientBindPort = 40001;
		for (size_t i = 0; i < NetCnt; ++i)
			g_Nets.emplace_back(TNet(new TServer(
				EAddressFamily::INET, LinkU, LinkFailU, UnLinkU, RecvU, RecvUBroadCast, LinkD, UnLinkD, RecvD, LinkC, UnLinkC, RecvC,
				CNamePort("127.0.0.1", (TPort)30000), ChildCntMax, ErrorCnt, 1, CNamePort("127.0.0.1", (TPort)(ChildBindPort + i)), CNamePort("127.0.0.1", (TPort)(ClientBindPort + i)), CNamePort("127.0.0.1", (TPort)(ClientBindPort + i)))));

		LOG(L"Initialized");

		while (true)
		{
			auto* InputKey = g_Input.Get();
			if (InputKey)
			{
				switch (InputKey->Event.KeyEvent.uChar.AsciiChar)
				{
				case 'd':
					g_Net->CloseAll();
					break;

				case 'b':
					g_Net->BroadCast(1);
					break;
				}
			}

			g_Net->Proc();

			for (auto& i : g_Nets)
				i->Proc();

			this_thread::sleep_for(milliseconds(1));
		}
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
	}

	return 0;
}
