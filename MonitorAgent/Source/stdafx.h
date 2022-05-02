#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Rso/GameUtil/OptionJson.h>
#include <Rso/Monitor/Agent.h>
#include <Rso/Base/Base.h>
#include <Rso/Log/Log.h>
#include <Rso/Win/Win.h>
#include <string>
#include <shellapi.h>
#include <process.h>
#include <filesystem>
#include "Protocol.h"

using namespace std;
using namespace rso;
using namespace gameutil;
using namespace log;
using namespace monitor;
using namespace win;

extern SERVICE_STATUS g_ServiceStatus;
extern SERVICE_STATUS_HANDLE g_StatusHandle;
extern HANDLE g_ServiceStopEvent;
extern wstring g_ServiceName;
extern unique_ptr<CLog> g_Log;
extern wstring g_WorkPath;
extern wstring g_DynamicOptionFileName;
extern wstring g_AppWorkPath;
extern unique_ptr<CAgent> g_Agent;

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler(DWORD);
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);

#define LOG(...) g_Log->Push(StringFormat(__VA_ARGS__))
