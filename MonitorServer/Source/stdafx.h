#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <Rso/Monitor/Server.h>
#include <Rso/GameUtil/OptionJson.h>
#include <Rso/Json/Json.h>
#include <Rso/Log/Log.h>
#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include "Protocol.h"

using namespace std;
using namespace rso;
using namespace monitor;
using namespace gameutil;
using namespace json;
using namespace log;
using namespace chrono;

using TAccounts = map<wstring, wstring>;

extern TAccounts g_Account;
extern unique_ptr<monitor::CServer> g_Server;
extern CLog g_Log;

#define LOG(...) g_Log.Push(StringFormat(__VA_ARGS__))

