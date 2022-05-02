#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	COptionJson<SStrIPPort> Option(L"Option.ini", true);

	CIPFinderClient Client;

	cout << LongIPToString(Client.GetMyLongIP(*Option)) << endl;

	return 0;
}
