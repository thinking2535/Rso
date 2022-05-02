#include "Cmd.h"
#include <shellapi.h>
#pragma comment( lib, "Shell32.lib" )

namespace rso
{
	namespace win
	{
		void CCmd::_WorkerRead(const volatile bool* Exit_)
		{
			SECURITY_ATTRIBUTES SecurityAttributes;

			SecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
			SecurityAttributes.bInheritHandle = TRUE;
			SecurityAttributes.lpSecurityDescriptor = NULL;

			// Create Pipe
			if (!CreatePipe(&_CmdStdOutRdHandle.Handle(), &_CmdStdOutWrHandle.Handle(), &SecurityAttributes, 0))
				THROWEX();

			if (!SetHandleInformation(_CmdStdOutWrHandle.Handle(), HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT)) // FILE_FLAG_OVERLAPPED
				THROWEX();

			CHAR chBuf[4096];

			while (*Exit_ == false)
			{
				try
				{
					while (auto OutObject = _OutQueue.new_buf())
					{
						DWORD TotalBytesAvail = 0;
						if (!PeekNamedPipe(_CmdStdOutRdHandle.Handle(), NULL, 0, NULL, &TotalBytesAvail, NULL))
							break;

						if (TotalBytesAvail == 0)
							break;

						DWORD NumberOfBytesRead = 0;
						if (!ReadFile(_CmdStdOutRdHandle.Handle(), chBuf, sizeof(chBuf), &NumberOfBytesRead, NULL))
						{
							auto LastError = GetLastError();
							if (LastError != ERROR_IO_PENDING)
								throw LastError;
						}

						if (NumberOfBytesRead == 0) // ReadFile 내부를 알 수 없으니 만일을 위해서
							break;

						OutObject->append(chBuf, NumberOfBytesRead);
						_OutQueue.push();
					}
				}
				catch (...)
				{
				}

				this_thread::sleep_for(milliseconds(100));
			}
		}
		void CCmd::_WorkerInput(const volatile bool* Exit_)
		{
			while (*Exit_ == false)
			{
				while (auto InObject = _InQueue.get())
				{
					try
					{
						PROCESS_INFORMATION ProcessInformation;
						ZeroMemory(&ProcessInformation, sizeof(PROCESS_INFORMATION));

						STARTUPINFO StartupInfo;
						ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));

						StartupInfo.cb = sizeof(STARTUPINFO);
						StartupInfo.hStdError = _CmdStdOutWrHandle.Handle();
						StartupInfo.hStdOutput = _CmdStdOutWrHandle.Handle();
						StartupInfo.dwFlags |= STARTF_USESTDHANDLES;

						if (!CreateProcess(NULL,
							(WCHAR*)(L"cmd.exe /C " + InObject->Cmd).c_str(),       // command line 
							NULL,          // process security attributes 
							NULL,          // primary thread security attributes 
							TRUE,          // handles are inherited 
							0,             // creation flags 
							NULL,          // use parent's environment 
							InObject->WorkDir.c_str(),  // use parent's current directory 
							&StartupInfo,  // STARTUPINFO pointer 
							&ProcessInformation)) // receives PROCESS_INFORMATION 
							throw 0;

						WaitForSingleObject(ProcessInformation.hProcess, INFINITE);
						CloseHandle(ProcessInformation.hProcess);
						CloseHandle(ProcessInformation.hThread);
						InObject->clear();
					}
					catch (...)
					{
					}

					_InQueue.pop();
				}

				this_thread::sleep_for(milliseconds(100));
			}
		}
		CCmd::CCmd(TOutCallback OutCallback_) :
			_OutCallback(OutCallback_),
			_WorkerReadThread(std::bind(&CCmd::_WorkerRead, this, _1)), // 초기화 순서 중요
			_WorkerInputThread(std::bind(&CCmd::_WorkerInput, this, _1)) // 초기화 순서 중요
		{
		}
		void CCmd::Input(const SInObject& Object_)
		{
			auto InObject = _InQueue.new_buf();
			(*InObject) = Object_;
			_InQueue.push();
		}
		void CCmd::Proc(void)
		{
			for (auto OutObject = _OutQueue.get();
				OutObject;
				OutObject = _OutQueue.get())
			{
				_OutCallback(*OutObject);
				OutObject->clear();
				_OutQueue.pop();
			}
		}
	}
}
