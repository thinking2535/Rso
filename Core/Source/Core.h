#pragma once
#pragma warning(disable:4996)

#include "Base.h"
#include <limits>
#include <array>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <memory>
#include <chrono>
#include <algorithm>
#include <codecvt>

namespace rso::json
{
	class Value;
}

namespace rso::core
{
	using namespace std;
	using namespace chrono;
	using namespace json;

	class CStream;
	struct SProto
	{
		virtual void operator<<(CStream&) = 0;
		virtual void operator>>(CStream&) const = 0;
		virtual void operator<<(const Value& Value_) = 0;
		virtual void operator>>(Value& Value_) const = 0;
		virtual ~SProto() {}
	};

	using TCoreCheckSum = uint64;
	using TTime = system_clock::time_point;
	using TSpan = milliseconds;
	using t_duration = TTime::duration;

	struct nowtime_t
	{
		nowtime_t() {}
	};
	const nowtime_t nowtime;

	enum EBool
	{
		Bool_Null = -1,
		Bool_False,
		Bool_True,
	};
	enum EDayOfWeek
	{
		Sun,
		Mon,
		Tue,
		Wed,
		Thr,
		Fri,
		Sat,
		Max,
		Null
	};

	enum class ELocale
	{
		Afrikaans,
		Afrikaans_SouthAfrica,
		Arabic,
		Arabic_UAE,
		Arabic_Bahrain,
		Arabic_Algeria,
		Arabic_Egypt,
		Arabic_Iraq,
		Arabic_Jordan,
		Arabic_Kuwait,
		Arabic_Lebanon,
		Arabic_Libya,
		Arabic_Morocco,
		Arabic_Oman,
		Arabic_Qatar,
		Arabic_SaudiArabia,
		Arabic_Syria,
		Arabic_Tunisia,
		Arabic_Yemen,
		Azeri_Latin,
		Azeri_Latin_Azerbaijan,
		Azeri_Cyrillic_Azerbaijan,
		Belarusian,
		Belarusian_Belarus,
		Bulgarian,
		Bulgarian_Bulgaria,
		Bosnian_BosniaAndHerzegovina,
		Catalan,
		Catalan_Spain,
		Czech,
		Czech_CzechRepublic,
		Welsh,
		Welsh_UnitedKingdom,
		Danish,
		Danish_Denmark,
		German,
		German_Austria,
		German_Switzerland,
		German_Germany,
		German_Liechtenstein,
		German_Luxembourg,
		Divehi,
		Divehi_Maldives,
		Greek,
		Greek_Greece,
		English,
		English_Australia,
		English_Belize,
		English_Canada,
		English_Caribbean,
		English_UnitedKingdom,
		English_Ireland,
		English_Jamaica,
		English_NewZealand,
		English_RepublicOfThePhilippines,
		English_TrinidadAndTobago,
		English_UnitedStates,
		English_SouthAfrica,
		English_Zimbabwe,
		Esperanto,
		Spanish,
		Spanish_Argentina,
		Spanish_Bolivia,
		Spanish_Chile,
		Spanish_Colombia,
		Spanish_CostaRica,
		Spanish_DominicanRepublic,
		Spanish_Ecuador,
		Spanish_Castilian,
		Spanish_Spain,
		Spanish_Guatemala,
		Spanish_Honduras,
		Spanish_Mexico,
		Spanish_Nicaragua,
		Spanish_Panama,
		Spanish_Peru,
		Spanish_PuertoRico,
		Spanish_Paraguay,
		Spanish_ElSalvador,
		Spanish_Uruguay,
		Spanish_Venezuela,
		Estonian,
		Estonian_Estonia,
		Basque,
		Basque_Spain,
		Farsi,
		Farsi_Iran,
		Finnish,
		Finnish_Finland,
		Faroese,
		Faroese_FaroeIslands,
		French,
		French_Belgium,
		French_Canada,
		French_Switzerland,
		French_France,
		French_Luxembourg,
		French_PrincipalityOfMonaco,
		Galician,
		Galician_Spain,
		Gujarati,
		Gujarati_India,
		Hebrew,
		Hebrew_Israel,
		Hindi,
		Hindi_India,
		Croatian,
		Croatian_BosniaAndHerzegovina,
		Croatian_Croatia,
		Hungarian,
		Hungarian_Hungary,
		Armenian,
		Armenian_Armenia,
		Indonesian,
		Indonesian_Indonesia,
		Icelandic,
		Icelandic_Iceland,
		Italian,
		Italian_Switzerland,
		Italian_Italy,
		Japanese,
		Japanese_Japan,
		Georgian,
		Georgian_Georgia,
		Kazakh,
		Kazakh_Kazakhstan,
		Kannada,
		Kannada_India,
		Korean,
		Korean_Korea,
		Konkani,
		Konkani_India,
		Kyrgyz,
		Kyrgyz_Kyrgyzstan,
		Lithuanian,
		Lithuanian_Lithuania,
		Latvian,
		Latvian_Latvia,
		Maori,
		Maori_NewZealand,
		FYRO_Macedonian,
		FYRO_Macedonian_FormerYugoslavRepublicOfMacedonia,
		Mongolian,
		Mongolian_Mongolia,
		Marathi,
		Marathi_India,
		Malay,
		Malay_BruneiDarussalam,
		Malay_Malaysia,
		Maltese,
		Maltese_Malta,
		Norwegian_Bokml,
		Norwegian_Bokml_Norway,
		Dutch,
		Dutch_Belgium,
		Dutch_Netherlands,
		Norwegian_Nynorsk_Norway,
		Northern_Sotho,
		Northern_Sotho_SouthAfrica,
		Punjabi,
		Punjabi_India,
		Polish,
		Polish_Poland,
		Pashto,
		Pashto_Afghanistan,
		Portuguese,
		Portuguese_Brazil,
		Portuguese_Portugal,
		Quechua,
		Quechua_Bolivia,
		Quechua_Ecuador,
		Quechua_Peru,
		Romanian,
		Romanian_Romania,
		Russian,
		Russian_Russia,
		Sanskrit,
		Sanskrit_India,
		Sami_Northern,
		Sami_Northern_Finland,
		Sami_Skolt_Finland,
		Sami_Inari_Finland,
		Sami_Northern_Norway,
		Sami_Lule_Norway,
		Sami_Southern_Norway,
		Sami_Northern_Sweden,
		Sami_Lule_Sweden,
		Sami_Southern_Sweden,
		Slovak,
		Slovak_Slovakia,
		Slovenian,
		Slovenian_Slovenia,
		Albanian,
		Albanian_Albania,
		Serbian_Latin_BosniaAndHerzegovina,
		Serbian_Cyrillic_BosniaAndHerzegovina,
		Serbian_Latin_SerbiaAndMontenegro,
		Serbian_Cyrillic_SerbiaAndMontenegro,
		Swedish,
		Swedish_Finland,
		Swedish_Sweden,
		Swahili,
		Swahili_Kenya,
		Syriac,
		Syriac_Syria,
		Tamil,
		Tamil_India,
		Telugu,
		Telugu_India,
		Thai,
		Thai_Thailand,
		Tagalog,
		Tagalog_Philippines,
		Tswana,
		Tswana_SouthAfrica,
		Turkish,
		Turkish_Turkey,
		Tatar,
		Tatar_Russia,
		Tsonga,
		Ukrainian,
		Ukrainian_Ukraine,
		Urdu,
		Urdu_IslamicRepublicOfPakistan,
		Uzbek_Latin,
		Uzbek_Latin_Uzbekistan,
		Uzbek_Cyrillic_Uzbekistan,
		Vietnamese,
		Vietnamese_VietNam,
		Xhosa,
		Xhosa_SouthAfrica,
		Chinese,
		Chinese_S,
		Chinese_HongKong,
		Chinese_Macau,
		Chinese_Singapore,
		Chinese_T,
		Zulu,
		Zulu_SouthAfrica,
		Max,
		Null
	};

