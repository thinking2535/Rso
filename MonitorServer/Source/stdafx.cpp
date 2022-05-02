#include "stdafx.h"

TAccounts g_Account;
unique_ptr<monitor::CServer> g_Server;
CLog g_Log(Target_Console | Target_File, EPeriod::Day, L"Log/", L"Log", ELocale::Korean);
