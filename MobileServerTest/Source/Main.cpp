#include "stdafx.h"

CPeriod<milliseconds> g_Period{ milliseconds(1) };
struct SData
{
	int64 SendNum = 0;
	int64 RecvNum = 0;
};
map<CKey, SData> g_Clients;
unique_ptr<mobile::CServer> g_Net;

void LinkNotify(const CKey& Key_)
{
	g_Net->Certify(Key_.PeerNum);

	g_Clients.emplace(Key_, SData());

	g_Net->Send(Key_.PeerNum, 0);

	printf("Link %d\n", Key_.PeerNum);
}
void UnLinkNotify(const CKey& Key_, ENetRet NetRet_)
{
	g_Clients.erase(Key_);

	wprintf(L"UnLink[%d] \n", Key_.PeerNum);
}
void RecvNotify(const CKey& Key_, CStream& Stream_)
{
	int64 Counter;
	Stream_ >> Counter;

	//auto itClient = g_Clients.find(Key_);
	//if (Counter != itClient->second.RecvNum++)
	//{
	//	wprintf(L"RecvData Error");
	//	throw 0;
	//}

	//if (Counter % 10 == 0)
	//	wprintf(L"Recv Key[%d %d] Counter:%I64d\n", Key_.PeerCounter, Key_.PeerNum, Counter);
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));

		LOG(L"Initializing");

		CJson OptionValue(CStream(L"Option.ini"));

		g_Net.reset(new mobile::CServer(
			EAddressFamily::INET,
			LinkNotify, UnLinkNotify, RecvNotify,
			true, 10240000, 10240000,
			milliseconds(), milliseconds(), 1, 0,
			CNamePort(OptionValue["BindIP"].asString(), OptionValue["BindPort"].asInt()), SOMAXCONN,
			milliseconds(30000)));

		for (;;)
		{
			g_Net->Proc();

			//if (g_Period.CheckAndNextFixed())
			//{
			//	for (auto& i : g_Clients)
			//		g_Net->Send(i.first, i.second.SendNum++);
			//}
			//else
			{
				this_thread::sleep_for(milliseconds(1));
			}
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
