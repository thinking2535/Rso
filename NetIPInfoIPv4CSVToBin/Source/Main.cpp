#include "stdafx.h"

#include <iostream>
#include <chrono>
#include <Rso/GameUtil/OptionJson.h>
#include <Rso/Net/IPInfo.h>

using namespace std;
using namespace chrono;
using namespace rso;
using namespace net;
using namespace gameutil;

int _tmain(int argc, _TCHAR* argv[])
{
// export	CIPInfo::CSVToBin_IP2LocationLite(L"IP2LOCATION-LITE-DB11.CSV", L"ipv4.bin");

	CIPInfo a(L"ipv4.bin");

	auto ip = _byteswap_ulong(2150439168); // Italy Lazio

	auto Now = system_clock::now();

	auto it = a.Get(ip);
	cout << it.CountryCode << endl;
	cout << it.GetLocalDateTime(Now).ToString() << endl;

	it = a.Get("121.165.94.77");
	cout << it.CountryCode << endl;
	cout << it.GetLocalDateTime(Now).ToString() << endl;

	//cout << SDateTime(system_clock::now(), true).ToString() << endl;


	return 0;
}
