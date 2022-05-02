#include "stdafx.h"

unique_ptr<CServer> g_Net;

int g_i = 0;
void LinkNotify(const CKey& Key_)
{
	printf("Link %d\n", Key_.PeerNum);

	g_Net->Certify(Key_.PeerNum);
//	g_Net->Send(Key_, 1);
}
void UnLinkNotify(const CKey& Key_, ENetRet NetRet_)
{
	g_i = 0;
	wprintf(L"UnLink[%d] \n", Key_.PeerNum);
}
void RecvNotify(const CKey& Key_, CStream& Stream_)
{
	int i = 0;
	Stream_ >> i;

	if (i != g_i)
		THROWEX();

	wprintf(L"Recv Key[%d] Value[%d]\n", Key_.PeerNum, i);
	++g_i;

	//g_Net->Send(Key_, i);
}

int _tmain(int argc, _TCHAR* argv[])
{
	CNamePort np("192.168.0.1", 1004);
	cout << CJson(np).toStyledString();

	return 0;

	try
	{
		wcout.imbue(std::locale("korean"));

		LOG(L"Initializing");

		CJson OptionValue(CStream(L"Option.ini"));

		g_Net.reset(new CServer(
			EAddressFamily::INET,
			LinkNotify, UnLinkNotify, RecvNotify,
			true, 10240000, 10240000,
			milliseconds(), milliseconds(), 4, 0,
			CNamePort(OptionValue["BindIP"].asString(), OptionValue["BindPort"].asInt()), SOMAXCONN));

		for (;;)
		{
			g_Net->Proc();
			this_thread::sleep_for(milliseconds(1));
		}
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
