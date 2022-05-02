#include "TickMerge.h"

namespace rso
{
	namespace stock
	{
		optional<SChartTime> CTickMerge::Insert(const SChartTime& Data_)
		{
			auto CurSeconds = Data_.TimePoint.time_since_epoch().count() / 10000000;

			if (_LastSeconds > 0)
			{
				if (_LastSeconds == CurSeconds)
				{
					_CurData.TimePoint = Data_.TimePoint;
					_CurData.Price = Data_.Price;
					_CurData.Volume += Data_.Volume;
					return {};
				}
				else
				{
					_LastSeconds = CurSeconds;
					auto Ret = _CurData;
					_CurData = Data_;
					return Ret;
				}
			}
			else // 날짜 갱신
			{
				_LastSeconds = CurSeconds;
				_CurData = Data_;
				return {};
			}
		}
	}
}
