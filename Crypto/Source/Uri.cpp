#include "Uri.h"


namespace rso
{
	namespace crypto
	{
		bool CUri::IsSafe(const char Char_)
		{
			if (Char_ >= 'a'  &&  Char_ <= 'z' ||
				Char_ >= 'A'  &&  Char_ <= 'Z' ||
				Char_ >= '0'  &&  Char_ <= '9' ||
				Char_ == '.' ||
				Char_ == '-' ||
				Char_ == '*' ||
				Char_ == '_')
				return true;

			return false;
		}

		char CUri::Hex2Dec(const char Char_)
		{
			if (Char_ >= '0'  &&  Char_ <= '9')
				return (Char_ - '0');

			if (Char_ >= 'A'  &&  Char_ <= 'Z')
				return (Char_ - 'A' + 10);

			if (Char_ >= 'a'  &&  Char_ <= 'z')
				return (Char_ - 'a' + 10);

			return -1;
		}

		string CUri::Encode(const string& Str_)
		{
			const char c_Dec2Hex[] = "0123456789ABCDEF";

			string Result;
			Result.reserve(Str_.size() * 3);

			for (auto& c : Str_)
			{
				if (IsSafe(c))
				{
					Result += c;
				}
				else
				{
					Result += '%';
					Result += c_Dec2Hex[(c >> 4) & 0xF];
					Result += c_Dec2Hex[c & 0x0F];
				}
			}

			return Result;
		}

		string CUri::Decode(const string& Str_)
		{
			bool IsDecoding = false;
			string Result;
			Result.reserve(Str_.size());

			for (auto it = Str_.begin(); it != Str_.end(); ++it)
			{
				if (IsDecoding)
				{
					if ((Str_.end() - it) < 2)
						break;

					char dec1, dec2;
					if (-1 != (dec1 = Hex2Dec(*it)) &&
						-1 != (dec2 = Hex2Dec(*(++it))))
					{
						Result += ((dec1 << 4) + dec2);
					}

					IsDecoding = false;
				}
				else
				{
					if (*it == '%')
					{
						IsDecoding = true;
						continue;
					}

					Result += *it;
				}
			}

			return Result;
		}
	}
}
