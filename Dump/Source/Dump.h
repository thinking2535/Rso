#pragma once

#pragma comment( lib, "Dbghelp.lib" )

#include <filesystem>
#include "Base.h"
#include <sstream>
#include <stdlib.h>
#include <rtcapi.h>
#include <dbghelp.h>

namespace rso
{
	namespace dump
	{
		using namespace std;
		using namespace filesystem;

		class CDump
		{
		public:
			struct SMiniDumpInfo
			{
				DWORD ThreadID;
				EXCEPTION_POINTERS* pException;

				SMiniDumpInfo()
				{
					ThreadID = 0;
					pException = NULL;
				}
			};

		private:
			static void PurecallHandler(void);
			static void InvalidParamHandler(const wchar_t* Expression_, const wchar_t* Function_, const wchar_t* File_, uint32 Line_, uintptr_t pReserved_);
			static int _cdecl RTCHandler(int i, const char* Function_, int Line_, const char* e, const char* m, ...);
			static LONG __stdcall CrashHandler(EXCEPTION_POINTERS * pExPtrs);
			static uint32 __stdcall CreateDumpProc(void* pParam_);
			static uint32 __stdcall Dump(HANDLE ProcessHandle_, DWORD ProcessID_, MINIDUMP_EXCEPTION_INFORMATION* pMDEI_);

		public:
			CDump();

			bool UserDump(DWORD ProcessID_);
		};
	}
}
