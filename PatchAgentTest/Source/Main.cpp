#include "stdafx.h"
#include <Rso/Patch/Agent.h>
#include <Rso/Util/Input.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace rso;
using namespace util;
using namespace patch;

CInput g_Input;
unique_ptr<patch::CAgent> g_Net;

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
void DataCallback(const SPatchData& PatchData_)
{
	cout << "Version [" << PatchData_.Version.Main << ", " << PatchData_.Version.Sub << "]" << endl;

	for (auto& i : PatchData_.Files)
		wcout << L"FileName : " << i.first << L" SubVersion:" << i.second.SubVersion << L" Added:" << i.second.IsAdded << endl;
}
int _tmain(int argc, _TCHAR* argv[])
{
	g_Net.reset(new patch::CAgent(L"DataFile.bin", "data", CNamePort("127.0.0.1", 35000), Link, LinkFail, UnLink, DataCallback));

	while (true)
	{
		g_Net->Proc();

		auto* InputKey = g_Input.Get();
		if (InputKey)
		{
			switch (InputKey->Event.KeyEvent.uChar.AsciiChar)
			{
			case 'c':
				if (!g_Net->Connect(L"rso", L"5528"))
				{
					cout << "Connect Fail" << endl;
					return 0;
				}
				break;

			case 'u':
			{
				TUpdateFiles f;
				f.emplace(L"\\a.txt", true);
				g_Net->Update(false, f);
				break;
			}
			case 'j':
			{
				TUpdateFiles f;
				f.emplace(L"\\a.txt", false);
				g_Net->Update(false, f);
				break;
			}
			case 'i':
			{
				TUpdateFiles f;
				f.emplace(L"\\b.txt", true);
				g_Net->Update(false, f);
				break;
			}
			case 'k':
			{
				TUpdateFiles f;
				f.emplace(L"\\b.txt", false);
				g_Net->Update(false, f);
				break;
			}
			case 'o':
			{
				TUpdateFiles f;
				f.emplace(L"\\c.txt", true);
				g_Net->Update(false, f);
				break;
			}
			case 'l':
			{
				TUpdateFiles f;
				f.emplace(L"\\c.txt", false);
				g_Net->Update(false, f);
				break;
			}

			case 'r':
			{
				TUpdateFiles f;
				f.emplace(L"\\r.txt", true);
				g_Net->Update(true, f);
				break;
			}

			case 'd':
			{
				g_Net->Close();
				break;
			}
			}
		}

		this_thread::sleep_for(milliseconds(1));
	}

	return 0;
}
