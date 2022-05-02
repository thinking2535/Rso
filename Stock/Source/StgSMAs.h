#pragma once


#include "Stg.h"
#include "SMARange.h"
#include <Rso/Log/Log.h>


namespace rso
{
	namespace stock
	{
		using namespace log;

//		class CStgSMAs : public CStg<42,12>
//		{
//		public:
//			using TSMAs = array<double, 4>;
//
//		private:
//			// delete me
//			//static const size_t c_DayNeuronInCnt = 10;
//			//static const size_t c_DayNeuronOutCnt = 2;
//			const double c_EnterPriceGap = 12.5;
//			const double c_LossStopGap = 10.0;
//			static const time_t c_NeuronTimeGap = 60;
//// delete me			static const time_t c_DayNeuronTimeGap = 1;
//
//			class _CSMARange : public CSMARange
//			{
//				using _TGaps = vector<size_t>;
//				_TGaps _Gaps;
//
//			public:
//				_CSMARange(size_t SMACount_) :
//					CSMARange(SMACount_, SMACount_ * 2)
//				{
//					const size_t c_SMAIndexCnt = 10;
//					for (size_t i = 0; i < c_SMAIndexCnt; ++i)
//						_Gaps.emplace_back(SMACount_ * i / c_SMAIndexCnt);
//				}
//				double operator[](size_t Index_) const
//				{
//					return CSMARange::operator[](_Gaps[Index_]);
//				}
//				size_t size(void) const
//				{
//					return _Gaps.size();
//				}
//				double current_value(void) const
//				{
//					return CSMARange::operator[](0);
//				}
//			};
//
//			using _TDatas = vector<_CSMARange>;
//			// delete me
//			//using _TDayNeuron = CNeuron<c_DayNeuronInCnt, c_DayNeuronOutCnt>;
//			//using _TDayNeuronIn = _TDayNeuron::TIn;
//			//using _TDayNeuronOut = _TDayNeuron::TOut;
//
//			CLog _Log{ Target_File, EPeriod::EveryRun, L"StockLog/", L"Normal", ELocale::korean };
//			_TDatas _Datas{
////				_CSMARange(360),
//				_CSMARange(120),
//				_CSMARange(60),
//				_CSMARange(20),
//				_CSMARange(5)
//			};
//			CSMADynamic<TValue> _OutDatas{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
//			CTrend _Trend;
//// delete me			time_t _DayNeuronTime = 0;	// 단위시간(분) 만큼 매번 증가
//			time_t _TimeValue = 1; // 전일 마지막 거래시각이 0 (_DayNeuronTime와 다르게 매일 초기화)
//// delete me			_TDayNeuron _DayNeuron;
//			// delete me
//			//bool _DayNeuronOutReady = false;
//			//_TDayNeuronOut _DayNeuronOut;
//
//// delete me			int _DayNeuronCallback(const _TDayNeuronIn& In_, const _TDayNeuronOut& OutMean_);
//			bool _InputCallback(const SDataDb& Data_, const SDataInfo& DataInfo_, TIn& In_) override;
//			int _RemindCallback(const SDataDb& Data_, const SDataInfo& DataInfo_, const TIn& In_, const TOut& OutMin_, const TOut& OutMax_) override;
//			bool _MemoryCallback(const SDataDb& Data_, TOut& Out_) override;
//
//		public:
//			CStgSMAs(TValue TradeCntUnit_, time_t TradeStartDate_, TTraderCallback TraderCallback_, size_t SampleCnt_);
//			TSMAs GetSMAs(void) const
//			{
//				return TSMAs{
//					_Datas[0].current_value(),
//					_Datas[1].current_value(),
//					_Datas[2].current_value(),
//					_Datas[3].current_value() };
//			}
//		};
	}
}
