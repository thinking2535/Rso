// 정해진 구간의 시작과 끝의 기울기를 추출

#pragma once


#include "Base.h"
#include <Rso/Base/Queue.h>


namespace rso
{
	namespace stock
	{
		using namespace base;

		class CTickTrendRange
		{
		public:
			struct STickTrend
			{
				TValue SellTrend{ 0 };
				TValue BuyTrend{ 0 };

				STickTrend(){}
				STickTrend(TValue SellTrend_, TValue BuyTrend_) :
					SellTrend(SellTrend_), BuyTrend(BuyTrend_)
				{}
			};

		private:
			struct _SData
			{
				time_t	TimeT{ 0 };
				SRealTick	Row;

				_SData(time_t TimeT_, const SRealTick& Row_) :
					TimeT(TimeT_), Row(Row_)
				{
				}
			};
			using _TDatas = CQueue<_SData>;

			_TDatas			_Datas;
			time_t			_Range{ 0 };

		public:
			CTickTrendRange(time_t Range_);
			void	emplace(time_t Time_, const SRealTick& Row_);
			void	clear(void);
			inline time_t	Range(void) const{ return _Range; }
			STickTrend	get(void) const;
		};
	}
}
