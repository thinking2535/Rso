// TODO : ��Ź���� ���� �� �� �����Ұ� (Sync - ���� Proc�� �ֱ⿡ �޷�����.)
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