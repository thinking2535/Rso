#pragma once


#include "Base.h"


namespace rso
{
	namespace crypto
	{
		class CUri
		{
		private:
			static bool IsSafe(const char Char_);
			static char Hex2Dec(const char Char_);

		public:
			static string Encode(const string& Str_);
			static string Decode(const string& Str_);
		};
	}
}
