#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		struct CStatistics
		{
			TValue _WinCnt = 0;
			TValue _LoseCnt = 0;
			TValue _InitAsset = 0;
			TValue _LastAsset = 0;
			double _CommissionRatio = 0.00006;

		public:
			TValue SetAsset(TValue Asset_);
			TValue GetAsset(void) const;
			TValue TradeCnt(void) const;
			double WinRatio(void) const;
			double Commission(void) const;
			double Gain(void) const;
			wstring ToString(void) const;
		};
	}
}
