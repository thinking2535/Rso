#include "RemainTrendRange.h"


namespace rso
{
	namespace stock
	{
		CRemainTrendRange::CRemainTrendRange(time_t Range_) :
			_Range(Range_)
		{
		}
		CRemainTrendRange::SRemainTrend	CRemainTrendRange::emplace(time_t Time_, const SRemain& Row_)
		{
			_Datas.emplace(_SData(Time_, Row_));

			while (_Datas.size() > 1)
			{
				auto& Data = _Datas.front();
				if (Time_ - Data.TimeT < _Range)
					break;

				_Datas.pop();
			}

			if (_Datas.size() == 0)
				return SRemainTrend();

			auto Front = _Datas.front();

			return SRemainTrend(Row_.TotalOfferRem - Front.Row.TotalOfferRem, Row_.TotalBidRem - Front.Row.TotalBidRem);
		}
		void	CRemainTrendRange::clear(void)
		{
			_Datas.clear();
		}
	}
}
