#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		class CFutureCommission
		{
			double	_CommissionRatio = 0.0;
			TValue	_TradeValue = 0;

		public:
			CFutureCommission(double CommissionRatio_) :
				_CommissionRatio(CommissionRatio_)
			{
			}
			inline void	Trade(const SOrder& Order_)
			{
				_TradeValue += Order_.PositiveValue();
			}
			inline double	Commission(void) const
			{
				return (_TradeValue * _CommissionRatio);
			}
		};
	}
}
