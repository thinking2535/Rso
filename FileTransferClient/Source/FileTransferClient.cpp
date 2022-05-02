// RsoMailTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Rso/FileTransfer/FileTransferClient.h>
#include <Rso/Util/Input.h>
#include <Rso/GameUtil/OptionJson.h>
#include <iostream>
#include <string>


using namespace std;
using namespace rso;
using namespace filetransfer;
using namespace util;
using namespace gameutil;


bool g_GetDone = false;
bool g_PutDone = false;
bool g_Error = false;
bool g_Linked = false;
bool g_LinkFailed = false;
CKey g_SvrKey;
CInput g_Input;

void Link(const CKey& Key_, const CIPPort& IPPort_)
{
	g_Linked = true;
	g_SvrKey = Key_;
	wcout << L"Link: Key[" << Key_.PeerNum << L"] LongIP[" << IPPort_.IP << L"]" << endl;
}
void LinkFail(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	g_LinkFailed = true;
	wcout << L"LinkFail PeerNum" << PeerNum_ << endl;
}
void UnLink(const CKey& Key_, ENetRet NetRet_)
{
	g_Linked = false;
	wcout << L"UnLink: Key[" << Key_.PeerNum << L"]" << endl;
}
void GetDone(const CKey& Key_, const wstring& Name_)
{
	wcout << L"GetDone : Key[" << Key_.PeerNum << L"] Name[" << Name_ << L"]" << endl;
	g_GetDone = true;
}
void PutDone(const CKey& Key_, const wstring& Name_)
{
	wcout << L"PutDone : Key[" << Key_.PeerNum << L"] Name[" << Name_ << L"]" << endl;
	g_PutDone = true;
}
void Error(const CKey& Key_, const wstring& Msg_)
{
	wcout << L"Error : Key[" << Key_.PeerNum << L"] Msg[" << Msg_ << L"]" << endl;
	g_Error = true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	COptionJson<SOption> Option(L"Option.ini", false);

	SetConsoleTitle(Option->Title.c_str());

	CFileTransferClient cli(Link, LinkFail, UnLink, GetDone, PutDone, Error);

    auto Key = cli.Connect(CIPPort("127.0.0.1", 9494));
	if (!Key)
    {
        wcout << L"Connect Fail" << endl;
        return 0;
    }

	//while (!g_Linked)
	//{
	//	if (g_LinkFailed)
	//	{
	//		wcout << L"Link Failed" << endl;
	//		return 0;
	//	}

	//	cli.Proc();

	//	auto* InputKey = g_Input.Get();
	//	if (InputKey)
	//	{
	//		switch (InputKey->Event.KeyEvent.uChar.AsciiChar)
	//		{
	//		case 'c':
	//			if (!cli.Connect())
	//			{
	//				cout << "Connect Fail" << endl;
	//				return 0;
	//			}
	//			break;
	//		}
	//	}

	//	this_thread::sleep_for(milliseconds(10));
	//}

	wstring Cmd(argv[3]);
	MakeLower(Cmd);

	if (Cmd == L"get")
	{
		cli.Get(g_SvrKey, wstring(argv[4]), wstring(argv[5]));
	}
	else if (Cmd == L"put")
	{
		cli.Put(g_SvrKey, wstring(argv[4]), wstring(argv[5]));
	}
	else
	{
		wcout << L"Invalid Command" << endl;
		return 0;
	}

	//while(!(!cli.IsLinked(0) || g_GetDone || g_PutDone || g_Error))
	//{
	//	cli.Proc();
	//	this_thread::sleep_for(milliseconds(1));
	//}

	if (!(g_GetDone || g_PutDone))
		wcout << L"File Transfer Fail" << endl;

	return 0;
}
