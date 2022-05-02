// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
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
