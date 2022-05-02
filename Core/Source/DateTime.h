#pragma once

#include "Core.h"
#include <chrono>
#include <sys/timeb.h>
#include <time.h>

namespace rso::core
{
	using namespace std;
	using namespace chrono;

	bool operator==(const tm& lhs_, const tm& rhs_);

	struct SDateTime
	{
		inline static const char* c_DefaultFormat = "%Y-%m-%d %H:%M:%S";
		inline static const wchar_t* c_DefaultFormatL = L"%Y-%m-%d %H:%M:%S";
		static constexpr uint32 c_FractionRatio = 1000000;
		static const int32 c_StreamSize = 16;
		static std::tm ToTM(const string& Time_, const char* pFormat_ = c_DefaultFormat);

		int16 year{ 1900 };
		uint16 month{ 1 };
		uint16 day{ 1 };
		uint16 hour{ 0 };
		uint16 minute{ 0 };
		uint16 second{ 0 };
		uint32 fraction{ 0 };

		SDateTime();
		SDateTime(
			int16 Year_,
			uint16 Month_,
			uint16 Day_,
			uint16 Hour_,
			uint16 Minute_,
			uint16 Second_,
			uint32 MilliSecond_ = 0);
		SDateTime(const std::tm& TM_, uint32 Fraction_ = 0);
		SDateTime(const string& Time_, const char* pFormat_ = c_DefaultFormat);
		bool operator==(const SDateTime& rTS_) const;
		bool operator!=(const SDateTime& rTS_) const;
		bool operator<(const SDateTime& TS_) const;
		bool operator>(const SDateTime& TS_) const;
		bool operator<=(const SDateTime& TS_) const;
		bool operator>=(const SDateTime& TS_) const;
		tm ToTM(void) const;
		bool IsSameDay(const SDateTime& TimeStamp_) const;
		SDateTime AddMonth(uint16 Month_) const;
		int32 GetMonthDiff(const SDateTime& Subtrahend_) const;
		uint16 GetFullWeek(void) const;
		string ToString(const char* pFormat_ = c_DefaultFormat) const;
		wstring ToWString(const wchar_t* pFormat_ = c_DefaultFormatL) const;
		virtual time_t ToTimeT(void) const;
		TTime ToTimePoint(void) const;
	};

	class CDateTime : public SDateTime
	{
	public:
		static SDateTime ToDateTime(const TTime& TimePoint_);
		static minutes GetMinuteOffset(bool Utc_ = false);

	private:
		minutes _MinuteOffset{};

	public:
		CDateTime(const SDateTime& DateTime_, const minutes& MinuteOffset_);
		CDateTime(const SDateTime& DateTime_, bool Utc_ = false);
		CDateTime(const TTime& TimePoint_, const minutes& MinuteOffset_);
		CDateTime(const TTime& TimePoint_, bool Utc_ = false);
		CDateTime(const string& Time_, const minutes& MinuteOffset_, const char* pFormat_ = c_DefaultFormat);
		CDateTime(const string& Time_, bool Utc_ = false, const char* pFormat_ = c_DefaultFormat);
		time_t ToTimeT(void) const override;
	};
}
