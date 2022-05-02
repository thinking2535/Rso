#include "Win.h"

namespace rso
{
	namespace win
	{
		DWORD GetTick(void)
		{
			LARGE_INTEGER liEndCounter, liFrequency;

			QueryPerformanceCounter(&liEndCounter);
			QueryPerformanceFrequency(&liFrequency);

			return (DWORD)(liEndCounter.QuadPart / (liFrequency.QuadPart / 1000.0));
		}
		int64 GetUID(void)
		{
			// Max DateTime 2958465.99999 
			// Date		Time	Counter
			// FFFFFF	FFFFF	FFFFF

			int64		UID = 0;

			// Second
			UID = system_clock::now().time_since_epoch().count();
			UID <<= 20;

			// Counter
			LARGE_INTEGER Counter;
			QueryPerformanceCounter(&Counter);

			UID += (Counter.QuadPart & 0xFFFFF);

			return UID;
		}
		uint32 Rand(uint32 Max_)
		{
			if (Max_ == 0)
				return 0;

			srand((uint32)GetUID());

			return (uint32)rand() % Max_;
		}
		uint64 Rand64(uint64 Max_)
		{
			if (Max_ == 0)
				return 0;

			srand((uint32)GetUID());

			return (((uint64(rand()) << 32) + uint64(rand())) % Max_);
		}
		DWORD GetPIDWithProcessName(const wstring& ProcessName_)
		{
			DWORD PIDs[1024];
			DWORD cbNeeded = 0;

			if (!EnumProcesses(PIDs, sizeof(PIDs), &cbNeeded))
				return 0;

			auto ProcessCount = (cbNeeded / sizeof(DWORD));
			WCHAR FileName[MAX_PATH + 1] = {};

			for (DWORD i = 0; i < ProcessCount; ++i)
			{
				auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PIDs[i]);
				if (hProcess == 0)
					continue;

				if (GetModuleFileNameEx(hProcess, 0, FileName, MAX_PATH))
				{
					if (path_compare(wstring(FileName), ProcessName_) == 0)
					{
						CloseHandle(hProcess);
						return PIDs[i];
					}
				}

				CloseHandle(hProcess);
			}

			return 0;
		}
		void TerminateProcessWithPID(DWORD PID_, UINT ExitCode_)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, PID_);
			if (hProcess == NULL)
				return;

			TerminateProcess(hProcess, ExitCode_);

			CloseHandle(hProcess);
		}
		BOOL SetConsoleCursorPos(SHORT X_, SHORT Y_)
		{
			HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
			if (Handle == INVALID_HANDLE_VALUE)
				return FALSE;

			return SetConsoleCursorPosition(Handle, COORD{ X_, Y_ });
		}
		COORD GetConsoleCursorPos(void)
		{
			HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
			if (Handle == INVALID_HANDLE_VALUE)
				return COORD{ 0, 0 };

			CONSOLE_SCREEN_BUFFER_INFO csbi;
			if (GetConsoleScreenBufferInfo(Handle, &csbi) == FALSE)
				return COORD{ 0, 0 };

			return csbi.dwCursorPosition;
		}
		BOOL SetConsoleCursorBack(void)
		{
			HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
			if (Handle == INVALID_HANDLE_VALUE)
				return FALSE;

			CONSOLE_SCREEN_BUFFER_INFO csbi;
			if (GetConsoleScreenBufferInfo(Handle, &csbi) == FALSE)
				return FALSE;

			if (csbi.dwCursorPosition.X <= 0)
				return TRUE;

			--csbi.dwCursorPosition.X;

			if (SetConsoleCursorPosition(Handle, csbi.dwCursorPosition) == FALSE)
				return FALSE;

			DWORD dwBytesWritten = 0;
			if (WriteConsoleOutputCharacter(Handle, L" ", 1, csbi.dwCursorPosition, &dwBytesWritten))
				return FALSE;

			return TRUE;
		}
	}
}
