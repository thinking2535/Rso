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
			CTradeIntensity(double ApplyRatio_); // 1�� ������ �ŷ����� ��ü���� Ratio_ ��ŭ ����
			double Tick(const TTime& Time_, double Volume_) noexcept;
		};
	}
}
