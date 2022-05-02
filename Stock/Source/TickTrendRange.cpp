#include "TickTrendRange.h"


namespace rso
{
	namespace stock
	{
		CTickTrendRange::CTickTrendRange(time_t Range_) :
			_Range(Range_)
		{
		}

		void	CTickTrendRange::emplace(time_t Time_, const SRealTick& Row_)
		{
			_Datas.emplace(_SData(Time_, Row_));

			while (_Datas.size() > 1)
			{
				auto& Data = _Datas.front();
				if (Time_ - Data.TimeT < _Range)
					break;

				_Datas.pop();
			}
		}

		void	CTickTrendRange::clear(void)
		{
			_Datas.clear();
		}
		CTickTrendRange::STickTrend	CTickTrendRange::get(void) const
		{
			return STickTrend(_Datas.back().Row.SellVolume - _Datas.front().Row.SellVolume, _Datas.back().Row.BuyVolume - _Datas.front().Row.BuyVolume);
		}
	}
}
