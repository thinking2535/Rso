#include "Base.h"


namespace rso
{
	namespace util
	{
		bool IsEmail(const wstring& Str_)
		{
			auto AtIndex = Str_.find_first_of(L"@");
			if (AtIndex == wstring::npos)
				return false;

			auto DotIndex = Str_.find_last_of(L".");
			if (DotIndex == wstring::npos)
				return false;

			if (AtIndex <= 0 ||
				DotIndex + 1 >= Str_.size())
				return false;

			if (DotIndex <= AtIndex)
				return false;

			return true;
		}
	}
}
