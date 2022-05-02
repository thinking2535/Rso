#include "Dump.h"
#include <process.h>


namespace rso
{
	namespace dump
	{
		void CDump::PurecallHandler(void)
		{
			RaiseException(EXCEPTION_INVALID_HANDLE, 0, 0, NULL);
		}
		void CDump::InvalidParamHandler(const wchar_t*, const wchar_t*, const wchar_t *, uint32, uintptr_t)
		{
			RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, NULL);
		}
		int _cdecl CDump::RTCHandler(int, const char*, int, const char*, const char*, ...)
		{
			RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, NULL);

			return 0;
		}
		LONG __stdcall CDump::CrashHandler(EXCEPTION_POINTERS * pExPtrs)
		{
			SMiniDumpInfo Info;
			Info.ThreadID = GetCurrentThreadId();
			Info.pException = pExPtrs;

			if (pExPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
			{
				HANDLE hThreadID = (HANDLE)_beginthreadex(0, (1024 * 1024 * 2), CreateDumpProc, &Info, 0, NULL);

				WaitForSingleObject(hThreadID, INFINITE);

				CloseHandle(hThreadID);
			}
			else
			{
				CreateDumpProc(&Info);
			}

			return EXCEPTION_EXECUTE_HANDLER;
		}
		uint32 __stdcall CDump::CreateDumpProc(void* pParam_)
		{
			SMiniDumpInfo* pInfo = (SMiniDumpInfo*)pParam_;
			MINIDUMP_EXCEPTION_INFORMATION stMDEI;

			stMDEI.ThreadId = pInfo->ThreadID;
			stMDEI.ExceptionPointers = pInfo->pException;
			stMDEI.ClientPointers = TRUE;

			return Dump(GetCurrentProcess(), GetCurrentProcessId(), &stMDEI);
		}
		uint32 CDump::Dump(HANDLE ProcessHandle_, DWORD ProcessID_, MINIDUMP_EXCEPTION_INFORMATION* pMDEI_)
		{
			SYSTEMTIME SystemTime;
			GetLocalTime(&SystemTime);

			wstringstream DumpFile;
			DumpFile.fill(L'0');
			DumpFile.width(2);
			DumpFile << SystemTime.wYear;
			DumpFile.width(2);
			DumpFile << SystemTime.wMonth;
			DumpFile.width(2);
			DumpFile << SystemTime.wDay;
			DumpFile << L'_';
			DumpFile.width(2);
			DumpFile << SystemTime.wHour;
			DumpFile.width(2);
			DumpFile << SystemTime.wMinute;
			DumpFile.width(2);
			DumpFile << SystemTime.wSecond;
			DumpFile << L".dmp";

			HANDLE hFile = CreateFile(
				absolute(DumpFile.str()).wstring().c_str(),
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (hFile == INVALID_HANDLE_VALUE) return 0;

			BOOL bRet = MiniDumpWriteDump(ProcessHandle_, ProcessID_, hFile, MiniDumpWithFullMemory, pMDEI_, NULL, NULL);

			CloseHandle(hFile);

			if (FALSE == bRet) return 0;

			return 1;
		}
		CDump::CDump()
		{
			if (_set_purecall_handler(PurecallHandler) != NULL ||
				_set_invalid_parameter_handler(InvalidParamHandler) != NULL ||
				_RTC_SetErrorFunc(RTCHandler) != NULL ||
				SetUnhandledExceptionFilter(CrashHandler) == NULL)
				THROWEX();
		}
		bool CDump::UserDump(DWORD ProcessID_)
		{
			HANDLE ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID_);

			if (ProcessHandle == NULL) return false;

			Dump(ProcessHandle, ProcessID_, NULL);

			CloseHandle(ProcessHandle);

			return true;
		}
	}
}
