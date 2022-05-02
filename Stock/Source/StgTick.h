//#pragma once
//
//
//#include "Stg.h"
//#include <Rso/Log/Log.h>
//#include "StockExchange.h"
//#include "Statistics.h"
//#include "RangeWeightAvg.h"
//#include "RangeWeightSum.h"
//#include "RangeSum.h"
//#include "Trend.h"
//#include "HighPenetrate.h"
//#include "LowPenetrate.h"
//
//
//namespace rso
//{
//	namespace stock
//	{
//		using namespace log;
//
//		class CStgTick : public CStg<1,0>
//		{
//		private:
//			using TRangeWeightAvg = CRangeWeightAvg<milliseconds>;
//			using TRangeWeightSum = CRangeWeightSum<milliseconds>;
//			using TRangeSum = CRangeSum<milliseconds>;
//
//			static const time_t c_NeuronTimeGap = 100;
//			static const TSqlSmallInt c_TradeEndHour = 11;
//			static const TSqlSmallInt c_TradeEndMinute = 0;
//
//			CLog _Log{ Target_File, EPeriod::EveryRun, L"StockLog/", L"Normal", ELocale::Korean };
//			//CQueue<TValue> _OutDatas;
//			CStatistics _Statistics;
//
//			bool _InputCallback(const STick& Data_, const SDataInfo& DataInfo_, TIn& In_) override;
//			void _RemindCallback(const STick& Data_, const TRmns& Offers_, const TRmns& Bids_, const SDataInfo& DataInfo_, const TIn& In_, const TOut& OutMin_, const TOut& OutMax_) override;
//			bool _MemoryCallback(const STick& Data_, TOut& Out_) override;
//			void _Proc(void) override;
//
//			TRangeWeightSum _RangeSums0{ milliseconds(1000) };
//			TRangeWeightSum _RangeSums1{ milliseconds(10000) };
//			TRangeWeightSum _RangeSums2{ milliseconds(100000) };
//			TRangeWeightSum _RangeSums10{ milliseconds(3000) };
//			TRangeWeightSum _RangeSums11{ milliseconds(10000) };
//			TRangeWeightSum _RangeSums12{ milliseconds(100000) };
//			TRangeWeightSum _RangeSumsAll{ milliseconds(10000) };
//			CTrend<TValue> _Trend{ 30 };
//			CHighPenetrate _HighPenetrate;
//			CLowPenetrate _LowPenetrate;
//			TValue _TradeUnitCnt = 0;
//			TValue _ExitPrice = 0;
//			int _BuyCnt = 0;
//			int _SellCnt = 0;
//
//		public:
//			CStgTick(bool SimulateServer_, size_t SampleCnt_, const CDateTime& TradeStartTimeStamp_,
//				TOrderCallback OrderCallback_,
//				TChangeCallback ChangeCallback_,
//				TCancelCallback CancelCallback_,
//				TValue TradeUnitCnt_);
//			wstring StatisticsString(void) const;
//			void Trade(__int64 OrderNo_, TValue Price_, TValue Volume_);
//		};
//	}
//}
