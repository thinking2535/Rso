#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <Rso/Game/Server.h>
#include <Rso/GameUtil/OptionJson.h>
#include <Rso/Util/Input.h>
#include <Rso/Base/Period.h>
#include "../../GameMasterTest/Source/Protocol.h"
#include "Protocol.h"
#include "../../../Server/Common/Source/NetProtocol.h"

using namespace std;
using namespace rso;
using namespace game;
using namespace base;
using namespace gameutil;
using namespace util;

using TServer = game::CServer;
using TNet = unique_ptr<TServer>;
using TRecvFuncs = CList<TRecvFunc>;
using TOutObject = TServer::TOutObject;
using TSessionsIt = TServer::TSessionsIt;

extern TNet g_Net;
extern CLog g_Log;
extern CInput g_Input;

extern TRecvFuncs g_BinderRecvM;
extern TRecvFuncs g_BinderRecvC;

template<typename _TType> struct SBinderM {};
template<> struct SBinderM<SGameProtoNetSm> { static const EProtoNetSm ProtoNum = EProtoNetSm::GameProto; };

template<typename _TType> struct SBinder {};
template<> struct SBinder<SRetNetSc> { static const EProtoNetSc ProtoNum = EProtoNetSc::Ret; };
template<> struct SBinder<SMsgNetSc> { static const EProtoNetSc ProtoNum = EProtoNetSc::Msg; };
template<> struct SBinder<SLoginNetSc> { static const EProtoNetSc ProtoNum = EProtoNetSc::Login; };
template<> struct SBinder<SLobbyNetSc> { static const EProtoNetSc ProtoNum = EProtoNetSc::Lobby; };
template<> struct SBinder<SChatNetSc> { static const EProtoNetSc ProtoNum = EProtoNetSc::Chat; };
template<> struct SBinder<SPurchaseNetSc> { static const EProtoNetSc ProtoNum = EProtoNetSc::Purchase; };
template<> struct SBinder<SChangeNickNetSc> { static const EProtoNetSc ProtoNum = EProtoNetSc::ChangeNick; };
template<> struct SBinder<SChangeNickFailNetSc> { static const EProtoNetSc ProtoNum = EProtoNetSc::ChangeNickFail; };


template<typename _TProto, typename... _TProtos>
inline void SendM(TPeerCnt PeerNum_, const _TProto& Proto_, const _TProtos&... Protos_)
{
	g_Net->SendM(PeerNum_, SBinderM<_TProto>::ProtoNum, Proto_, Protos_...);
}
template<typename _TProto, typename... _TProtos>
inline void SendMAll(const _TProto& Proto_, const _TProtos&... Protos_)
{
	g_Net->SendMAll(SBinderM<_TProto>::ProtoNum, Proto_, Protos_...);
}

template<typename _TProto, typename... _TProtos>
inline void Send(const CKey& Key_, const _TProto& Proto_, const _TProtos&... Protos_)
{
	g_Net->Send(Key_, SBinder<_TProto>::ProtoNum, Proto_, Protos_...);
}
template<typename _TProto, typename... _TProtos>
inline void SendAll(const _TProto& Proto_, const _TProtos&... Protos_)
{
	g_Net->SendAll(SBinder<_TProto>::ProtoNum, Proto_, Protos_...);
}

#define LOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#ifdef _DEBUG
#define DLOG(...) g_Log.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#else
#define DLOG(...) __noop(__VA_ARGS__)
#endif
