#pragma once

#include "Base.h"
#include <string>
#include <list>
#include <tlhelp32.h>
#include <psapi.h>

namespace rso
{
	namespace win
	{
		using namespace std;

		DWORD GetTick(void);
		int64 GetUID(void);
		uint32 Rand(uint32 Max_);
		uint64 Rand64(uint64 Max_);
		DWORD GetPIDWithProcessName(const wstring& ProcessName_);

		// delete me
		//struct SProcess
		//{
		//	PROCESSENTRY32	ProcessEntry;
		//	DWORD dwPriorityClass = 0;
		//	wstring FullPath;

		//	SProcess()
		//	{
		//		ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
		//	}
		//};
		// list<SProcess>	GetProcesses(void); bad cpu usage
		void TerminateProcessWithPID(DWORD PID_, UINT ExitCode_);
		BOOL SetConsoleCursorPos(SHORT X_, SHORT Y_);
		COORD GetConsoleCursorPos(void);
		BOOL SetConsoleCursorBack(void);
	}
}
