#include "Base.h"

namespace rso::mssql
{
	SConnectInfo::SConnectInfo(const SConnInfo& ConnInfo_) :
		QueryTimeOut(ConnInfo_.QueryTimeOut)
	{
		wstringstream Str;
		Str << L"DRIVER={" << SQLNCLI_PRODUCT_NAME_SHORT_VER_UNICODE << L"};SERVER=";
		Str << ConnInfo_.ServerName;
		Str << L",";
		Str << ConnInfo_.Port;
		Str << L";UID=";
		Str << ConnInfo_.UserName;
		Str << L";PWD=";
		Str << ConnInfo_.PassWord;
		Str << L";DATABASE=";
		Str << ConnInfo_.DataBase;
		ConnStr = Str.str();
	}
	bool SqlError(CLog& Log_, SQLHENV hENV_, SQLHDBC hDBC_, SQLHSTMT hSTMT_)
	{
		SQLINTEGER NativeError = 0;
		SQLSMALLINT Len = 0;
		WCHAR ErrMsg[1024] = {};

		bool Connected = true;

		while (true)
		{
			auto Ret = SQLErrorW(hENV_, hDBC_, hSTMT_, NULL, &NativeError, (SQLWCHAR*)ErrMsg, (sizeof(ErrMsg) / sizeof(WCHAR)), &Len);
			if (Ret == SQL_NO_DATA)
				break;

			if (!SQL_SUCCEEDED(Ret))
			{
				Log_.Push(StringFormat(L"SQLError() Fail TReturn[%d] LastError[%d]", Ret, GetLastError()));
				break;
			}

			if (NativeError == 10054)
				Connected = false;

			Log_.Push(StringFormat(L"%s [NativeError]%d [LastError]%d", ErrMsg, NativeError, GetLastError()));
		}

		return Connected;
	}
}
