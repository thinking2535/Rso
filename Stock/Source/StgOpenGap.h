#pragma once


#include "Stg.h"
#include <Rso/Log/Log.h>
#include "SMAs.h"


namespace rso
{
	namespace stock
	{
		using namespace log;

		//class CStgOpenGap : public CStg<CNeuron>
		//{
		//	CLog _Log{ Target_File, EPeriod::EveryRun, L"StockLog/", L"Normal", ELocale::korean };
		//	CSMAs _SMAs{ 120, 60, 20, 5 };

		//	int _EnterDataCallback(const SEnterDataInfo& DataInfo_, const SDataDb& Data_) override;
		//	int _LeaveDataCallback(const SLeaveDataInfo& DataInfo_, const SDataDb& Data_) override;
		//	int _EnterCheckCallback(const SEnterDataInfo& DataInfo_, const TIn& In_, size_t DataCnt_, const TOut& OutMean_) override;
		//	int _LeaveCheckCallback(const SLeaveDataInfo& DataInfo_, const TIn& In_, size_t DataCnt_, const TOut& OutMean_) override;

		//public:
		//	CStgOpenGap(TValue TradeCntUnit_, time_t TradeStartDate_, TTraderCallback TraderCallback_, size_t SampleCnt_);
		//};
	}
}
