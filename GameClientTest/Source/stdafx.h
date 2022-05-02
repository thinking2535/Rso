#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <Rso/Game/Client.h>
#include <Rso/Json/Json.h>
#include <Rso/Util/Input.h>
#include "../../GameServerTest/Source/Protocol.h"
#include "../../../Server/Common/Source/NetProtocol.h"

using namespace std;
using namespace rso;
using namespace game;
using namespace json;
using namespace util;

using TClient = game::CClient;
using TNet = unique_ptr<TClient>;
using TRecvFuncs = CList<TRecvFunc>;

extern TNet g_Net;
extern CLog g_Log;
extern CInput g_Input;

extern TRecvFuncs g_BinderRecvS;

template<typename _TType> struct SBinder {};
template<> struct SBinder<SChatNetCs> { static const EProtoNetCs ProtoNum = EProtoNetCs::Chat; };
template<> struct SBinder<SFCMTokenSetNetCs> { static const EProtoNetCs ProtoNum = EProtoNetCs::FCMTokenSet; };
template<> struct SBinder<SFCMTokenUnsetNetCs> { static const EProtoNetCs ProtoNum = EProtoNetCs::FCMTokenUnset; };
template<> struct SBinder<SFCMCanPushAtNightNetCs> { static const EProtoNetCs ProtoNum = EProtoNetCs::FCMCanPushAtNight; };
template<> struct SBinder<SPurchaseNetCs> { static const EProtoNetCs ProtoNum = EProtoNetCs::Purchase; };
template<> struct SBinder<SChangeNickNetCs> { static const EProtoNetCs ProtoNum = EProtoNetCs::ChangeNick; };

template<typename _TProto, typename... _TProtos>
void Send(const CKey& Key_, const _TProto& Proto_, const _TProtos&... Protos_)
{
	g_Net->Send(Key_, SBinder<_TProto>::ProtoNum, Proto_, Protos_...);
}

#define LOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#ifdef _DEBUG
#define DLOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#else
#define DLOG(...) __noop(__VA_ARGS__)
#endif