	static const char* g_Locale[] =
	{
		"af",
		"af-ZA",
		"ar",
		"ar-AE",
		"ar-BH",
		"ar-DZ",
		"ar-EG",
		"ar-IQ",
		"ar-JO",
		"ar-KW",
		"ar-LB",
		"ar-LY",
		"ar-MA",
		"ar-OM",
		"ar-QA",
		"ar-SA",
		"ar-SY",
		"ar-TN",
		"ar-YE",
		"az",
		"az-AZ",
		"az-AZ",
		"be",
		"be-BY",
		"bg",
		"bg-BG",
		"bs-BA",
		"ca",
		"ca-ES",
		"cs",
		"cs-CZ",
		"cy",
		"cy-GB",
		"da",
		"da-DK",
		"de",
		"de-AT",
		"de-CH",
		"de-DE",
		"de-LI",
		"de-LU",
		"dv",
		"dv-MV",
		"el",
		"el-GR",
		"en",
		"en-AU",
		"en-BZ",
		"en-CA",
		"en-CB",
		"en-GB",
		"en-IE",
		"en-JM",
		"en-NZ",
		"en-PH",
		"en-TT",
		"en-US",
		"en-ZA",
		"en-ZW",
		"eo",
		"es",
		"es-AR",
		"es-BO",
		"es-CL",
		"es-CO",
		"es-CR",
		"es-DO",
		"es-EC",
		"es-ES",
		"es-ES",
		"es-GT",
		"es-HN",
		"es-MX",
		"es-NI",
		"es-PA",
		"es-PE",
		"es-PR",
		"es-PY",
		"es-SV",
		"es-UY",
		"es-VE",
		"et",
		"et-EE",
		"eu",
		"eu-ES",
		"fa",
		"fa-IR",
		"fi",
		"fi-FI",
		"fo",
		"fo-FO",
		"fr",
		"fr-BE",
		"fr-CA",
		"fr-CH",
		"fr-FR",
		"fr-LU",
		"fr-MC",
		"gl",
		"gl-ES",
		"gu",
		"gu-IN",
		"he",
		"he-IL",
		"hi",
		"hi-IN",
		"hr",
		"hr-BA",
		"hr-HR",
		"hu",
		"hu-HU",
		"hy",
		"hy-AM",
		"id",
		"id-ID",
		"is",
		"is-IS",
		"it",
		"it-CH",
		"it-IT",
		"ja",
		"ja-JP",
		"ka",
		"ka-GE",
		"kk",
		"kk-KZ",
		"kn",
		"kn-IN",
		"ko",
		"ko-KR",
		"kok",
		"kok-IN",
		"ky",
		"ky-KG",
		"lt",
		"lt-LT",
		"lv",
		"lv-LV",
		"mi",
		"mi-NZ",
		"mk",
		"mk-MK",
		"mn",
		"mn-MN",
		"mr",
		"mr-IN",
		"ms",
		"ms-BN",
		"ms-MY",
		"mt",
		"mt-MT",
		"nb",
		"nb-NO",
		"nl",
		"nl-BE",
		"nl-NL",
		"nn-NO",
		"ns",
		"ns-ZA",
		"pa",
		"pa-IN",
		"pl",
		"pl-PL",
		"ps",
		"ps-AR",
		"pt",
		"pt-BR",
		"pt-PT",
		"qu",
		"qu-BO",
		"qu-EC",
		"qu-PE",
		"ro",
		"ro-RO",
		"ru",
		"ru-RU",
		"sa",
		"sa-IN",
		"se",
		"se-FI",
		"se-FI",
		"se-FI",
		"se-NO",
		"se-NO",
		"se-NO",
		"se-SE",
		"se-SE",
		"se-SE",
		"sk",
		"sk-SK",
		"sl",
		"sl-SI",
		"sq",
		"sq-AL",
		"sr-BA",
		"sr-BA",
		"sr-SP",
		"sr-SP",
		"sv",
		"sv-FI",
		"sv-SE",
		"sw",
		"sw-KE",
		"syr",
		"syr-SY",
		"ta",
		"ta-IN",
		"te",
		"te-IN",
		"th",
		"th-TH",
		"tl",
		"tl-PH",
		"tn",
		"tn-ZA",
		"tr",
		"tr-TR",
		"tt",
		"tt-RU",
		"ts",
		"uk",
		"uk-UA",
		"ur",
		"ur-PK",
		"uz",
		"uz-UZ",
		"uz-UZ",
		"vi",
		"vi-VN",
		"xh",
		"xh-ZA",
		"zh",
		"zh-CN",
		"zh-HK",
		"zh-MO",
		"zh-SG",
		"zh-TW",
		"zu",
		"zu-ZA"
	};

