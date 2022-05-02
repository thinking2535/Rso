#include "Json.h"


namespace rso
{
	namespace json
	{
		CJson::CJson(const SProto& Proto_) :
			Value(Proto_)
		{
		}
		CJson::CJson(const string& String_)
		{
			if (!Reader().parse(String_, *this))
				THROWEXA(L"Json Parse Fail\n%s", MBSToWCS(String_));
		}
		CJson::CJson(const wstring& String_)
		{
			if (!Reader().parse(WCSToMBS(String_), *this))
				THROWEXA(L"Json Parse Fail\n%s", String_);
		}
		CJson::CJson(const CStream& Stream_)
		{
			if (!Reader().parse(Stream_.to_string(), *this))
				THROWEXA(L"Json Parse Fail");
		}
	}
}