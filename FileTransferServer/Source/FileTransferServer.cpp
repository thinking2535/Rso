// RsoMailTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Rso/FileTransfer/FileTransferServer.h>
#include <Rso/GameUtil/OptionJson.h>
#include <iostream>
#include <thread>


using namespace std;
using namespace rso;
using namespace filetransfer;
using namespace gameutil;


void Link(const CKey& Key_, const CIPPort& IPPort_)
{
	wcout << L"Link: Key[" << Key_.PeerNum << L"] LongIP[" << IPPort_.IP << L"]" << endl;
}
void UnLink(const CKey& Key_, ENetRet NetRet_, const wstring& Msg_)
{
	wcout << L"UnLink: Key[" << Key_.PeerNum << L"]" << endl;
}
void GetDone(const CKey& Key_, const wstring& Name_)
{
	wcout << L"GetDone : Key[" << Key_.PeerNum << L"] Name[" << Name_ << L"]" << endl;
}
void PutDone(const CKey& Key_, const wstring& Name_)
{
	wcout << L"PutDone : Key[" << Key_.PeerNum << L"] Name[" << Name_ << L"]" << endl;
}
void Error(const CKey& Key_, const wstring& Msg_)
{
	wcout << L"Error : Key[" << Key_.PeerNum << L"] Msg[" << Msg_ << L"]" << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	COptionJson<SOption> Option(L"Option.ini", false);

	SetConsoleTitle(Option->Title.c_str());

	CFileTransferServer svr(Link, UnLink, GetDone, PutDone, Error, Option->IPPort);

	while (true)
	{
		svr.Proc();
		this_thread::sleep_for(milliseconds(1));
	}

	return 0;
}
