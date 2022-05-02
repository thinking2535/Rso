#include "NumberToKorean.h"


namespace rso
{
	namespace util
	{
		const wchar_t* CNumberToKorean::c_1{ L"ÀÏÀÌ»ï»ç¿ÀÀ°Ä¥ÆÈ±¸" };
		const wchar_t* CNumberToKorean::c_2{ L"½Ê¹éÃµ" };
		const wchar_t* CNumberToKorean::c_3{ L"¸¸¾ïÁ¶°æ" };

		wstring CNumberToKorean::Get(_TValue  Value_)
		{
			wstring Str;

			for (_TValue n3 = 0; Value_ > 0; ++n3)
			{
				if (n3 > 0 && Value_ % 10000)
				{
					Str += L" ";
					Str += c_3[n3 - 1];
				}

				for (_TValue n2 = 0; n2 < 4 && Value_ > 0; ++n2)
				{
					_TValue n1 = Value_ % 10;
					if (n1 != 0)
					{
						if (n2 > 0)
							Str += c_2[n2 - 1];

						Str += c_1[n1 - 1];
					}

					Value_ /= 10;
				}
			}

			std::reverse(std::begin(Str), std::end(Str));

			return Str;
		}
	}
}