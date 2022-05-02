#include "stdafx.h"

#include <Windows.h>
#include <Rso/Proto/Proto.h>
#include <iostream>
#include <fstream>

using namespace rso;
using namespace proto;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	wstring SourceFile(L"Source/Protocol");
	wstring IncludePath(L"");

	wcout << L"SourceFile : " << SourceFile << endl;
	try
	{
		wstring CurPath;
		wchar_t wCurPath[2048] = {};
		GetCurrentDirectoryW(2048, wCurPath);

		auto Index = IncludePath.find_first_of(L':');
		if (Index == wstring::npos)
		{
			IncludePath = L"\\" + IncludePath;
			IncludePath = (wCurPath + IncludePath);
		}

		{
			auto* pProto = new CProtoCPP(IncludePath, SourceFile, L"");
			if (pProto == 0)
				THROWEX();

			pProto->Export();
			delete pProto;
		}

		{
			auto* pProto = new CProtoCS(IncludePath, SourceFile, L"");
			if (pProto == 0)
				THROWEX();

			pProto->Export();
			delete pProto;
		}

		wcout << L"Export Done" << endl;
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
	}

	return 0;
}
