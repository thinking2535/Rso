#include "stdafx.h"
#include <iostream>
#include <string>
#include <Rso/Ranking/Server.h>

using namespace std;
using namespace rso;
using namespace ranking;

int main(void)
{
	ranking::CServer<int, string> s(100, SDBOption());

	return 0;
}
