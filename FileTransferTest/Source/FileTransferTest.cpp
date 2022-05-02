// RsoMailTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Rso/FileTransfer/FileTransferServer.h>
#include <Rso/FileTransfer/FileTransferClient.h>
#include <iostream>
#include <thread>


using namespace std;
using namespace rso;
using namespace filetransfer;


bool g_svrDone = false;
bool g_cliDone = false;
bool g_Accepted = false;
CKey g_CliKey;

void Link(const CKey& Key_, const CIPPort& IPPort_)
{
	g_CliKey = Key_;
	g_Accepted = true;
	wcout << L"Link: Key[" << Key_.PeerNum << L"] LongIP[" << IPPort_.IP << L"]" << endl;
}
void UnLink(const CKey& Key_, ENetRet NetRet_)
{
	wcout << L"UnLink: Key[" << Key_.PeerNum << L"]" << endl;
	g_Accepted = false;
}
void GetDone(const CKey& Key_, const wstring& Name_)
{
	wcout << L"GetDone : Key[" << Key_.PeerNum << L"] Name[" << Name_ << L"]" << endl;

	g_svrDone = true;
}
void PutDone(const CKey& Key_, const wstring& Name_)
{
	wcout << L"PutDone : Key[" << Key_.PeerNum << L"] Name[" << Name_ << L"]" << endl;

	g_svrDone = true;
}
void Error(const CKey& Key_, const wstring& Msg_)
{
	wcout << L"Error : Key[" << Key_.PeerNum << L"] Msg[" << Msg_ << L"]" << endl;
}

void CLink(const CKey& Key_, const CIPPort& IPPort_)
{
	wcout << L"CLink: Key[" << Key_.PeerNum << L"] LongIP[" << IPPort_.IP << L"]" << endl;
}
void CLinkFail(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	wcout << L"CLinkFail: Key[" << PeerNum_ << L"]" << endl;
}
void CUnLink(const CKey& Key_, ENetRet NetRet_)
{
	wcout << L"CUnLink: Key[" << Key_.PeerNum << L"]" << endl;
}
void CGetDone(const CKey& Key_, const wstring& Name_)
{
	wcout << L"CGetDone : Key[" << Key_.PeerNum << L"] Name[" << Name_ << L"]" << endl;

	g_cliDone = true;
}
void CPutDone(const CKey& Key_, const wstring& Name_)
{
	wcout << L"CPutDone : Key[" << Key_.PeerNum << L"] Name[" << Name_ << L"]" << endl;

	g_cliDone = true;
}
void CError(const CKey& Key_, const wstring& Msg_)
{
	wcout << L"CError : Key[" << Key_.PeerNum << L"] Msg[" << Msg_ << L"]" << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CFileTransferServer svr(Link, UnLink, GetDone, PutDone, Error, CIPPort("211.52.116.86", 33223));
	auto* cli = new CFileTransferClient(CLink, CLinkFail, CUnLink, CGetDone, CPutDone, CError);

	cli->Connect(CIPPort("211.52.116.86", 33223));

	//while (!cli->IsLinked() || !g_Accepted)
	//{
	//	svr.Proc();
	//	cli->Proc();

	//	this_thread::sleep_for(milliseconds(10));
	//}

	cli->Put(CKey(), L"src.ini", L"cli_put.ini");
	cli->Get(CKey(), L"src.ini", L"cli_get.ini");
	svr.Put(g_CliKey, L"src.ini", L"svr_put.ini");
	svr.Get(g_CliKey, L"src.ini", L"svr_get.ini");

	cout << "Processing" << endl;

	for (int i = 0; i < 1000; ++i)
	{
		svr.Proc();
		cli->Proc();
		this_thread::sleep_for(milliseconds(10));
	}

	return 0;
}
