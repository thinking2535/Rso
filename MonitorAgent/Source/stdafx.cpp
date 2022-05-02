// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// RsoMailTest.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

SERVICE_STATUS g_ServiceStatus = { 0 };
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;
wstring g_ServiceName;
unique_ptr<CLog> g_Log;
wstring g_WorkPath;
wstring g_DynamicOptionFileName(L"DynamicOption.ini");
wstring g_AppWorkPath;
unique_ptr<CAgent> g_Agent;
