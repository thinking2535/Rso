#include "stdafx.h"
#include <iostream>
#include <string>
#include <Rso/Ranking/Client.h>

using namespace std;
using namespace rso;
using namespace ranking;

void Callback(const SRanking& Ranking_)
{
	cout << "Callback" << endl;
	for (auto& i : Ranking_.Users)
	{
		wcout << i.Nick << endl;
	}
}
void LinkS(const CKey& Key_)
{
	cout << "LinkR" << endl;
}
void LinkFailS(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	cout << "LinkFailR" << endl;
}
void UnLinkS(const CKey& Key_, ENetRet NetRet_)
{
	cout << "UnLinkR" << endl;
}

int main(void)
{
	ranking::CClient c(Callback, LinkS, LinkFailS, UnLinkS);
	if (!c.Connect(L"Data/", CNamePort("127.0.0.1", 35020)))
	{
		cout << "Connect Fail" << endl;
		return 0;
	}

	while (!c.IsLinked())
	{
		c.Proc();
		this_thread::sleep_for(milliseconds(1));
	}

	c.Request();

	while (true)
	{
		c.Proc();
		this_thread::sleep_for(milliseconds(1));
	}

	return 0;
}
