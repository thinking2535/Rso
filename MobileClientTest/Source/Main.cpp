#include "stdafx.h"

CPeriod<milliseconds> g_Period{ milliseconds(1) };
struct SData
{
	int64 SendNum = 0;
	int64 RecvNum = 0;
};
map<TPeerCnt, SData> g_Servers;
unique_ptr<mobile::CClient> g_Net;
CListVoid g_Void;

void LinkNotify(const CKey& Key_)
{
	printf("Link %d\n", Key_.PeerNum);
}
void LinkFailNotify(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	g_Void.erase(PeerNum_);
	g_Servers.erase(PeerNum_);
	wprintf(L"LinkFail PeerNum[%d]\n", PeerNum_);
}
void UnLinkNotify(const CKey& Key_, ENetRet NetRet_)
{
	g_Void.erase(Key_.PeerNum);
	g_Servers.erase(Key_.PeerNum);

	wprintf(L"UnLink [%d] \n", Key_.PeerNum);
}
void RecvNotify(const CKey& Key_, CStream& Stream_)
{
	int32 Counter;
	Stream_ >> Counter;

	auto itClient = g_Servers.find(Key_.PeerNum);
	if (itClient == g_Servers.end())
	{
		wprintf(L"Invalid Client");
		throw 0;
	}

	//if (Counter != itClient->second.RecvNum++)
	//{
	//	wprintf(L"RecvData Error");
	//	throw 0;
	//}

//	if (Counter % 10 == 0)
		wprintf(L"Recv Key[%d %d] Counter:%d\n", Key_.PeerCounter, Key_.PeerNum, Counter);
}
void LinkNotifySoft(const CKey& Key_)
{
	printf("LinkSoft %d\n", Key_.PeerNum);
}
void UnLinkNotifySoft(const CKey& Key_, ENetRet NetRet_)
{
	wprintf(L"UnLinkSoft [%d] \n", Key_.PeerNum);
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));

		LOG(L"Initializing");

		CJson OptionValue(CStream(L"Option.ini"));

		g_Net.reset(new mobile::CClient(
			LinkNotify, LinkFailNotify, UnLinkNotify, RecvNotify,
			true, 1024000, 1024000,
			milliseconds(), milliseconds(), 1, 5,
			LinkNotifySoft, UnLinkNotifySoft, milliseconds(30000)));

		auto Key = g_Net->Connect(CNamePort(OptionValue["ServerIP"].asString(), OptionValue["ServerPort"].asInt()), TPeerCnt(g_Void.new_index()));
		if (!Key)
			THROWEX();
		g_Void.emplace_at(Key.PeerNum);
		g_Servers.emplace(Key.PeerNum, SData());


		int64 Counter = 0;
		for (;;)
		{

			//auto Key1 = g_Net->Connect(CNamePort(OptionValue["ServerIP"].asString(), OptionValue["ServerPort"].asInt()), g_Void.new_index());
			//if (!Key1)
			//	THROWEX();
			//g_Void.emplace_at(Key1.PeerNum);
			//g_Servers.emplace(Key1.PeerNum, SData());

			//auto Key2 = g_Net->Connect(CNamePort(OptionValue["ServerIP"].asString(), OptionValue["ServerPort"].asInt()), g_Void.new_index());
			//if (!Key2)
			//	THROWEX();
			//g_Void.emplace_at(Key2.PeerNum);
			//g_Servers.emplace(Key2.PeerNum, SData());

			g_Net->Proc();

			//if (rand() % 100)
			//	g_Net->CloseAll();

			g_Net->Proc();

			this_thread::sleep_for(milliseconds(1000));
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
