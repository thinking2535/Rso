#include "stdafx.h"
#include <iostream>
#include <string>
#include <Rso/Ranking/Agent.h>

using namespace std;
using namespace rso;
using namespace ranking;

void LinkR(const CKey& Key_)
{
	cout << "LinkR" << endl;
}
void LinkFailR(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	cout << "LinkFailR" << endl;
}
void UnLinkR(const CKey& Key_, ENetRet NetRet_)
{
	cout << "UnLinkR" << endl;
}

int main(void)
{
	ranking::CAgent a(LinkR, LinkFailR, UnLinkR);
	if (!a.Connect(CNamePort("127.0.0.1", 35030)))
	{
		cout << "Connect Fail" << endl;
		return 0;
	}

	while (!a.IsLinked())
	{
		a.Proc();
		this_thread::sleep_for(milliseconds(1));
	}

	TUsers u;
	u.emplace_back(SUser(SUserCore(L"MyNick", 1234, L"kr"), 900, 1000));

	a.SendUpdate(u);

	while (true)
	{
		a.Proc();
		this_thread::sleep_for(milliseconds(1));
	}

	return 0;
}
