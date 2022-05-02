#include "PeriodCounter.h"

namespace rso
{
	namespace gameutil
	{
		int32 CPeriodCounter::Count(const TTime& Now_)
		{
			if (_LastTime == TTime())
				return 0;

			auto Cnt = (Now_ - _LastTime).count() / _Period.count();
			_LastTime += (_Period * Cnt);

			return int32(Cnt);
		}
		bool CPeriodCounter::Stop(void)
		{
			if (_LastTime == TTime())
				return false;

			_LastTime = TTime();

			return true;
		}
		bool CPeriodCounter::Start(const TTime& Now_)
		{
			if (_LastTime != TTime())
				return false;

			_LastTime = Now_;

			return true;
		}
	}
}