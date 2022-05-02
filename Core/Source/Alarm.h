#pragma once

#include "DateTime.h"

namespace rso
{
	namespace core
	{
		//class CAlarm abstract
		//{
		//protected:
		//	TTime _NextTime;

		//public:
		//	using TValue = uint16;

		//	CAlarm()
		//	{}
		//	CAlarm(const TTime& NextTime_) :
		//		_NextTime(NextTime_)
		//	{}
		//	inline TTime GetNextTime(void) const
		//	{
		//		return _NextTime;
		//	}
		//	inline void SetNextTime(const TTime& NextTime_)
		//	{
		//		_NextTime = NextTime_;
		//	}
		//	bool Check(void) const
		//	{
		//		auto Now = system_clock::now();
		//		if (Now < _NextTime)
		//			return false;

		//		return true;
		//	}
		//	virtual int64 Proc(void) = 0;
		//};
		//template<typename TDuration>
		//class CAlarmDuration : public CAlarm // _NextTime 세팅 기준이 현재 시각
		//{
		//	TDuration _Duration;

		//public:
		//	CAlarmDuration(const TDuration& Duration_) :
		//		CAlarm(system_clock::now() + Duration_), _Duration(Duration_)
		//	{
		//	}
		//	CAlarmDuration(const TTime& NextTime_, const TDuration& Duration_) :
		//		CAlarm(NextTime_), _Duration(Duration_)
		//	{
		//	}
		//	void Reset(void)
		//	{
		//		new ((CAlarm*)this) CAlarm(system_clock::now() + _Duration);
		//	}
		//	int64 Proc(void) override
		//	{
		//		auto Now = system_clock::now();
		//		if (Now < _NextTime)
		//			return 0;

		//		int64 PassedCnt = (duration_cast<TDuration>(Now - _NextTime).count() / _Duration.count() + 1);
		//		_NextTime = Now + _Duration;

		//		return PassedCnt;
		//	}
		//};
		//template<typename TDuration>
		//class CAlarmPeriod : public CAlarm // _NextTime 세팅 기준이 _NextTime
		//{
		//	TDuration _Duration;

		//public:
		//	CAlarmPeriod(const TDuration& Duration_) :
		//		CAlarm(system_clock::now() + Duration_), _Duration(Duration_)
		//	{
		//	}
		//	CAlarmPeriod(const TTime& NextTime_, const TDuration& Duration_) :
		//		CAlarm(NextTime_), _Duration(Duration_)
		//	{
		//	}
		//	int64 Proc(void) override
		//	{
		//		auto Now = system_clock::now();
		//		if (Now < _NextTime)
		//			return 0;

		//		int64 PassedCnt = (duration_cast<TDuration>(Now - _NextTime).count() / _Duration.count() + 1);
		//		_NextTime += (_Duration * PassedCnt);

		//		return PassedCnt;
		//	}
		//};
		//class CAlarmDay : public CAlarm
		//{
		//	hours _Duration;

		//public:
		//	CAlarmDay(uint16 Hour_, uint16 Minute_, uint16 Second_, uint16 DelayDay_) :
		//		_Duration(24 * DelayDay_)
		//	{
		//		auto Now = system_clock::now();

		//		SDateTime NextTime(Now);
		//		NextTime.hour = Hour_;
		//		NextTime.minute = Minute_;
		//		NextTime.second = Second_;
		//		NextTime.fraction = 0;
		//		_NextTime = NextTime.ToTimePoint();

		//		if (_NextTime < Now)
		//			_NextTime += _Duration;
		//	}
		//	CAlarmDay(const TTime& NextTime_, uint16 DelayDay_) :
		//		CAlarm(NextTime_), _Duration(24 * DelayDay_)
		//	{
		//	}
		//	int64 Proc(void) override
		//	{
		//		auto Now = system_clock::now();
		//		if (Now < _NextTime)
		//			return 0;

		//		int64 PassedCnt = (duration_cast<hours>(Now - _NextTime).count() / _Duration.count() + 1);
		//		_NextTime += (_Duration * PassedCnt);

