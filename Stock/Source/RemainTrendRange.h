#pragma once


#include "Base.h"
#include <Rso/Base/Queue.h>


namespace rso
{
	namespace stock
	{
		using namespace base;

		class CRemainTrendRange
		{
		public:
			struct SRemainTrend
			{
				TValue OfferTrend{ 0 };
				TValue BidTrend{ 0 };

				SRemainTrend(){}
				SRemainTrend(TValue OfferTrend_, TValue BidTrend_) :
					OfferTrend(OfferTrend_), BidTrend(BidTrend_)
				{}
			};

		private:
			struct _SData
			{
				time_t TimeT{ 0 };
				SRemain Row;

				_SData(time_t TimeT_, const SRemain& Row_) :
					TimeT(TimeT_), Row(Row_)
				{
				}
			};
			using _TDatas = CQueue<_SData>;

			_TDatas			_Datas;
			time_t			_Range{ 0 };

		public:
			CRemainTrendRange(time_t Range_);
			SRemainTrend	emplace(time_t Time_, const SRemain& Row_);
			void	clear(void);
		};
	}
}
