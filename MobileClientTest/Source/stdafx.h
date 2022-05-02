// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <Rso/Json/Json.h>
#include <Rso/Mobile/Client.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <Rso/Base/ListVoid.h>
#include <Rso/Log/Log.h>

using namespace std;
using namespace chrono;
using namespace rso;
using namespace base;
using namespace mobile;
using namespace log;
using namespace json;


extern CLog g_Log;

#define LOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#ifdef _DEBUG
#define DLOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#else
#define DLOG(...) __noop(__VA_ARGS__)
#endif
