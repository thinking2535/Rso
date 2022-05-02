// RsoMailTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

