#pragma once

#include "Base.h"
#include <string>

namespace rso
{
	namespace crypto
	{
		using namespace std;

		class CCrypto
		{
		private:
			unsigned char _aKey[8];
			unsigned char _Index = 0;
			unsigned char _Bit = 0;

			unsigned char _GetBit(void);
			void _SetKey(uint64 Key_);

		public:
			void Encode(unsigned char* pData_io, uint64 Size_, uint64 Key_);
			void Decode(unsigned char* pData_io, uint64 Size_, uint64 Key_);
		};
	}
}