	inline const char* GetLocaleString(ELocale Locale_)
	{
		return g_Locale[(int32)Locale_];
	}

	template <typename DataType, DataType Min_, DataType Max_, DataType Null_>
	class SValue
	{
	public:
		static const DataType Min = Min_;
		static const DataType Max = Max_;
		static const DataType Null = Null_;

		typedef DataType TType;

	private:
		DataType m_Value;

	public:
		SValue()
		{
			m_Value = Null_;
		}

		SValue(const DataType Value_)
		{
			m_Value = Value_;
		}

		~SValue()
		{
		}

		bool IsValid(void) const
		{
			if (m_Value == Null_ || m_Value < Min_ || m_Value > Max_)
				return false;

			return true;
		}
		bool IsValid(const DataType Value_) const
		{
			if (Value_ == Null_ || Value_ < Min_ || Value_ > Max_)
				return false;

			return true;
		}
		const DataType Get(void) const { return m_Value; }

		/////////////////////////////////////////////////////////////////////////////////
		const DataType operator+(const DataType Value_) const
		{
			if (IsValid(Value_) == false)
				return Null_;

			return (m_Value + Value_);
		}
		const DataType operator+(const SValue Value_) const
		{
			if (Value_.IsValid() == false)
				return Null_;

			return (*this + Value_.m_Value);
		}
		const DataType operator-(const DataType Value_) const
		{
			if (IsValid(Value_) == false)
				return Null_;

			return (m_Value - Value_);
		}
		const DataType operator-(const SValue Value_) const
		{
			if (Value_.IsValid() == false)
				return Null_;

			return (*this - Value_.m_Value);
		}
		const DataType operator*(const DataType Value_) const
		{
			if (IsValid(Value_) == false)
				return Null_;

			return (m_Value * Value_);
		}
		const DataType operator*(const SValue Value_) const
		{
			if (Value_.IsValid() == false)
				return Null_;

			return (*this * Value_.m_Value);
		}
		const DataType operator/(const DataType Value_) const
		{
			if (IsValid(Value_) == false)
				return Null_;

			return (m_Value / Value_);
		}
		const DataType operator/(const SValue Value_) const
		{
			if (Value_.IsValid() == false)
				return Null_;

			return (*this / Value_.m_Value);
		}

		/////////////////////////////////////////////////////////////////////////////////
		DataType& operator=(const DataType Value_)
		{
			if (Value_ < Min_)
				m_Value = Min_;
			else if (Value_ > Max_)
				m_Value = Max_;
			else
				m_Value = Value_;

			return m_Value;
		}

