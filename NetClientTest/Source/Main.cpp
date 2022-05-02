#include "stdafx.h"

CKey g_ServerKey;
unique_ptr<CClient> g_Net;

void LinkNotify(const CKey& Key_)
{
	g_ServerKey = Key_;
	printf("Link %d\n", Key_.PeerNum);
}
void LinkFailNotify(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	wprintf(L"LinkFail PeerNum[%d]\n", PeerNum_);
}
void UnLinkNotify(const CKey& Key_, ENetRet NetRet_)
{
	g_ServerKey = CKey();
	wprintf(L"UnLink[%d] \n", Key_.PeerNum);
}
void RecvNotify(const CKey& Key_, CStream& Stream_)
{
	wprintf(L"Recv Key[%d]\n", Key_.PeerNum);
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));

		LOG(L"Initializing");

		COptionJson<SOption> Option(L"Option.ini", false);

		g_Net.reset(new CClient(
			LinkNotify, LinkFailNotify, UnLinkNotify, RecvNotify,
			true, 1024000, 1024000,
			milliseconds(), milliseconds(), 1, 5));

		//auto Key = g_Net->Connect(CNamePort(Option->IP, Option->Port));
		//if (!Key)
		//	THROWEX();

		//for (;;)
		//{
		//	g_Net->Proc();
		//	this_thread::sleep_for(milliseconds(1));

		//	//if (g_ServerKey)
		//	//	c.Send(g_ServerKey, 1);
		//}
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
	}
	catch (...)
	{
		cout << "Exception" << endl;
	}

	return 0;
}
