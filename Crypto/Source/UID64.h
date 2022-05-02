#pragma once

#include "Base.h"

namespace rso
{
	namespace crypto
	{
		using namespace base;

		class CUID64
		{
			static const uint64 _ParityCount = 5;
			inline static const u16string _StringTable = u"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+="s;

			array<size_t, 0xFF> _IndexTable;
			uint64 _BitCount = 0;
			uint64 _Bit = 1;
			uint64 _WindowCount = 0;
			uint64 _PreventMinusNumber = 0;
			uint64 _Key = 0;

			uint64 _GetEncodedSize(void) const;
			u16string _NumberToString(uint64 Number_);
			uint64 _StringToNumber(const u16string& String_);
			u16string _AddParity(const u16string& String_);
			u16string _SubParity(const u16string& String_);

		public:
			CUID64(uint64 Key_);
			u16string Encode(uint64 UID_);
			uint64 Decode(const u16string& UIDE_); // -0 : error
		};
	}
}
