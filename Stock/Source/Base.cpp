#include "Base.h"


namespace rso
{
	namespace stock
	{
		SDateTime LastCanTradeTime(const SDateTime& TimeStamp_)
		{
			if (TimeStamp_ < SDateTime(2016, 8, 1, 0, 0, 0))
				return SDateTime(1900, 1, 1, 15, 5, 0);
			else
				return SDateTime(1900, 1, 1, 15, 35, 0);
		}
		void operator += (SChart& Lhs_, const SChart& Rhs_)
		{
			Lhs_.Price += Rhs_.Price;
			Lhs_.Volume += Rhs_.Volume;
		}
		void operator -= (SChart& Lhs_, const SChart& Rhs_)
		{
			Lhs_.Price -= Rhs_.Price;
			Lhs_.Volume -= Rhs_.Volume;
		}
		TTime GetFutureEndTime(const TTime& StartTime_) noexcept
		{
			if (StartTime_ >= c_ExtendedDate)
				return StartTime_ + minutes(395);
			else
				return StartTime_ + minutes(365);

			//2016년 8월 1일 이전
			//15시 05분 종료
			//2016년 8월 1일 부터
			//15시 35분 종료
			//최근월물 15::20분 종료
		}
		string PriceToPointString(int32 Price_)
		{
			auto Price = to_string(Price_);
			if (Price.size() < 3)
				Price = string(3 - Price.size(), '0').append(Price);

			return Price.insert(Price.size() - 2, 1, '.');
		}
		int32 PointStringToPrice(const string& Price_)
		{
			try
			{
				if (Price_.size() > 3)
					return stoi(string(Price_).erase(Price_.size() - 3, 1));
				else
					return stoi(Price_);
			}
			catch (...)
			{
				return 0;
			}
		}
	}
}
