#include "DateTime.h"
#include <iomanip>

namespace rso::core
{
	bool operator==(const tm& lhs_, const tm& rhs_)
	{
		return (
			lhs_.tm_sec == rhs_.tm_sec &&
			lhs_.tm_min == rhs_.tm_min &&
			lhs_.tm_hour == rhs_.tm_hour &&
			lhs_.tm_mday == rhs_.tm_mday &&
			lhs_.tm_mon == rhs_.tm_mon &&
			lhs_.tm_year == rhs_.tm_year); // tm_wday is not filled with get_time, so compare without tm_wday
	}

	std::tm SDateTime::ToTM(const string& Time_, const char* pFormat_)
	{
		std::tm TM = {};
		std::stringstream(Time_) >> std::get_time(&TM, pFormat_);

		// Check Time_
		auto Ori = TM;
		_mkgmtime(&TM); // month, day will modify when call _mkgmtime
		ASSERTIONA(TM == Ori, L"Invalid DateTime [%s]", MBSToWCS(Time_).c_str());
		///////////////

		return TM;
	}
	SDateTime::SDateTime()
	{
	}
	SDateTime::SDateTime(
		int16 Year_,
		uint16 Month_,
		uint16 Day_,
		uint16 Hour_,
		uint16 Minute_,
		uint16 Second_,
		uint32 MilliSecond_) : year(Year_),
		month(Month_),
		day(Day_),
		hour(Hour_),
		minute(Minute_),
		second(Second_),
		fraction(MilliSecond_* c_FractionRatio)
	{
	}
	SDateTime::SDateTime(const std::tm& TM_, uint32 Fraction_) : year((int16)(TM_.tm_year + 1900)),
		month((uint16)(TM_.tm_mon + 1)),
		day((uint16)TM_.tm_mday),
		hour((uint16)TM_.tm_hour),
		minute((uint16)TM_.tm_min),
		second((uint16)TM_.tm_sec),
		fraction(Fraction_)
	{
	}
	SDateTime::SDateTime(const string& Time_, const char* pFormat_) : SDateTime(SDateTime::ToTM(Time_, pFormat_))
	{
	}
	bool SDateTime::operator==(const SDateTime& rTS_) const
	{
		return (year == rTS_.year &&
			month == rTS_.month &&
			day == rTS_.day &&
			hour == rTS_.hour &&
			minute == rTS_.minute &&
			second == rTS_.second &&
			fraction == rTS_.fraction);
	}
	bool SDateTime::operator!=(const SDateTime& rTS_) const
	{
		return !(*this == rTS_);
	}
	bool SDateTime::operator<(const SDateTime& TS_) const
	{
		if (year < TS_.year)
			return true;
		if (year > TS_.year)
			return false;
		if (month < TS_.month)
			return true;
		if (month > TS_.month)
			return false;
		if (day < TS_.day)
			return true;
		if (day > TS_.day)
			return false;
		if (hour < TS_.hour)
			return true;
		if (hour > TS_.hour)
			return false;
		if (minute < TS_.minute)
			return true;
		if (minute > TS_.minute)
			return false;
		if (second < TS_.second)
			return true;
		if (second > TS_.second)
			return false;
		if (fraction < TS_.fraction)
			return true;

		return false;
	}
	bool SDateTime::operator>(const SDateTime& TS_) const
	{
		return (TS_ < *this);
	}
	bool SDateTime::operator<=(const SDateTime& TS_) const
	{
		return !(*this > TS_);
	}
	bool SDateTime::operator>=(const SDateTime& TS_) const
	{
		return !(*this < TS_);
	}
	tm SDateTime::ToTM(void) const
	{
		return tm{
			second,
			minute,
			hour,
			day,
			month - 1,
			year - 1900 };
	}
	bool SDateTime::IsSameDay(const SDateTime& TimeStamp_) const
	{
		if (year == TimeStamp_.year &&
			month == TimeStamp_.month &&
			day == TimeStamp_.day)
			return true;

		return false;
	}
	SDateTime SDateTime::AddMonth(uint16 Month_) const
	{
		SDateTime Time = *this;
		Time.year += ((Time.month - 1 + Month_) / 12);
		Time.month = (Time.month - 1 + Month_) % 12 + 1;

		auto NextLastDayOfMonth = LastDayOfMonth(Time.year, Time.month);
		if (Time.day > NextLastDayOfMonth)
			Time.day = NextLastDayOfMonth;

		return Time;
	}
	int32 SDateTime::GetMonthDiff(const SDateTime& Subtrahend_) const
	{
		int32 Result = (year > Subtrahend_.year ? (year - Subtrahend_.year) : -(int32)(Subtrahend_.year - year)) * 12;
		Result += (month > Subtrahend_.month ? (month - Subtrahend_.month) : -(int32)(Subtrahend_.month - month));
		return Result;
	}
	uint16 SDateTime::GetFullWeek(void) const
	{
		return (day - 1) / 7;
	}
	time_t SDateTime::ToTimeT(void) const
	{
		auto TM(ToTM());
		return _mkgmtime(&TM);
	}
	TTime SDateTime::ToTimePoint(void) const
	{
		return system_clock::from_time_t(ToTimeT()) + milliseconds(fraction / c_FractionRatio);
	}
	string SDateTime::ToString(const char* pFormat_) const
	{
		tm TM(ToTM());

		char Str[1024 + 1] = {};
		if (strftime(Str, 1024, pFormat_, &TM) == 0)
			return string();

		return string(Str);
	}
	wstring SDateTime::ToWString(const wchar_t* pFormat_) const
	{
		tm TM(ToTM());

		wchar_t Str[1024 + 1] = {};
		if (wcsftime(Str, 1024, pFormat_, &TM) == 0)
			return wstring();

		return wstring(Str);
	}

