#include "StgOpenGap.h"


namespace rso
{
	namespace stock
	{
		//int CStgOpenGap::_EnterDataCallback(const SEnterDataInfo& DataInfo_, const SDataDb& Data_)
		//{
		//	_SMAs.insert(Data_.Close);

		//	int Ret = 0;
		//	//if (DataInfo_.IsEnd)
		//	//{
		//	//	auto Avg0 = _SMAs[0].avg();
		//	//	auto Avg1 = _SMAs[1].avg();
		//	//	auto Avg2 = _SMAs[2].avg();
		//	//	auto Avg3 = _SMAs[3].avg();
		//	//	auto Diff0 = _SMAs[0].diff();
		//	//	auto Diff1 = _SMAs[1].diff();
		//	//	auto Diff2 = _SMAs[2].diff();
		//	//	auto Diff3 = _SMAs[3].diff();
		//	//	auto Diff2_0 = _SMAs[0].diff2();
		//	//	auto Diff2_1 = _SMAs[1].diff2();
		//	//	auto Diff2_2 = _SMAs[2].diff2();
		//	//	auto Diff2_3 = _SMAs[3].diff2();

		//	//	Ret = Remind(Data_,
		//	//		TIn{
		//	//		//					double(Data_.Close - _PrevDayClose),
		//	//		Diff0,
		//	//		Avg1 - Avg0,
		//	//		Avg2 - Avg1,
		//	//		Avg3 - Avg2,
		//	//		Diff1 - Diff0,
		//	//		Diff2 - Diff1,
		//	//		Diff3 - Diff2,
		//	//		Diff2_0,
		//	//		Diff2_1,
		//	//		Diff2_2,
		//	//		Diff2_3 });
		//	//}

		//	//if (DataInfo_.IsBegin && DayDatas().size() > 1)
		//	//	Memory(TOut{ double(Data_.Close - DayDatas()[DayDatas().size() - 2].Trend.Close()) });

		//	return Ret;
		//}
		//int CStgOpenGap::_LeaveDataCallback(const SLeaveDataInfo& DataInfo_, const SDataDb& Data_)
		//{
		//	if (DataInfo_.IsBegin)
		//	{
		//		if (DayDatas().size() > 1)
		//			Memory(TOut{ double(Data_.Close - DayDatas()[DayDatas().size() - 2].Trend.Close()) });

		//		return 1;
		//	}

		//	return 0;
		//}
		//int CStgOpenGap::_EnterCheckCallback(const SEnterDataInfo& DataInfo_, const TIn& In_, size_t DataCnt_, const TOut& OutMean_)
		//{
		//	if (OutMean_[0] > 20.0)
		//	{
		//		_Log.Push(GetFormat(L"DateTime[%d] Enter Buy", CurData().Time()));
		//		return 1;
		//	}
		//	else if (OutMean_[0] < -20.0)
		//	{
		//		_Log.Push(GetFormat(L"DateTime[%d] Enter Sell", CurData().Time()));
		//		return -1;
		//	}

		//	return 0;
		//}
		//int CStgOpenGap::_LeaveCheckCallback(const SLeaveDataInfo& DataInfo_, const TIn& In_, size_t DataCnt_, const TOut& OutMean_)
		//{
		//	return 1;
		//}
		//CStgOpenGap::CStgOpenGap(TValue TradeCntUnit_, time_t TradeStartDate_, TTraderCallback TraderCallback_, size_t SampleCnt_) :
		//	CStg(11, 1, TraderCallback_, SampleCnt_, TradeCntUnit_, TradeStartDate_)
		//{
		//}
	}
}