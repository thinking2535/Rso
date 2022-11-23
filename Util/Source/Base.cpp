#include "Base.h"


namespace rso::util
{
	bool IsEmail(const wstring& Str_)
	{
		const auto AtIndex = Str_.find_first_of(L"@");
		if (AtIndex == wstring::npos)
			return false;

		const auto DotIndex = Str_.find_last_of(L".");
		if (DotIndex == wstring::npos)
			return false;

		if (AtIndex <= 0 ||
			DotIndex + 1 >= Str_.size())
			return false;

		if (DotIndex <= AtIndex)
			return false;

		return true;
	}
	wstring newGuid()
	{
		GUID guid;
		const auto result = CoCreateGuid(&guid);
		if (result != S_OK)
			THROWEXA(L"CoCreateGuid fail [%d]", result);

		wchar_t wszCLSID[129]{};
		const auto length = StringFromGUID2(guid, wszCLSID, _countof(wszCLSID) - 1);
		if (length <= 0)
			THROWEXA(L"StringFromGUID2 fail");

		return wstring(wszCLSID);
	}
}
