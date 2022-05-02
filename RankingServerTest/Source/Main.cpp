#include "stdafx.h"
#include <iostream>
#include <string>
#include <Rso/Ranking/Master.h>
#include <Rso/GameUtil/OptionJson.h>

using namespace std;
using namespace rso;
using namespace ranking;
using namespace gameutil;

void LinkA(const CKey& Key_)
{
	cout << "LinkA" << endl;
}
void UnLinkA(const CKey& Key_, ENetRet NetRet_)
{
	cout << "UnLinkA" << endl;
}
void LinkU(const CKey& Key_)
{
	cout << "LinkU" << endl;
}
void LinkFailU(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	cout << "LinkFailU" << endl;
}
void UnLinkU(const CKey& Key_, ENetRet NetRet_)
{
	cout << "UnLinkU" << endl;
}
void LinkD(const CKey& Key_)
{
	cout << "LinkD" << endl;
}
void UnLinkD(const CKey& Key_, ENetRet NetRet_)
{
	cout << "UnLinkD" << endl;
}

int main(void)
{
	try
	{
		wcout.imbue(std::locale("korean"));

		COptionJson<SOption> Option(L"Option.ini", true);
		COptionJson<SMasterOption> MasterOption(L"MasterOption.ini", true);
		COptionJson<SDBOption> DBOption(L"DBOption.ini", true);

		ranking::CMaster Server(
			LinkU, LinkFailU, UnLinkU,
			LinkD, UnLinkD,
			*Option,
			LinkA, UnLinkA,
			*MasterOption , *DBOption);

		cout << "Started" << endl;

		while (true)
		{
			Server.Proc();
			this_thread::sleep_for(milliseconds(1));
		}
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
	}

	return 0;
}
