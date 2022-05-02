#include "stdafx.h"

void CallbackUserProto(const CKey& ClientKey_, CStream& Stream_)
{
}

void _tmain(int argc, WCHAR *argv[])
{
	g_WorkPath = absolute(argv[0]).wstring();
	SplitBack(L'\\', g_WorkPath);
	g_WorkPath += L'\\';

	SetCurrentDirectory(g_WorkPath.c_str());

	wstring OptionPath = g_WorkPath;
	OptionPath += L"Option.ini";

	g_Log.reset(new CLog(
		Target_File,
		EPeriod::Day,
		g_WorkPath + L"Log\\",
		L"Log",
		ELocale::Korean));

	try
	{
		COptionJson<SOption> Option(OptionPath, false);

		g_AppWorkPath = Option->WorkingRelativePath;
		if (g_AppWorkPath.back() != L'/' &&
			g_AppWorkPath.back() != L'\\')
			g_AppWorkPath += L'/';

		if (!path(g_AppWorkPath).is_absolute())
			g_AppWorkPath = (g_WorkPath + g_AppWorkPath);

		g_AppWorkPath = GetSlashPath(g_AppWorkPath);
		g_ServiceName = Option->ServiceName;

		g_Agent.reset(
			new CAgent(
				CallbackUserProto,
				g_ServiceName, g_AppWorkPath, Option->AppFile, g_WorkPath + g_DynamicOptionFileName, CNamePort(Option->ServerNamePort), CNamePort(Option->AppNamePort), wstring(L"{}")
			)
		);

		SERVICE_TABLE_ENTRY ServiceTable[] =
		{
		{ (wchar_t*)g_ServiceName.c_str(), (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
		};

		if (StartServiceCtrlDispatcher(ServiceTable) == FALSE)
			THROWEX();
	}
	catch (const exception& Exception_)
	{
		LOG(MBSToWCS(Exception_.what()).c_str());
	}
	catch (...)
	{
		LOG(L"System Error");
	}
}
VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
	try
	{
		ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
		g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

		// Register our service control handler with the SCM
		g_StatusHandle = RegisterServiceCtrlHandler(g_ServiceName.c_str(), ServiceCtrlHandler);
		if (g_StatusHandle == NULL)
			THROWEX();

		// Tell the service controller we are starting
		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwServiceSpecificExitCode = 0;
		g_ServiceStatus.dwCheckPoint = 0;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
			THROWEX();

		g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (g_ServiceStopEvent == NULL)
		{
			// Error creating event
			// Tell service controller we are stopped and exit
			g_ServiceStatus.dwControlsAccepted = 0;
			g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
			g_ServiceStatus.dwWin32ExitCode = GetLastError();
			g_ServiceStatus.dwCheckPoint = 1;

			THROWEX();
		}

		g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
		g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwCheckPoint = 0;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
			THROWEX();

		auto hThread = CreateThread(NULL, 0, ServiceWorkerThread, NULL, 0, NULL);
		if (hThread == NULL)
			THROWEX();

		LOG(L"Started");

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(g_ServiceStopEvent);

		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwCheckPoint = 3;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
			THROWEX();

		LOG(L"Stopped");
	}
	catch (...)
	{
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

		try
		{
			throw;
		}
		catch (const exception& Exception_)
		{
			LOG(MBSToWCS(Exception_.what()).c_str());
		}
		catch (...)
		{
			LOG(L"System Error");
		}
	}
}

VOID WINAPI ServiceCtrlHandler(DWORD CtrlCode)
{
	switch (CtrlCode)
	{
	case SERVICE_CONTROL_STOP:

		if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
			break;

		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwCheckPoint = 4;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
			THROWEX();

		SetEvent(g_ServiceStopEvent);
		break;

	default:
		break;
	}
}

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{
	while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0)
	{
		g_Agent->Proc();
		this_thread::sleep_for(milliseconds(1));
	}

	return ERROR_SUCCESS;
}