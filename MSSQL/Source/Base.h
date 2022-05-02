#pragma once

#include <Rso/Core/Stream.h>

#pragma comment( lib, "Rso/MSSQL_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <functional>
#include <Rso/Log/Log.h>
#include "Sql.h"
#include "Protocol.h"

namespace rso::mssql
{
	using namespace core;
	using namespace std;
	using namespace log;

	struct SConnectInfo
	{
		wstring ConnStr;
		DWORD QueryTimeOut = 0;

		SConnectInfo(const SConnInfo& ConnInfo_);
	};

	bool SqlError(CLog& Log_, SQLHENV hENV_, SQLHDBC hDBC_, SQLHSTMT hSTMT_);
}

#define SQL_LOG(Log_, ...) Log_.Push(StringFormat(__VA_ARGS__))
#ifdef _DEBG
#define SQL_DLOG(Log_, ...) Log_.Push(GET_DEBUG_FORMAT(__VA_ARGS__))
#else
#define SQL_DLOG(Log_, ...) __noop(__VA_ARGS__)
#endif
