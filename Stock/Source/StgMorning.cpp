#include "StgMorning.h"


namespace rso
{
	namespace stock
	{
//		const double CStgMorning::c_EnterPriceGap = 50.0;
//		const double CStgMorning::c_LeavePriceGap = -2.0;
//
//		int CStgMorning::_Remind(const SDataDb& Data_)
//		{
//			_SMADynamicIn.push_back(Data_);
//
//			TIn In;
//
//			In.emplace_back(CurDayData().Trend.Trend());
//			//In_[InIndex++] = _DayData.High() - _DayData.Low();
//			//In_[InIndex++] = GetLevel(_DayData.High(), _DayData.Low(), _DayData.CurData().Price());
//
//			auto Data = _SMADynamicIn.reverse_at(0);
//			In.emplace_back(Data.Close - CurData().Price());
//			//In.emplace_back(Data.High - CurData().Price());
//			//In.emplace_back(Data.Low - CurData().Price());
//			In.emplace_back(Data.Volume);
//
//			for (size_t i = 1; i < _SMADynamicIn.size(); ++i)
//			{
//				//In_[InIndex++] = _SMADynamicIn.reverse_at(i).Price - _PrevDayClose;
//
//				Data = _SMADynamicIn.reverse_at(i);
//				In.emplace_back(Data.Close - CurData().Price());
//				//In.emplace_back(Data.High - CurData().Price());
//				//In.emplace_back(Data.Low - CurData().Price());
//				In.emplace_back(Data.Volume);
//			}
//
//			return Remind(Data_, In);
//		}
//		void CStgMorning::_Memory(const SDataDb& Data_)
//		{
//			bool IsFull = _SMADynamicOut.full();
//			_SMADynamicOut.push_back(Data_);
//			if (!IsFull)
//				return;
//
//			if (!CanMemory())
//				return;
//
//			TOut Out;
//			for (size_t i = 0; i < _SMADynamicOut.size(); ++i)
//			{
//				auto Data = _SMADynamicOut.at(i);
//				Out.emplace_back((Data.High + Data.Low) / 2 - Ins().front().Data.Close);
//			}
//			Memory(Out);
//		}
//		int CStgMorning::_EnterDataCallback(const SEnterDataInfo& DataInfo_, const SDataDb& Data_)
//		{
//			if (DataInfo_.IsBegin)
//			{
//				ClearIns();
//				_SMADynamicIn.clear();
//				_SMADynamicOut.clear();
//			}
//
//			int Ret = 0;
//			if (Data_.Time % 10000000000 < 1000000000)
//				Ret = _Remind(Data_);
//			_Memory(Data_);
//			return Ret;
//		}
//		int CStgMorning::_LeaveDataCallback(const SLeaveDataInfo& DataInfo_, const SDataDb& Data_)
//		{
//			auto Ret = _Remind(Data_);
//			_Memory(Data_);
//			return Ret;
//		}
//		int CStgMorning::_EnterCheckCallback(const SEnterDataInfo& DataInfo_, const TIn& In_, size_t DataCnt_, const TOut& OutMean_)
//		{
//			if (DayDatas().back().TradeCnt > 0)
//				return 0;
//
//			if (OutMean_[DataCnt_ - 1] > c_EnterPriceGap)
//			{
//				_Log.Push(GetFormat(L"DateTime[%d] Enter Buy", CurData().Time()));
//				return 1;
//			}
//			else if (OutMean_[DataCnt_ - 1] < -c_EnterPriceGap)
//			{
//				_Log.Push(GetFormat(L"DateTime[%d] Enter Sell", CurData().Time()));
//				return -1;
//			}
//
//			return 0;
//		}
//		int CStgMorning::_LeaveCheckCallback(const SLeaveDataInfo& DataInfo_, const TIn& In_, size_t DataCnt_, const TOut& OutMean_)
//		{
////			auto CheckIndex = (DataCnt_ > 2 ? 2 : DataCnt_ - 1);
//
//			if (OpenInterest() > 0)
//			{
//				if (DataInfo_.IsEnd ||
//					(OutMean_[DataCnt_ - 1] < c_LeavePriceGap && OutMean_[(DataCnt_ - 1) / 2] < c_LeavePriceGap) ||
//					(DataInfo_.ElapsedTimeT > _SMADynamicOut.data_cnt() * 60))
//					return -1;
//			}
//			else
//			{
//				if (DataInfo_.IsEnd ||
//					(OutMean_[DataCnt_ - 1] > -c_LeavePriceGap && OutMean_[(DataCnt_ - 1) / 2] > -c_LeavePriceGap) ||
//					(DataInfo_.ElapsedTimeT > _SMADynamicOut.data_cnt() * 60))
//					return 1;
//			}
//
//			return 0;
//		}
//		CStgMorning::CStgMorning(TValue TradeCntUnit_, time_t TradeStartDate_, TTraderCallback TraderCallback_, size_t SampleCnt_) :
//			CStg(0, 16, TraderCallback_, SampleCnt_, TradeCntUnit_, TradeStartDate_)
//		{
//		}
	}
}