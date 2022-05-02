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
	if (argc < 3)
	{
		wcout << L"exe SourceFileName TargetLanguage [IncludePath] [LibraryName]" << endl;
		wcout << L"TargetLanguage : [CPP, CS, Dart]" << endl;
		return 0;
	}

	wstring SourceFile(argv[1]);
	wstring TargetLang(argv[2]);

	wstring IncludePath(argc < 4 ? L"" : argv[3]);
	wstring LibraryName(argc < 5 ? L"" : argv[4]);

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

		if (TargetLang == L"CPP")
		{
			auto* pProto = new CProtoCPP(IncludePath, SourceFile, LibraryName);
			if (pProto == 0)
				THROWEX();

			pProto->Export();
			delete pProto;
		}
		else if (TargetLang == L"CS")
		{
			auto* pProto = new CProtoCS(IncludePath, SourceFile, LibraryName);
			if (pProto == 0)
				THROWEX();

			pProto->Export();
			delete pProto;
		}
		else if (TargetLang == L"Dart")
		{
			auto* pProto = new CProtoDart(IncludePath, SourceFile, LibraryName);
			if (pProto == 0)
				THROWEX();

			pProto->Export();
			delete pProto;
		}
		else
		{
			THROWEXA(L"Invalid LanguageType [%s]", TargetLang);
		}

		wcout << L"Export Done" << endl;
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
	}

	return 0;
}
