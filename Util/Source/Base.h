#pragma once

#include <Rso/Core/Core.h>

#pragma comment( lib, "Rso/Util_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

namespace rso
{
	namespace util
	{
		using namespace core;

		bool IsEmail(const wstring& Str_);

		template<typename TType>
		string NumberToBinaryStr(TType Value_)
		{
			if (!numeric_limits<TType>::is_integer)
				return string();

			string Str(sizeof(TType) * 8, '0');

			for (int i = 0; i < sizeof(TType) * 8; ++i)
			{
				if (Value_ & 1)
					Str[sizeof(TType) * 8 - i - 1] = '1';

				Value_ >>= 1;
			}

			return Str;
		}
	}
}