	SDateTime CDateTime::ToDateTime(const TTime& TimePoint_)
	{
		auto TimeT = system_clock::to_time_t(TimePoint_);
		tm Tm;
		gmtime_s(&Tm, &TimeT);
		return SDateTime(Tm, duration_cast<milliseconds>(TimePoint_.time_since_epoch()).count() % 1000 * c_FractionRatio);
	}
	minutes CDateTime::GetMinuteOffset(bool Utc_)
	{
		if (Utc_)
			return minutes();

		long Offset = 0;
		_get_timezone(&Offset); // time

		//int DayLight = 0;
		//ret = _get_daylight(&DayLight); // apply when 1

		long DayLightSavingBias = 0;
		_get_dstbias(&DayLightSavingBias); // get seconds ( korea is -3600 )

		return minutes((Offset - DayLightSavingBias) / 60);
	}
	CDateTime::CDateTime(const SDateTime& DateTime_, const minutes& MinuteOffset_) : SDateTime(DateTime_), _MinuteOffset(MinuteOffset_)
	{
	}
	CDateTime::CDateTime(const SDateTime& DateTime_, bool Utc_) : CDateTime(DateTime_, GetMinuteOffset(Utc_))
	{
	}
	CDateTime::CDateTime(const TTime& TimePoint_, const minutes& MinuteOffset_) : CDateTime(ToDateTime(TimePoint_ + MinuteOffset_), MinuteOffset_)
	{
	}
	CDateTime::CDateTime(const TTime& TimePoint_, bool Utc_) : CDateTime(TimePoint_, GetMinuteOffset(Utc_))
	{
	}
	CDateTime::CDateTime(const string& Time_, const minutes& MinuteOffset_, const char* pFormat_) : SDateTime(Time_, pFormat_), _MinuteOffset(MinuteOffset_)
	{
	}
	CDateTime::CDateTime(const string& Time_, bool Utc_, const char* pFormat_) : CDateTime(Time_, GetMinuteOffset(Utc_), pFormat_)
	{
	}
	time_t CDateTime::ToTimeT(void) const
	{
		return SDateTime::ToTimeT() - (_MinuteOffset.count() * 60);
	}
}
