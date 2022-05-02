#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <Rso/Json/Json.h>
#include <Rso/Net/Server.h>
#include <iostream>
#include <Rso/Log/Log.h>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace rso;
using namespace json;
using namespace net;
using namespace chrono;
using namespace log;

extern CLog g_Log;

#define LOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#ifdef _DEBUG
#define DLOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#else
#define DLOG(...) __noop(__VA_ARGS__)
#endif
