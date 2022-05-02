#pragma once

#include "Base.h"

namespace rso
{
	namespace stock
	{
		using namespace std;

		class CTradeIntensity
		{
			const double _ApplyRatio = 0.0;
			double _TradeIntensity = 0.0;
			double _VolumeSum = 0.0;
			TTime _LastTime;

		public:
			CTradeIntensity(double ApplyRatio_); // 1초 동안의 거래량을 전체에서 Ratio_ 만큼 적용
			double Tick(const TTime& Time_, double Volume_) noexcept;
		};
	}
}