		/////////////////////////////////////////////////////////////////////////////////
		const DataType operator+=(const DataType Value_)
		{
			if (*this + Value_ > Max_)
				*this = Max_;
			else if (*this + Value_ < Min_)
				*this = Min_;
			else
				*this = *this + Value_;

			return m_Value;
		}
		const DataType operator+=(const SValue Value_)
		{
			return (*this += Value_.m_Value);
		}
		const DataType operator-=(const DataType Value_)
		{
			if (*this - Value_ > Max_)
				*this = Max_;
			else if (*this - Value_ < Min_)
				*this = Min_;
			else
				*this = *this - Value_;

			return m_Value;
		}
		const DataType operator-=(const SValue Value_)
		{
			return (*this -= Value_.m_Value);
		}
		const DataType operator*=(const DataType Value_)
		{
			if (*this * Value_ > Max_)
				*this = Max_;
			else if (*this * Value_ < Min_)
				*this = Min_;
			else
				*this = *this * Value_;

			return m_Value;
		}
		const DataType operator*=(const SValue Value_)
		{
			return (*this *= Value_.m_Value);
		}
		const DataType operator/=(const DataType Value_)
		{
			if (*this / Value_ > Max_)
				*this = Max_;
			else if (*this / Value_ < Min_)
				*this = Min_;
			else
				*this = *this / Value_;

			return m_Value;
		}
		const DataType operator/=(const SValue Value_)
		{
			return (*this /= Value_.m_Value);
		}

		/////////////////////////////////////////////////////////////////////////////////
		EBool operator<(const DataType Value_) const
		{
			if (IsValid(Value_) == false)
				return Bool_Null;

			return (m_Value < Value_ ? Bool_True : Bool_False);
		}
		EBool operator<(const SValue Value_) const
		{
			if (Value_.IsValid() == false)
				return Bool_Null;

			return (*this < Value_.m_Value);
		}
		EBool operator>(const DataType Value_) const
		{
			if (IsValid(Value_) == false)
				return Bool_Null;

			return (m_Value > Value_ ? Bool_True : Bool_False);
		}
		EBool operator>(const SValue Value_) const
		{
			if (Value_.IsValid() == false)
				return Bool_Null;

			return (*this > Value_.m_Value);
		}
		EBool operator<=(const DataType Value_) const
		{
			if (IsValid(Value_) == false)
				return Bool_Null;

			return (m_Value <= Value_ ? Bool_True : Bool_False);
		}
		EBool operator<=(const SValue Value_) const
		{
			if (Value_.IsValid() == false)
				return Bool_Null;

			return (*this <= Value_.m_Value);
		}
		EBool operator>=(const DataType Value_) const
		{
			if (IsValid(Value_) == false)
				return Bool_Null;

			return (m_Value >= Value_ ? Bool_True : Bool_False);
		}
		EBool operator>=(const SValue Value_) const
		{
			if (Value_.IsValid() == false)
				return Bool_Null;

			return (*this >= Value_.m_Value);
		}
		EBool operator==(const DataType Value_) const
		{
			if (IsValid(Value_) == false)
				return Bool_Null;

			return (m_Value == Value_ ? Bool_True : Bool_False);
		}
		EBool operator==(const SValue Value_) const
		{
			if (Value_.IsValid() == false)
				return Bool_Null;

			return (*this == Value_.m_Value);
		}
		EBool operator!=(const DataType Value_) const
		{
			if (IsValid(Value_) == false)
				return Bool_Null;

			return (m_Value != Value_ ? Bool_True : Bool_False);
		}
		EBool operator!=(const SValue Value_) const
		{
			if (Value_.IsValid() == false)
				return Bool_Null;

			return (*this != Value_.m_Value);
		}
	};

	system_clock::time_point steady_now(void);

	bool IsLeapYear(int16 Year_);
	uint16 LastDayOfMonth(int16 Year_, uint16 Month_);

	template <typename _t_to, typename _t_from>
	auto duration_cast_ceil(const _t_from& duration_)
	{
		if (_t_from::period::den > _t_to::period::den)
			if (duration_.count() % (_t_from::period::den / _t_to::period::den))
				return (duration_cast<_t_to>(duration_) + _t_to(duration_.count() >= 0 ? 1 : -1));
			else if (_t_from::period::num < _t_to::period::num)
				if (duration_.count() % (_t_to::period::num / _t_from::period::num))
					return (duration_cast<_t_to>(duration_) + _t_to(duration_.count() >= 0 ? 1 : -1));

		return duration_cast<_t_to>(duration_);
	}

	// StringFormat /////////////////////////////////////////////////////////////////////////////////////
	template <typename TType>
	inline basic_string<TType> StringFormat(const TType* Format_)
	{
		return basic_string<TType>(Format_);
	}
	template <typename TType, typename TArg, typename... TArgs>
	basic_string<TType> StringFormat(const TType* Format_, const TArg& Arg_, const TArgs &...Args_)
	{
		basic_stringstream<TType> ss;

		while (*Format_)
		{
			if (*Format_ == '%' && *++Format_ != '%')
			{
				ss << Arg_;
				ss << StringFormat(++Format_, Args_...);
				return ss.str();
			}
			ss << *Format_++;
		}

		return ss.str();
	}
	template <typename TType>
	inline TType StringFormat(const TType& Format_)
	{
		return Format_;
	}
	template <typename TType, typename TArg, typename... TArgs>
	inline TType StringFormat(const TType& Format_, const TArg& Arg_, const TArgs &...Args_)
	{
		return StringFormat(Format_.c_str(), Arg_, Args_...);
	}