		//		return PassedCnt;
		//	}
		//};
		//class CAlarmDayOfWeek : public CAlarm
		//{
		//	hours _Duration;

		//public:
		//	CAlarmDayOfWeek(uint16 Hour_, uint16 Minute_, uint16 Second_, EDayOfWeek DayOfWeek_, uint16 DelayWeek_) :
		//		_Duration(24 * 7 * DelayWeek_)
		//	{
		//		auto Now = system_clock::now();
		//		auto TimeT = system_clock::to_time_t(Now);
		//		tm Tm;
		//		localtime_s(&Tm, &TimeT);

		//		SDateTime NextTime(Now + hours((DayOfWeek_ - Tm.tm_wday) * 24));
		//		NextTime.hour = Hour_;
		//		NextTime.minute = Minute_;
		//		NextTime.second = Second_;
		//		NextTime.fraction = 0;
		//		_NextTime = NextTime.ToTimePoint();

		//		if (_NextTime <= Now)
		//			_NextTime += _Duration;
		//	}
		//	CAlarmDayOfWeek(const TTime& NextTime_, uint16 DelayWeek_) :
		//		CAlarm(NextTime_), _Duration(24 * 7 * DelayWeek_)
		//	{
		//	}
		//	int64 Proc(void) override
		//	{
		//		auto Now = system_clock::now();
		//		if (Now < _NextTime)
		//			return 0;

		//		int64 PassedCnt = (duration_cast<hours>(Now - _NextTime).count() / _Duration.count() + 1);
		//		_NextTime += (_Duration * PassedCnt);

		//		return PassedCnt;
		//	}
		//};
		//class CAlarmMonth : public CAlarm
		//{
		//	static const uint16 c_CheckMonths = 96;
		//	uint16 _DelayMonth{};

		//	int64 _SetNextMonth(const TTime& Now_, const SDateTime& NextTime_, uint16 AddedMonth_)
		//	{
		//		uint16 NeedToAddMonth = ((AddedMonth_ + 1) / _DelayMonth) * _DelayMonth;
		//		for (uint16 SkippedMonth = 0; SkippedMonth < c_CheckMonths; SkippedMonth += _DelayMonth) // 96 = 8 * 12 ( 8 년 동안의 개월수 ), 8년동안의 윤년으로 인해 해당 월의 해당일이 없는 경우가 있을 것이라 가정하고 반복
		//		{
		//			_NextTime = NextTime_.AddMonth(NeedToAddMonth + SkippedMonth).ToTimePoint();
		//			if (_NextTime > Now_)
		//				return NeedToAddMonth + SkippedMonth;
		//		}

		//		return NeedToAddMonth + c_CheckMonths;
		//	}

		//public:
		//	CAlarmMonth(uint16 Day_, uint16 Hour_, uint16 Minute_, uint16 Second_, uint16 DelayMonth_) :
		//		_DelayMonth(DelayMonth_)
		//	{
		//		auto Now = system_clock::now();

		//		SDateTime NextTime(Now);
		//		NextTime.day = Day_;
		//		NextTime.hour = Hour_;
		//		NextTime.minute = Minute_;
		//		NextTime.second = Second_;
		//		NextTime.fraction = 0;
		//		_NextTime = NextTime.ToTimePoint();

		//		if (_NextTime < Now)
		//			_SetNextMonth(Now, NextTime, 0);
		//	}
		//	CAlarmMonth(const TTime& NextTime_, uint16 DelayMonth_) :
		//		CAlarm(NextTime_), _DelayMonth(DelayMonth_)
		//	{
		//	}
		//	int64 Proc(void) override
		//	{
		//		auto Now = system_clock::now();
		//		if (Now < _NextTime)
		//			return 0;

		//		SDateTime NowStamp(Now);
		//		SDateTime NextTime(_NextTime);

		//		auto MonthDiff = NowStamp.GetMonthDiff(NextTime);
		//		if (MonthDiff < 0)
		//			return 0;

		//		return _SetNextMonth(Now, NextTime, static_cast<uint16>(MonthDiff));
		//	}
		//};
	}
}