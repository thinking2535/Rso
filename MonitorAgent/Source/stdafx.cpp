// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// RsoMailTest.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

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
