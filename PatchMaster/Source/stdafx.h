#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Rso/Patch/Master.h>
#include <Rso/GameUtil/OptionJson.h>
#include <Rso/Log/Log.h>
#include "Protocol.h"
#include <iostream>
#include <thread>

using namespace std;
using namespace rso;
using namespace patch;
using namespace gameutil;
using namespace log;

extern CLog g_Log;

#define LOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#ifdef _DEBUG
#define DLOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#else
#define DLOG(...) __noop(__VA_ARGS__)
#endif
