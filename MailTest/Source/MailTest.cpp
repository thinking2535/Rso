// RsoMailTest.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <Rso/Mail/Mail.h>


using namespace rso;
using namespace mail;


int _tmain(int argc, _TCHAR* argv[])
{
	CMail	mail;

	mail.Send( "ok", "thinking2535@gmail.co.kr", "title", "body" );

	return 0;
}

