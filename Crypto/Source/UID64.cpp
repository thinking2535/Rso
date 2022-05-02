#include "UID64.h"

// [[Char + Parity0 + Parity1 + Parity2 + Parity3 + Parity4] * WindowCount]

namespace rso
{
	namespace crypto
	{
		uint64 CUID64::_GetEncodedSize(void) const
		{
			return _WindowCount * (_ParityCount + 1);
		}
		u16string CUID64::_NumberToString(uint64 Number_)
		{
			Number_ ^= _Key;

			u16string Encoded;
			Encoded.reserve(_WindowCount);

			uint64 Mod = 0;
			for (uint64 i = 0; i < _WindowCount; ++i)
			{
				Mod ^= Number_ & _Bit;
				Number_ >>= _BitCount;
				Encoded += _StringTable[Mod];
			}

			return Encoded;
		}
		uint64 CUID64::_StringToNumber(const u16string& String_)
		{
			uint64 Decoded = 0;

			for (size_t i = String_.size(); i > 1; --i)
			{
				Decoded <<= _BitCount;
				Decoded += (_IndexTable[(char)(String_[i - 1])] ^ _IndexTable[(char)(String_[i - 2])]);
			}
			Decoded <<= _BitCount;
			Decoded += _IndexTable[(char)(String_[0])];

			Decoded ^= _Key;

			return Decoded;
		}
		u16string CUID64::_AddParity(const u16string& String_)
		{
			u16string Added;
			Added.reserve(_GetEncodedSize());

			uint64 Number = 0;
			for (auto& i : String_)
				Number += i;

			for (size_t i = String_.size(); i > 0; --i)
			{
				Added += String_[i - 1];

				// Add Parity
				for (size_t j = 0; j < _ParityCount; ++j)
				{
					Number += Added.back();
					Added += _StringTable[Number % _StringTable.size()];
				}
			}

			return Added;
		}
		u16string CUID64::_SubParity(const u16string& String_)
		{
			uint64 Number = _IndexTable[(char)String_.back()];
			u16string Subed;
			Subed.reserve(_WindowCount);

			for (size_t i = String_.size(); i > 0; --i)
			{
				if ((i - 1) % (_ParityCount + 1) == 0)
				{
					Subed += String_[i - 1];
				}
				else // Check Parity
				{
					if (String_[i - 1] != _StringTable[Number % _StringTable.size()])
						THROWEX();

					if (Number < 0xFF)
						Number += _PreventMinusNumber;

					Number -= String_[i - 2];
				}
			}

			return Subed;
		}
		CUID64::CUID64(uint64 Key_) :
			_Key(Key_)
		{
			// shuffle _StringTable
			uint64 TargetIndex = Key_;
			for (size_t i = 0; i < _StringTable.size(); ++i)
			{
				swap((char16_t&)_StringTable[i], (char16_t&)_StringTable[TargetIndex % _StringTable.size()]);
				uint64 LastBitToFirst = (TargetIndex << 63);
				TargetIndex >>= 1;
				TargetIndex += LastBitToFirst;
			}

			for (size_t i = 0; i < _StringTable.size(); ++i)
				_IndexTable[(size_t)_StringTable[i]] = i;

			uint64 i = 1;
			for (; i < _StringTable.size(); i *= 2)
				++_BitCount;

			if (_BitCount == 0)
				THROWEX();

			_Bit <<= _BitCount;
			if (_StringTable.size() != _Bit)
				THROWEXA(L"_StringTable.size() must be squared number of 2");

			--_Bit;
			_WindowCount = (sizeof(uint64) * 8 + _BitCount - 1) / _BitCount;

			while (_PreventMinusNumber < 0xFF)
				_PreventMinusNumber += _StringTable.size();
		}
		u16string CUID64::Encode(uint64 UID_)
		{
			return _AddParity(_NumberToString(UID_));
		}
		uint64 CUID64::Decode(const u16string& UIDE_)
		{
			try
			{
				if (UIDE_.size() != _GetEncodedSize())
					THROWEX();

				return _StringToNumber(_SubParity(UIDE_));
			}
			catch (...)
			{
				return 0;
			}

		}
	}
}
