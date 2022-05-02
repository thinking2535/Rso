// 전체 구간에서의 고가, 저가, 최대 상승폭, 하락폭 추출

#pragma once


#include "Candle.h"


namespace rso
{
	namespace stock
	{
		using namespace base;

		class CUpDown : public CCandle
		{
		private:
			TValue _MaxUp = 0;
			TValue _MaxDown = 0;

		public:
			void set(const SDataDb& Data_);
			TValue MaxUp(void) const
			{
				return _MaxUp;
			}
			TValue MaxDown(void) const
			{
				return _MaxDown;
			}
			TValue Trend(void) const;
		};
	}
}
