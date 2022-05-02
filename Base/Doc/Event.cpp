#include "Event.h"

namespace rso
{
	namespace base
	{
		CEvent::CEvent(EPeriod Period_, const CTime& Next_) :
			_Period(Period_),
			_Next(Next_)
		{
		}
		CEvent::CEvent(int Month_, int Day_, int Hour_, int Minute_, int Second_) :
			_Period(EPeriod::Year),
			_Next(nowtime, Month_, Day_, Hour_, Minute_, Second_)
		{
		}
		CEvent::CEvent(int Day_, int Hour_, int Minute_, int Second_) :
			_Period(EPeriod::Month),
			_Next(nowtime, Day_, Hour_, Minute_, Second_)
		{
		}
		CEvent::CEvent(EDayOfWeek DayOfWeek_, int Hour_, int Minute_, int Second_) :
			_Period(EPeriod::Week),
			_Next(nowtime, DayOfWeek_, Hour_, Minute_, Second_)
		{
		}
		CEvent::CEvent(int Hour_, int Minute_, int Second_) : // Month, ¶£¶£Week     ¶§ø‰¿œ ¶§, Hour, Minute, Second
			_Period(EPeriod::Day),                            //        ¶¢¶¶FullWeek ¶•     ¶¢
			_Next(nowtime, Hour_, Minute_, Second_)           //        ¶ßLastDay           ¶©
		{                                                     //        ¶¶Day               ¶•
		}
		CEvent::CEvent(int Minute_, int Second_) :
			_Period(EPeriod::Hour),
			_Next(nowtime, Minute_, Second_)
		{
		}
		CEvent::CEvent(int Second_) :
			_Period(EPeriod::Minute),
			_Next(nowtime, Second_)
		{
		}
		CEvent::CEvent() :
			_Period(EPeriod::Second),
			_Next(nowtime)
		{
		}
		const CTime& CEvent::GetNext(void) const
		{
			return _Next;
		}
		bool CEvent::CheckAndNextFixed(void)
		{
			if (CTime(nowtime) < _Next)
				return false;

			switch (_Period)
			{
			case EPeriod::Year:
				return _Next.SetNextYear();
			case EPeriod::Month:
				return _Next.SetNextMonth();
			case EPeriod::Week:
				_Next.AddDay(7);
				return true;
			case EPeriod::Day:
				_Next.AddDay(1);
				return true;
			case EPeriod::Hour:
				_Next.AddHour(1);
				return true;
			case EPeriod::Minute:
				_Next.AddMinute(1);
				return true;
			case EPeriod::Second:
				_Next.AddSecond(1);
				return true;
			default:
				return false;
			}
		}
	}
}
