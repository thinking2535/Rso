// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include <Rso/Json/Json.h>
#include <Rso/GameUtil/OptionJson.h>
#include <Rso/Net/Client.h>
#include "Protocol.h"
#include <Rso/Log/Log.h>

using namespace chrono;
using namespace rso;
using namespace json;
using namespace net;
using namespace gameutil;
using namespace log;

extern CLog g_Log;

#define LOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#ifdef _DEBUG
#define DLOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#else
#define DLOG(...) __noop(__VA_ARGS__)
#endif
