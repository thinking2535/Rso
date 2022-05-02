#include "stdafx.h"


TNet g_Net;
vector<TNet> g_Nets;
CLog g_Log(ETarget::Target_Console | ETarget::Target_File, EPeriod::Day, L"Log/", L"Log", ELocale::Korean);
CInput g_Input;
