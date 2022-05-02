#include "stdafx.h"


#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <Rso/UDP/UDP.h>
#include <Rso/Net/Base.h>


using namespace std;
using namespace rso;
using namespace udp;
using namespace net;


void	RecvNotify(const SOCKADDR_IN& Addr_, CStream& Stream_)
{
	string d;
    Stream_ >> d;
}

int32 counter = 0;
void	RecvNotifyr(const SOCKADDR_IN& Addr_, CStream& Stream_)
{
	string d;
    Stream_ >> d;
	++counter;

//	if (d != "asdfalsdkfjalskdfjlaskdjflaskjdflaksjdflasjdflkajsdflkjasldfkjasldkfjlaskdjflaksjdflkajsdlfjasldkfjalskdjflaskjdflaskjdflkasjdfasdfals")
//	{
//		cout << d;
//		exit(0);
//	}
//	else
//	{
////		wcout << d << endl;
//	}

//	printf("2: Recv Data Ok Data[%d]\n", d);
}

int _tmain(int argc, _TCHAR* argv[])
{
	string BindIP;
	TPort  BindPort;
	string SendIP;
	TPort  SendPort;

	fstream	File;
	File.open("Info.txt", ios::out | ios::in | ios::_Nocreate);

	File >> BindIP >> BindPort >> SendIP >> SendPort;

	File.close();

	try
	{
		CUDP u(EAddressFamily::INET, CNamePort(BindIP, BindPort), RecvNotify, milliseconds(2000), true, 100000, 10000);
		CUDP r(EAddressFamily::INET, CNamePort(SendIP, SendPort), RecvNotifyr, milliseconds(2000), true, 100000, 10000);

		string w;
		w.resize(1, 't');
		w.front() = 'a';
		w.back() = 'b';
		// 무한전송
#if 1
		for (int32 i = 0; i < 2;)
		{
			if (u.Send(CNamePort(SendIP, SendPort), w))
				++i;
			else
				return 0;

			u.Proc();
			r.Proc();
		}

		for (int32 i = 0; i < 5; ++i)
		{
			r.Proc();
			this_thread::sleep_for(milliseconds(100));
		}

		cout << "counter:" << counter << endl;

		return 0;
#else
		// 1회 전송 무한 대기
		int32 d = 3;

		if (!u.Send(Addr, d))
		{
			printf("Send Fail");
			exit(0);
		}

		while (true)
		{
			u.Proc();
			r.Proc();
			this_thread::sleep_for(milliseconds(10));
		}
#endif
	}
	catch (const string& Err_)
	{
		cout << Err_ << endl;
		return 0;
	}

	return 0;
}
