// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
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



// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
