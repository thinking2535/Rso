#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		class CAvgData : public TPrcVols
		{
			milliseconds _UnitDuration;
			TTime _NextTime;
			SPriceVolume _Buffer{};
			size_t _BufferDataCnt = 0;

		public:
			CAvgData(const TTime& StartTime_, const milliseconds& UnitDuration_);
			SPriceVolume current(void) const;
			void push(const TTime& Time_, const SPriceVolume& Data_);
			void reset(const TTime& StartTime_);
		};
	}
}
