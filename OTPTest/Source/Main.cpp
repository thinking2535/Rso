#include "stdafx.h"
#include <Rso/OTP/OTP.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;
using namespace rso;
using namespace otp;

int _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned int)time(0));
	auto r0 = rand();
	auto r1 = rand();

	uint64 Seed = ((uint64)r0 << 32) + (uint64)r1;

	COTP Server(Seed);
	COTP Client(Seed);

	while (true)
	{
		auto OutValue = Client.Get();
		cout.width(6);
		cout.fill('0');
		cout << OutValue << "	";

		if (Server.IsValid(OutValue))
			cout << "Ok" << endl;
		else
			cout << "Error !!!!!!!!!!!!!" << endl;
	}

	return 0;
}
