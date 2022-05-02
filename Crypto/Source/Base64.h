#pragma once

#include "Base.h"
#include <string>

namespace rso
{
	namespace crypto
	{
		using namespace std;

		class CBase64
		{
		private:
			static const string c_Base64Chars;
			static bool IsBase64(char C_) { return (isalnum(C_) || (C_ == '+') || (C_ == '/')); }
		public:
			CBase64() {}
			~CBase64() {}
			static string	Encode(const string& Str_);
			static string	Decode(const string& Str_);
		};
	}
}
