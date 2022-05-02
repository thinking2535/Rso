#include "StgSMAs.h"


namespace rso
{
	namespace stock
	{
		//// delete me
		////int CStgSMAs::_DayNeuronCallback(const _TDayNeuronIn& In_, const _TDayNeuronOut& OutMean_)
		////{
		////	_DayNeuronOutReady = true;
		////	_DayNeuronOut = OutMean_;
		////	return 0;
		////}
		//bool CStgSMAs::_InputCallback(const SDataDb& Data_, const SDataInfo& DataInfo_, TIn& In_)
		//{
		//	if (DataInfo_.IsEnd)
		//	{
		//		_TimeValue = 0;

		//		// delete me
		//		//auto& Days = DayDatas();
		//		//if (Days.size() >= c_DayNeuronInCnt)
		//		//{
		//		//	_TDayNeuronIn DayNeuronIn;
		//		//	for (size_t i = 0; i < c_DayNeuronInCnt; ++i)
		//		//		DayNeuronIn[i] = Days[Days.size() - c_DayNeuronInCnt + i].Trend.Close(); 절대값에 대한 차로 다시 계산할것.

		//		//	_DayNeuron.Remind(_DayNeuronTime, DayNeuronIn);
		//		//	아래 back()에서 size() -2 를 빼야 함.
		//		//	_DayNeuron.Memory(_DayNeuronTime, _TDayNeuronOut{ double(Days[Days.size() - 2].Trend.Open() - Days.back().Trend.Close()), double(Days[Days.size() - 2].Trend.Close() - Days.back().Trend.Close()) });
		//		//}

		//		//++_DayNeuronTime;
		//	}
		//	else
		//		++_TimeValue;

		//	for (auto& Data : _Datas)
		//		Data.insert(Data_.Close);

		//	// delete me
		//	//if (!_DayNeuronOutReady)
		//	//	return false;

		//	if (!_Datas.front().full())
		//		return false;

		//	size_t Index = 0; // 분산이 가장 작은값만 처리?, 오픈갭도 포함? 입력을 현재값 기준 차를 넣을 경우 현재가 는 range가 너무 작게 되는데...
		//	// delete me
		//	//아래 장중 현재가에 따라 _DayNeuronOut[1] - _DayNeuronOut[0] 의 영향이 달라지므로 ,
		//	//	장 내내 _DayNeuronOut[1] - _DayNeuronOut[0] 를 InData 로 하는것은 문제가 있음.

		//	//In_[Index++] = _DayNeuronOut[1] - _DayNeuronOut[0]; // OpenGap 은 사용하지 않고, Close와 Open과의 차 만을 사용
		//	In_[Index++] = double(_TimeValue);
		//	In_[Index++] = Data_.Close - _Datas[0][0];
		//	for (size_t j = 0; j < _Datas.size(); ++j)
		//	{
		//		auto Data = _Datas[j];
		//		if (j > 0)
		//			//In_[Index++] = (Data[0] - _Datas[j - 1][0]);	// 직전 SMA[0]과의 차
		//			In_[Index++] = (Data[0] - _Datas[0][0]);	// 가장 큰 SMA[0]과의 차

		//		for (size_t i = 1; i < Data.size(); ++i)
		//			In_[Index++] = (Data[i - 1] - Data[i]);
		//	}

		//	return true;
		//}
		//int CStgSMAs::_RemindCallback(const SDataDb& Data_, const SDataInfo& DataInfo_, const TIn& /*In_*/, const TOut& OutMin_, const TOut& OutMax_)
		//{
		//	if (DataInfo_.OpenInterest == 0)
		//	{
		//		double MaxUp = 0.0;
		//		for (auto& Out : OutMin_)
		//		{
		//			if (Out > MaxUp)
		//				MaxUp = Out;
		//			else if (MaxUp - Out > c_LossStopGap)
		//				break;

		//			if (MaxUp >= c_EnterPriceGap)
		//			{
		//				_Trend = CTrend();
		//				_Log.Push(GetFormat(L"DateTime[%d] Enter Buy", BackData().Time));
		//				return 1;
		//			}
		//		}

		//		double MaxDown = 0.0;
		//		for (auto& Out : OutMax_)
		//		{
		//			if (Out < MaxDown)
		//				MaxDown = Out;
		//			else if (MaxDown - Out < -c_LossStopGap)
		//				break;

		//			if (MaxDown <= -c_EnterPriceGap)
		//			{
		//				_Trend = CTrend();
		//				_Log.Push(GetFormat(L"DateTime[%d] Enter Sell", BackData().Time));
		//				return -1;
		//			}
		//		}

		//		return 0;
		//	}
		//	else
		//	{
		//		_Trend.set(Data_);

		//		if (DataInfo_.OpenInterest > 0)
		//		{
		//			if (_Trend.MaxDown() > c_LossStopGap)
		//				return -1;
		//		}
		//		else
		//		{
		//			if (_Trend.MaxUp() > c_LossStopGap)
		//				return 1;
		//		}

		//		return 0;
		//	}
		//}
		//bool CStgSMAs::_MemoryCallback(const SDataDb& Data_, TOut& Out_)
		//{
		//	_OutDatas.push_back(Data_.Close);
		//	if (!_OutDatas.full())
		//		return false;

		//	for (size_t i = 0; i < _OutDatas.size(); ++i)
		//		Out_[i] = (_OutDatas.at(i) - FrontData().Close);

		//	return true;
		//}
		//CStgSMAs::CStgSMAs(TValue TradeCntUnit_, time_t TradeStartDate_, TTraderCallback TraderCallback_, size_t SampleCnt_) :
		//	CStg(TraderCallback_, SampleCnt_, c_NeuronTimeGap, TradeCntUnit_, TradeStartDate_)/*, delete me
		//	_DayNeuron(std::bind(&CStgSMAs::_DayNeuronCallback, this, _1,_2), SampleCnt_, c_DayNeuronTimeGap)*/
		//{
		//}
	}
}