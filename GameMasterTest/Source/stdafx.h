// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <Rso/Game/Master.h>
#include <Rso/GameUtil/OptionJson.h>
#include "Protocol.h"

using namespace std;
using namespace rso;
using namespace game;
using namespace gameutil;

using TNet = unique_ptr<game::CMaster>;
using TRecvFuncs = CList<TRecvFunc>;

extern TNet g_Net;
extern CLog g_Log;
extern TRecvFuncs g_BinderRecvS;

#define LOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#ifdef _DEBUG
#define DLOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#else
#define DLOG(...) __noop(__VA_ARGS__)
#endif



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
