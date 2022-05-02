#pragma once

#include "Base.h"

namespace rso
{
	namespace gameutil
	{
		class CPeriodCounter
		{
			t_duration _Period;
			TTime _LastTime;

		public:
			template<typename _TDuration>
			CPeriodCounter(const _TDuration& Period_, const TTime& LastTime_) :
				_Period(Period_), _LastTime(LastTime_)
			{
			}
			template<typename _TDuration>
			CPeriodCounter(const _TDuration& Period_) :
				_Period(Period_)
			{
			}
			inline TTime GetLastTime(void) const
			{
				return _LastTime;
			}
			inline void SetLastTime(const TTime& LastTime_)
			{
				_LastTime = LastTime_;
			}
			int32 Count(const TTime& Now_);
			bool Stop(void); // if full count
			bool Start(const TTime& Now_); // if use count
		};
	}
}