	// StringFormatCS ///////////////////////////////////////////////////////////////////////////////////
	template <typename TType>
	inline void _StringFormatCS(vector<TType>& Strings_)
	{
	}
	template <typename TType, typename TArg, typename... TArgs>
	void _StringFormatCS(vector<TType>& Strings_, const TArg& Arg_, const TArgs &...Args_)
	{
		basic_stringstream<TType::value_type> ss;
		ss << Arg_;
		Strings_.emplace_back(ss.str());
		_StringFormatCS(Strings_, Args_...);
	}
	template <typename TType>
	basic_string<TType> _StringFormatCSStrings(const TType* Format_, const vector<basic_string<TType>>& Strings_)
	{
		basic_string<TType> Str;
		bool StrMode = true;
		bool ValidIndex = false;
		size_t Index = 0;
		for (; *Format_; ++Format_)
		{
			if (StrMode)
			{
				if (*Format_ == '}')
					throw exception("Composite Formatting : Invalid Close");

				if (*Format_ == '{')
					StrMode = false;
				else
					Str += *Format_;
			}
			else
			{
				if (*Format_ == '{')
					throw exception("Composite Formatting : Invalid Open");

				if (*Format_ == '}')
				{
					if (ValidIndex)
					{
						StrMode = true;
						Str += Strings_[Index];
						Index = 0;
						ValidIndex = false;
					}
					else
					{
						throw exception("Composite Formatting : No Index");
					}
				}
				else
				{
					auto Num = (int32)(*Format_) - (int32)'0';

					if (Num < 0 || Num > 9)
						throw exception("Composite Formatting : Invalid Index");

					ValidIndex = true;
					Index *= 10;
					Index += (size_t)Num;
				}
			}
		}

		return Str;
	}
	template <typename TType>
	inline TType StringFormatCS(const TType& Format_)
	{
		return StringFormatCS(Format_.c_str());
	}
	template <typename TType, typename TArg, typename... TArgs>
	inline TType StringFormatCS(const TType& Format_, const TArg& Arg_, const TArgs &...Args_)
	{
		return StringFormatCS(Format_.c_str(), Arg_, Args_...);
	}
	template <typename TType>
	inline basic_string<TType> StringFormatCS(const TType* Format_)
	{
		return _StringFormatCSStrings(Format_, vector<basic_string<TType>>());
	}
	template <typename TType, typename TArg, typename... TArgs>
	basic_string<TType> StringFormatCS(const TType* Format_, const TArg& Arg_, const TArgs &...Args_)
	{
		vector<basic_string<TType>> Strings;
		_StringFormatCS(Strings, Arg_, Args_...);
		return _StringFormatCSStrings(Format_, Strings);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	TCoreCheckSum GetCheckSum(const void* pData_, int64 TotSize_);
	TCoreCheckSum GetFileCheckSum(ifstream& File_);
	TCoreCheckSum GetFileCheckSum(const wstring& FileName_);

	template<typename TCharType>
	inline const TCharType* GetTrimLiteral()
	{
		return L" \t\n\r\v";
	}
	template<>
	inline const char* GetTrimLiteral()
	{
		return " \t\n\r\v";
	}

	template <typename TType>
	basic_string<TType> Trim(const TType* pStr_)
	{
		basic_string<TType> Ret(pStr_);
		Ret.erase(Ret.find_last_not_of(GetTrimLiteral<TType>()) + 1);
		return Ret.erase(0, Ret.find_first_not_of(GetTrimLiteral<TType>()));
	}
	template <typename TType>
	basic_string<TType> RTrim(const TType* pStr_)
	{
		basic_string<TType> Ret(pStr_);
		return Ret.erase(Ret.find_last_not_of(GetTrimLiteral<TType>()) + 1);
	}
	template <typename TType>
	basic_string<TType> LTrim(const TType* pStr_)
	{
		basic_string<TType> Ret(pStr_);
		return Ret.erase(0, Ret.find_first_not_of(GetTrimLiteral<TType>()));
	}

	template <typename TType>
	inline basic_string<TType> Trim(const basic_string<TType>& Str_)
	{
		return Trim(Str_.c_str());
	}
	template <typename TType>
	inline basic_string<TType> RTrim(const basic_string<TType>& Str_)
	{
		return RTrim(Str_.c_str());
	}
	template <typename TType>
	inline basic_string<TType> LTrim(const basic_string<TType>& Str_)
	{
		return LTrim(Str_.c_str());
	}

	template <typename TType>
	void ReplaceAll(TType& Source_, const TType& From_, const TType& To_)
	{
		static_assert(is_same<TType, string>::value || is_same<TType, wstring>::value, "ReplaceAll can use string, wstring");

		for (auto FoundIndex = Source_.find(From_);
			FoundIndex != TType::npos;
			FoundIndex = Source_.find(From_))
		{
			Source_.replace(FoundIndex, From_.size(), To_);
		}
	}
	template <typename TType>
	inline void EraseChar(basic_string<TType>& Str_, TType Char_)
	{
		Str_.erase(std::remove(Str_.begin(), Str_.end(), Char_), Str_.end());
	}
	// void ReplaceAll( string& Source_, char From_, const string& To_ );
	// void ReplaceAll( wstring& Source_, wchar_t From_, const wstring& To_ );

	template <typename TType>
	list<basic_string<TType>> Split(const basic_string<TType>& Str_, const TType* Delim_)
	{
		list<basic_string<TType>> Tokens;

		if (!Str_.empty())
		{
			size_t TokenLeftIndex = 0;

			while (true)
			{
				auto TokenRightIndex = Str_.find_first_of(Delim_, TokenLeftIndex);
				if (TokenRightIndex == basic_string<TType>::npos)
					TokenRightIndex = Str_.size();

				Tokens.emplace_back(Str_.substr(TokenLeftIndex, TokenRightIndex - TokenLeftIndex));
				if (TokenRightIndex == Str_.size())
					break;

				TokenLeftIndex = Str_.find_first_not_of(Delim_, TokenRightIndex);
				if (TokenLeftIndex == basic_string<TType>::npos)
					TokenLeftIndex = Str_.size();
			}
		}

		return Tokens;
	}
	template <typename TType>
	list<basic_string<TType>> Split(const basic_string<TType>& Str_, const basic_string<TType>& Delim_)
	{
		return Split(Str_, Delim_.c_str());
	}
	template <typename TType>
	list<basic_string<TType>> Split(const TType* Str_, const basic_string<TType>& Delim_)
	{
		return Split(basic_string<TType>(Str_), Delim_.c_str());
	}
	template <typename TType>
	list<basic_string<TType>> Split(const TType* Str_, const TType* Delim_)
	{
		return Split(basic_string<TType>(Str_), Delim_);
	}

	template <typename TType0, typename TType1>
	TType1 SplitBack(const TType0& Delim_, TType1& Str_o)
	{
		auto Index = Str_o.find_last_of(Delim_);
		if (Index == TType1::npos)
			return TType1(std::move(Str_o));

		auto Right = Str_o.substr(Index + 1);

		Index = Str_o.find_last_not_of(Delim_, Index);
		if (Index == TType1::npos)
			Str_o.clear();
		else
			Str_o = Str_o.substr(0, Index + 1);

		return Right;
	}

	template <typename TType>
	inline TType Abs(const TType& Value_)
	{
		return (Value_ < TType() ? -Value_ : Value_);
	}

	string WCSToMBS(const wstring& Src_, ELocale Locale_ = ELocale::Null);
	wstring MBSToWCS(const string& Src_, ELocale Locale_ = ELocale::Null);
	wstring GetSlashPath(const wstring& Path_);
	wstring GetBackSlashPath(const wstring& Path_);
	int32 GetWeekOfDay(const TTime& Time_);
	int32 GetWeek(const TTime& Time_);
	int32 GetFullWeek(const TTime& Time_);
	int64 GetTimePart(const TTime& Time_);
	int64 GetTimePart(int64 Hours_, int64 Minutes_, int64 Seconds_);

	struct SException : public exception
	{
		SException(const exception& Exception_) : exception(Exception_)
		{
		}
		SException(const string& Error_) : exception(Error_.c_str())
		{
		}
		SException(const wstring& Error_) : exception(WCSToMBS(Error_).c_str())
		{
		}
		SException(const char* Error_, int32 i_) : exception(Error_, i_)
		{
		}
	};

	template <typename TType>
	inline constexpr TType MinValue(void)
	{
		return (numeric_limits<TType>::min)();
	}
	template <typename TType>
	inline constexpr TType MaxValue(void)
	{
		return (numeric_limits<TType>::max)();
	}
	template <>
	inline constexpr float MinValue<float>(void)
	{
		return -(numeric_limits<float>::max)();
	}
	template <>
	inline constexpr double MinValue<double>(void)
	{
		return -(numeric_limits<double>::max)();
	}

	template <typename TValue>
	TValue ValueFromString(const string& Value_)
	{
		istringstream ss(Value_);
		TValue Value{};

		if (!(ss >> Value))
			throw exception("Invalid String");

		return Value;
	}
	template <typename TValue>
	TValue ValueFromString(const wstring& Value_)
	{
		wistringstream ss(Value_);
		TValue Value{};

		if (!(ss >> Value))
			throw exception("Invalid String");

		return Value;
	}

	template <typename T>
	struct is_std_array : public false_type
	{
	};

	template <typename T, size_t N>
	struct is_std_array<array<T, N>> : public true_type
	{
	};

	template <typename TContainer>
	struct is_set
	{
		static const bool value = is_same<TContainer::key_type, TContainer::value_type>::value;
	};
	template <typename TContainer>
	struct is_map
	{
		static const bool value = !is_same<TContainer::key_type, TContainer::value_type>::value;
	};

	// reverse iteratable /////////////////
	template <typename T>
	struct reversion_wrapper
	{
		T& iterable;
	};
	template <typename T>
	auto begin(reversion_wrapper<T> w)
	{
		return std::rbegin(w.iterable);
	}
	template <typename T>
	auto end(reversion_wrapper<T> w)
	{
		return std::rend(w.iterable);
	}
	template <typename T>
	reversion_wrapper<T> reverse(T&& iterable)
	{
		return { iterable };
	}


	///////////////////////////////////////
	// std::codecvt_utf8_utf16<char16_t> // converts between UTF-8 <-> UTF-16
	// std::codecvt_utf8<char32_t> // converts between UTF-8 <-> UTF-32
	// std::codecvt_utf8<char16_t> // converts between UTF-8 <-> UCS-2 (warning, not UTF-16! Don't bother using this one)

	inline std::u16string u8string_to_u16string(const std::string& Str_) // normal string, utf-8 string ok
	{
		return std::wstring_convert<std::codecvt_utf8_utf16<std::u16string::value_type>, std::u16string::value_type>{}.from_bytes(Str_);
	}
	inline std::string u16string_to_u8string(const std::u16string& Str_)
	{
		return std::wstring_convert<std::codecvt_utf8_utf16<std::u16string::value_type>, std::u16string::value_type>{}.to_bytes(Str_);
	}
	inline std::u32string u8string_to_u32string(const std::string& Str_) // normal string, utf-8 string ok
	{
		return std::wstring_convert<std::codecvt_utf8<std::u32string::value_type>, std::u32string::value_type>{}.from_bytes(Str_);
	}
	inline std::string u32string_to_u8string(const std::u32string& Str_)
	{
		return std::wstring_convert<std::codecvt_utf8<std::u32string::value_type>, std::u32string::value_type>{}.to_bytes(Str_);
	}
	inline std::wstring u16string_to_wstring(const std::u16string& Str_)
	{
		return std::wstring_convert< std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>, wchar_t>{}.from_bytes(
			reinterpret_cast<const char*> (&Str_[0]),
			reinterpret_cast<const char*> (&Str_[0] + Str_.size()));
	}
	inline std::u16string wstring_to_u16string(const std::wstring& Str_)
	{
		return std::wstring_convert< std::codecvt_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t>{}.from_bytes(
			reinterpret_cast<const char*> (&Str_[0]),
			reinterpret_cast<const char*> (&Str_[0] + Str_.size()));
	}
	//////////////////////////////////////

	template <bool>
	struct SPointerComparer
	{
	};
	template <>
	struct SPointerComparer<true>
	{
		template <typename TType>
		bool operator()(const TType& lhs_, const TType& rhs_) const
		{
			return (*lhs_ < *rhs_);
		}
	};
	template <>
	struct SPointerComparer<false>
	{
		template <typename TType>
		bool operator()(const TType& lhs_, const TType& rhs_) const
		{
			return (lhs_ < rhs_);
		}
	};
	template <typename TType>
	struct SComparer
	{
		bool operator()(const TType& lhs_, const TType& rhs_) const
		{
			return SPointerComparer<is_pointer<TType>::value>()(lhs_, rhs_);
		}
	};

	// Path 관련 함수는 \ 를 / 로 변환시키는 함수 이외에는 / 기반 path 라고 간주.
	template <typename TType>
	inline const typename TType::value_type* get_current_dir(void)
	{
		static_assert(false, "invalid string type");
	}
	template <>
	inline const string::value_type* get_current_dir<string>(void)
	{
		return ".";
	}
	template <>
	inline const wstring::value_type* get_current_dir<wstring>(void)
	{
		return L".";
	}

	template <typename TType>
	inline const typename TType::value_type* get_parent_dir(void)
	{
		static_assert(false, "invalid string type");
	}
	template <>
	inline const string::value_type* get_parent_dir<string>(void)
	{
		return "..";
	}
	template <>
	inline const wstring::value_type* get_parent_dir<wstring>(void)
	{
		return L"..";
	}

	template <typename TType>
	inline typename TType::value_type get_slash(void)
	{
		static_assert(false, "invalid string type");
	}
	template <>
	inline string::value_type get_slash<string>(void)
	{
		return '/';
	}
	template <>
	inline wstring::value_type get_slash<wstring>(void)
	{
		return L'/';
	}

	template <typename TType>
	inline typename TType::value_type get_inverse_slash(void)
	{
		static_assert(false, "invalid string type");
	}
	template <>
	inline string::value_type get_inverse_slash<string>(void)
	{
		return '\\';
	}
	template <>
	inline wstring::value_type get_inverse_slash<wstring>(void)
	{
		return L'\\';
	}

	template <typename TType>
	void make_slash_path(TType& path_)
	{
		std::replace(path_.begin(), path_.end(), get_inverse_slash<TType>(), get_slash<TType>());
	}

	template <typename TType>
	void make_std_path(TType& path_)
	{
		if (path_.empty())
			return;

		make_slash_path(path_);

		bool has_last_slash = (path_.back() == get_slash<TType>());

		vector<TType> tokens;
		size_t index = 0;
		for (auto i = path_.find_first_of(get_slash<TType>(), index);; i = path_.find_first_of(get_slash<TType>(), index))
		{
			auto dir = (i == TType::npos ? path_.substr(index) : path_.substr(index, i - index));
			if (dir == get_parent_dir<TType>())
			{
				if (tokens.empty())
				{
					path_.clear();
					return;
				}

				tokens.pop_back();
			}
			else if (dir == get_current_dir<TType>() || dir.empty())
			{
			}
			else
			{
				tokens.emplace_back(dir);
			}

			if (i == TType::npos)
				break;

			index = i + 1;
		}

		path_.clear();

		for (auto& i : tokens)
		{
			path_ += i;
			path_ += get_slash<TType>();
		}

		if (!has_last_slash) // 마지막 토큰이 파일명인지 아닌지는 dot(.) 로 구분할 수 있는것이 아니기 때문에 그냥 이렇게 처리
			path_.pop_back();
	}
	template <typename TType>
	TType to_std_path(const TType& path_)
	{
		auto path = path_;
		make_std_path(path);
		std::transform(path.begin(), path.end(), path.begin(), ::tolower);
		return path;
	}

	template <typename TType>
	int32 path_compare(const TType& path0_, const TType& path1_)
	{
		return to_std_path(path0_).compare(to_std_path(path1_));
	}
	template <typename TType>
	TType path_pop_right(TType& Path_)
	{
		if (Path_.empty())
			return TType();

		if (Path_.back() == get_slash<TType>())
			Path_.pop_back();

		auto Index = Path_.find_last_of(get_slash<TType>());
		if (Index == TType::npos)
		{
			TType Token = Path_;
			Path_.clear();
			return Token;
		}

		TType Token = Path_.substr(Index + 1);
		Path_.resize(Index + 1);
		return Token;
	}
	bool compare_files(const std::string& file0_, const std::string& file1_);

	struct SLittleEndian
	{
		static const bool value = (0x41424344UL == ('ABCD'));
	};
	template<int32 DataSize_>
	inline void ReverseBytes(char* /*pBytes_*/)
	{
	}
	template<>
	inline void ReverseBytes<2>(char* pBytes_)
	{
		swap(pBytes_[0], pBytes_[1]);
	}
	template<>
	inline void ReverseBytes<4>(char* pBytes_)
	{
		swap(pBytes_[0], pBytes_[3]);
		swap(pBytes_[1], pBytes_[2]);
	}
	template<>
	inline void ReverseBytes<8>(char* pBytes_)
	{
		swap(pBytes_[0], pBytes_[7]);
		swap(pBytes_[1], pBytes_[6]);
		swap(pBytes_[2], pBytes_[5]);
		swap(pBytes_[3], pBytes_[4]);
	}

	template<typename TType, bool LittleEndian_>
	struct SEndianness
	{
		static inline void Fix(char* /*pBytes_*/)
		{
		}
		static inline void Fix(char* /*pBytes_*/, int32 /*Length_*/)
		{
		}
	};
	template<typename TType>
	struct SEndianness<TType, false>
	{
		static inline void Fix(char* pBytes_)
		{
			ReverseBytes<sizeof(TType)>(pBytes_);
		}
		static void Fix(char* pBytes_, int32 Length_)
		{
			int32 Index = 0;
			ReverseBytes<sizeof(int32)>(&pBytes_[Index]);
			Index += sizeof(int32);

			for (size_t i = 0; i < Length_; ++i)
			{
				ReverseBytes<sizeof(TType)>(&pBytes_[Index]);
				Index += sizeof(TType);
			}
		}
		static void Fix(const basic_string<TType>& Value_, char* pBytes_)
		{
			int32 Index = 0;
			ReverseBytes<sizeof(int32)>(&pBytes_[Index]);
			Index += sizeof(int32);

			for (size_t i = 0; i < Value_.size(); ++i)
			{
				ReverseBytes<sizeof(TType)>(&pBytes_[Index]);
				Index += sizeof(TType);
			}
		}
	};
}

#define MAKE_WIDE_STRING(s) _MAKE_WIDE_STRING(s)
#define _MAKE_WIDE_STRING(s) L##s
#define GET_DEBUG_FORMAT(...) (StringFormat(L"File[%s] Line[%d] ", MAKE_WIDE_STRING(__FILE__), __LINE__) + StringFormat(__VA_ARGS__))
#define GET_DEBUG_FORMAT_MBS(...) (StringFormat("File[%s] Line[%d] ", __FILE__, __LINE__) + StringFormat(__VA_ARGS__))
#define THROWEX() throw SException(GET_DEBUG_FORMAT(L""))
#define THROWEX_MBS() throw SException(GET_DEBUG_FORMAT_MBS(""))
#define THROWEXA(...) throw SException(GET_DEBUG_FORMAT(__VA_ARGS__))
#define THROWEXA_MBS(...) throw SException(GET_DEBUG_FORMAT_MBS(__VA_ARGS__))

#ifdef _DEBUG
#define ASSERTION_DEBUG(Condition_) \
	if (!(Condition_))              \
	throw SException(GET_DEBUG_FORMAT(L""))
#define ASSERTION_DEBUG_MBS(Condition_) \
	if (!(Condition_))              \
	throw SException(GET_DEBUG_FORMAT_MBS(""))
#define ASSERTIONA_DEBUG(Condition_, ...) \
	if (!(Condition_))                    \
	throw SException(GET_DEBUG_FORMAT(__VA_ARGS__))
#define ASSERTIONA_DEBUG_MBS(Condition_, ...) \
	if (!(Condition_))                    \
	throw SException(GET_DEBUG_FORMAT_MBS(__VA_ARGS__))
#else
#define ASSERTION_DEBUG(Condition_) ((void)0)
#define ASSERTION_DEBUG_MBS(Condition_) ((void)0)
#define ASSERTIONA_DEBUG(Condition_, ...) ((void)0)
#define ASSERTIONA_DEBUG_MBS(Condition_, ...) ((void)0)
#endif

#define ASSERTION(Condition_) \
	if (!(Condition_))        \
	throw SException(GET_DEBUG_FORMAT(L""))
#define ASSERTIONA(Condition_, ...) \
	if (!(Condition_))              \
	throw SException(GET_DEBUG_FORMAT(__VA_ARGS__))
