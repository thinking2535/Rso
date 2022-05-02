#pragma once

#include <map>
#include <functional>
#include <vector>
#include <array>
#include "Protocol.h"

#pragma comment( lib, "Rso/Stock_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

namespace rso
{
	namespace stock
	{
		enum class EOrderType
		{
			시장가,
			지정가,
			시장가_IOC,
			시장가_FOK,
			지정가_IOC,
			지정가_FOK,
			Max,
			Null
		};

		const __int64 c_PointPrice = 2500;
		const int32 c_PriceGap = 5;
		const TTime c_ExtendedDate = CDateTime{ SDateTime{2016, 8, 1, 0, 0, 0} }.ToTimePoint();

		SDateTime LastCanTradeTime(const SDateTime& TimeStamp_);
		void operator += (SChart& Lhs_, const SChart& Rhs_);
		void operator -= (SChart& Lhs_, const SChart& Rhs_);
		TTime GetFutureEndTime(const TTime& StartTime_) noexcept;
		string PriceToPointString(int32 Price_);
		int32 PointStringToPrice(const string& Price_);
	}
}