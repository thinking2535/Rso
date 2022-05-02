#include "stdafx.h"


TNet g_Net;
CLog g_Log(ETarget::Target_Console | ETarget::Target_File, EPeriod::Day, L"Log/", L"Log", ELocale::Korean);
CInput g_Input;

TRecvFuncs g_BinderRecvM;
TRecvFuncs g_BinderRecvC;
