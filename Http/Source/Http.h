#pragma once

#include "Session.h"

namespace rso::http
{
	using namespace core;

	template<typename... TArgs>
	class CHttp final
	{
	public:
		using TDownloadChangedFunc = typename CSession<TArgs...>::TDownloadChangedFunc;
		using TDownloadCompletedFunc = typename CSession<TArgs...>::TDownloadCompletedFunc;

	private:
		CLog _Log{ Target_Console | Target_File, EPeriod::Day, L"HttpLog/", L"Normal", ELocale::Korean };
		vector<unique_ptr<CSession<TArgs...>>> _Sessions;
		size_t _CurThreadNum = 0;
		TDownloadChangedFunc _DownloadChangedFunc;
		TDownloadCompletedFunc _DownloadCompletedFunc;

		void _ErrorLog(void)
		{
			DWORD ErrorNo = GetLastError();
			CHAR* pErrMsg = NULL;
			DWORD Ret = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, ErrorNo,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPSTR)&pErrMsg, 0, NULL);

			if (pErrMsg != NULL && Ret != 0)
				_Log.Push(StringFormat("[%d] %s", ErrorNo, pErrMsg));

			if (pErrMsg != NULL)
				LocalFree(pErrMsg);
		}

	public:
		CHttp(size_t SessionCnt_, TDownloadChangedFunc DownloadChangedFunc_, TDownloadCompletedFunc DownloadCompletedFunc_) :
			_DownloadChangedFunc(DownloadChangedFunc_),
			_DownloadCompletedFunc(DownloadCompletedFunc_)
		{
			if (SessionCnt_ <= 0)
				THROWEX();

			_Sessions.reserve(SessionCnt_);

			for (size_t i = 0; i < SessionCnt_; ++i)
			{
				CHAR Agent[1024] = {};
				printf(Agent, "RsoHttp%d", i);
				_Sessions.emplace_back(new CSession(i, Agent, _DownloadChangedFunc, _DownloadCompletedFunc, std::bind(&CHttp::_ErrorLog, this)));
			}
		}
		void Push(const SInObj& Obj_, const TArgs&... Args_)
		{
			++_CurThreadNum;
			_CurThreadNum %= _Sessions.size();
			_Sessions[_CurThreadNum]->Push(Obj_, Args_...);
		}
		void Proc(void)
		{
			for (size_t i = 0; i < _Sessions.size(); ++i)
				_Sessions[i]->Proc();
		}
	};
}