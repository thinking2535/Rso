#pragma once


#include "Stg.h"
#include <Rso/Log/Log.h>


namespace rso
{
	namespace stock
	{
		using namespace log;

		//class CStgMorning : public CStg<CNeurons>
		//{
		//	static const double c_EnterPriceGap;
		//	static const double c_LeavePriceGap;

		//	using _TNeuronCallback = function<void(size_t DataCnt_, const TOut& OutMin_, const TOut& OutMax_)>;
		//	using _TSMADynamic = CSMADynamic<SDataDb>;

		//	CLog _Log{ Target_File, EPeriod::EveryRun, L"StockLog/", L"Normal", ELocale::korean };
		//	_TSMADynamic _SMADynamicIn{ 3, 3, 3, 3, 2, 2, 1, 1 };
		//	_TSMADynamic _SMADynamicOut{ 1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5 };

		//	int _EnterDataCallback(const SEnterDataInfo& DataInfo_, const SDataDb& Data_) override;
		//	int _LeaveDataCallback(const SLeaveDataInfo& DataInfo_, const SDataDb& Data_) override;
		//	int _EnterCheckCallback(const SEnterDataInfo& DataInfo_, const TIn& In_, size_t DataCnt_, const TOut& OutMean_) override;
		//	int _LeaveCheckCallback(const SLeaveDataInfo& DataInfo_, const TIn& In_, size_t DataCnt_, const TOut& OutMean_) override;

		//	int _Remind(const SDataDb& Data_);
		//	void _Memory(const SDataDb& Data_);

		//public:
		//	CStgMorning(TValue TradeCntUnit_, time_t TradeStartDate_, TTraderCallback TraderCallback_, size_t SampleCnt_);
		//};
	}
}
