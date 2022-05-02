#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	COptionJson<SStrIPPort> Option(L"Option.ini", true);
	CIPFinderServer Server(*Option);

	while (true)
	{
		Server.Proc();
		this_thread::sleep_for(milliseconds(1));
	}

	return 0;
}
