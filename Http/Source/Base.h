#pragma once

#include <Rso/Core/Base.h>

#pragma comment( lib, "Rso/Http_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <functional>
#include <string>
#include <Rso/Core/Stream.h>
#include <Rso/Base/LFQueue.h>
#include <Rso/Base/Thread.h>
#include <Rso/Log/Log.h>

namespace rso::http
{
	using namespace std;
	using namespace win;
	using namespace base;
	using namespace log;

	using THttpPort = uint16;
	using TCmdCnt = int32;
	using TErrorLogFunc = function<void(void)>;

	const TCmdCnt c_CmdNum_Null = (~0);
	const DWORD c_SetQueDelayTick = 50;

	enum class EMethod
	{
		Get,
		Post,
		Max
	};
	enum class EHttpRet
	{
		Ok,
		InternetConnectFail,
		OpenRequestFail,
		InternetQueryOptionFail,
		InternetSetOptionFail,
		AddRequestHeadersFail,
		SendRequestFail,
		InternetReadFileFail,
		QueryInfoFail,
		InternetQueryDataAvailableFail,
		InvalidAvailableSize,
		InvalidReadSize,
		NotEnoughMemory,
		SystemError,
		Max,
		Null
	};

	using TMethos = array<const char*, size_t(EMethod::Max)>;

	static const TMethos c_Methos = { "GET", "POST" };

	struct SInObj
	{
		string Header;
		string ServerName;
		THttpPort Port;
		string UserName;
		string PassWord;
		EMethod Method;
		string ObjectName;
		string Params;
		bool Secure;

		SInObj(
			const string& Header_,
			const string& ServerName_,
			THttpPort Port_,
			const string& UserName_,
			const string& PassWord_,
			EMethod Method_,
			const string& ObjectName_,
			const string& Params_,
			bool Secure_) :
			Header(Header_),
			ServerName(ServerName_),
			Port(Port_),
			UserName(UserName_),
			PassWord(PassWord_),
			Method(Method_),
			ObjectName(ObjectName_),
			Params(Params_),
			Secure(Secure_)
		{
		}
	};
	using TBuffer = vector<char>;
}
