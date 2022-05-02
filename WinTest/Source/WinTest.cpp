// RsoMailTest.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <Rso/Win/Win.h>
#include <Rso/Win/Cmd.h>
#include <iostream>
#include <memory>

using namespace std;
using namespace rso;
using namespace win;

void Callback(const string& Out_)
{
	cout << Out_;
}

int _tmain(int argc, _TCHAR* argv[])
{
	unique_ptr<CCmd> Cmd(new CCmd(Callback));

	Cmd->Input(CCmd::SInObject{ L"svn update", L"." });

	while (true)
	{
		Cmd->Proc();
		this_thread::sleep_for(milliseconds(10));
	}

	return 0;
}
