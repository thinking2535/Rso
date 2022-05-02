#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <Rso/Balance/Client.h>
#include <Rso/Json/Json.h>
#include <Rso/Util/Input.h>

using namespace std;
using namespace rso;
using namespace net;
using namespace balance;
using namespace json;
using namespace util;

using TClient = balance::CClient;
using TNet = unique_ptr<TClient>;

extern TNet g_Net;
extern CLog g_Log;
extern CInput g_Input;

#define LOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#ifdef _DEBUG
#define DLOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#else
#define DLOG(...) __noop(__VA_ARGS__)
#endif
