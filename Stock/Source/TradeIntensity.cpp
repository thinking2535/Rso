#include "TradeIntensity.h"

namespace rso
{
	namespace stock
	{
		CTradeIntensity::CTradeIntensity(double ApplyRatio_) :
			_ApplyRatio(ApplyRatio_ <= 0.0 || ApplyRatio_ > 1.0 ? THROWEX() : ApplyRatio_)
		{

		}
		double CTradeIntensity::Tick(const TTime& Time_, double Volume_) noexcept
		{
			_VolumeSum += Volume_;
			const auto Elapsed = Time_ - _LastTime;

			if (Elapsed < t_duration(10000000))
				return _TradeIntensity;

			_TradeIntensity *= (1.0 - _ApplyRatio);
			_TradeIntensity += (_VolumeSum * (10000000.0 / double(Elapsed.count())) * _ApplyRatio);
			_VolumeSum = 0;
			_LastTime = Time_;
			return _TradeIntensity;
		}
	}
}
