#include "stdafx.h"


void Link(const CKey& Key_)
{
	DLOG(StringFormat(L"Link PeerNum:%d", Key_.PeerNum).c_str());
}
void LinkFail(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	DLOG(StringFormat(L"LinkFail PeerNum:%d", PeerNum_).c_str());
}
void UnLink(const CKey& Key_, ENetRet NetRet_)
{
	DLOG(StringFormat(L"UnLink PeerNum:%d", Key_.PeerNum).c_str());
}
void Recv(const CKey& Key_, CStream& Stream_)
{
	LOG(StringFormat(L"Recv PeerNum:%d", Key_.PeerNum).c_str());
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));

		LOG(L"Initializing");

		g_Net.reset(new TClient(Link, LinkFail, UnLink, Recv));

		while (true)
		{
			auto* InputKey = g_Input.Get();
			if (InputKey)
			{
				switch (InputKey->Event.KeyEvent.uChar.AsciiChar)
				{
				case 'c':
					if (!g_Net->Connect(L"Data/", CNamePort("127.0.0.1", 40000)))
						THROWEX();
					break;

				case 'd':
					g_Net->CloseAll();
					break;
				}
			}

			g_Net->Proc();
			this_thread::sleep_for(milliseconds(1));
		}
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
	}

	return 0;
}
