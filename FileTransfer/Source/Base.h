// TODO : 통신방식을 변경 후 재 적용할것 (Sync - 현재 Proc의 주기에 달려있음.)
#pragma once

#include <Rso/Core/Core.h>
#pragma comment( lib, "Rso/FileTransfer_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <functional>
#include <string>
#include <Rso/Base/Base.h>
#include <Rso/Core/Stream.h>
#include <Rso/Net/Base.h>

namespace rso
{
	namespace filetransfer
	{
		using namespace std;
		using namespace core;
		using namespace base;
		using namespace net;

		using TGetDoneFunc = function<void(TPeerCnt PeerNum_, const wstring& Name_)>;
		using TPutDoneFunc = function<void(TPeerCnt PeerNum_, const wstring& Name_)>;
		using TErrorFunc = function<void(TPeerCnt PeerNum_, const wstring& Msg_)>;
	}
}