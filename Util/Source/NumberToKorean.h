#pragma once


#include "Base.h"
#include <string>


namespace rso
{
	namespace util
	{
		using namespace std;

		class CNumberToKorean
		{
			using _TValue = uint64;

			static const wchar_t* c_1;
			static const wchar_t* c_2;
			static const wchar_t* c_3;

		public:
			static wstring Get(_TValue Value_);
		};
	}
}