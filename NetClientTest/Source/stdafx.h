// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
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
