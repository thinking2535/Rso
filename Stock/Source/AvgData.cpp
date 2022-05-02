#include "AvgData.h"


namespace rso
{
	namespace stock
	{
		CAvgData::CAvgData(const TTime& StartTime_, const milliseconds& UnitDuration_) :
			_UnitDuration(UnitDuration_),
			_NextTime(StartTime_ + UnitDuration_)
		{
		}
		SPriceVolume CAvgData::current(void) const
		{
			return (_BufferDataCnt > 1 ? (_Buffer / _BufferDataCnt) : _Buffer);
		}
		void CAvgData::push(const TTime& Time_, const SPriceVolume& Data_)
		{
			if (_NextTime < Time_)
			{
				if (_BufferDataCnt > 1)
					_Buffer /= _BufferDataCnt;

				for (; _NextTime < Time_; _NextTime += _UnitDuration)
					emplace_back(_Buffer);

				_Buffer = Data_;
				_BufferDataCnt = 1;
			}
			else
			{
				_Buffer += Data_;
				++_BufferDataCnt;
			}
		}
		void CAvgData::reset(const TTime& StartTime_)
		{
			__super::clear();
			_NextTime = StartTime_ + _UnitDuration;
			new (&_Buffer) SPriceVolume();
			_BufferDataCnt = 0;
		}
	}
}
