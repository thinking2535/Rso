#include "StgDynamicChart.h"


namespace rso
{
	namespace stock
	{
	//	const double CStgDynamicChart::c_EnterHighLowGap = 30.0;
	//	const double CStgDynamicChart::c_EnterPriceLevel = 10.0;
	//	const double CStgDynamicChart::c_LeaveHighLowGap = 5.0;	// 이 값보다 작으면 청산
	//	const double CStgDynamicChart::c_LeavePriceLevel = 100.1;

	//	bool	CStgDynamicChart::_PushIn(const SPriceVolume& Data_)
	//	{
	//		if (_PrevDayClose == 0)	// 전일 종가 데이터가 없다면
	//			return false;

	//		_SMADynamicIn.push_back(Data_);

	//		return true;
	//	}
	//	CStgDynamicChart::_SOutInfo	CStgDynamicChart::_GetOutInfo(size_t DataCnt_, const TOut& OutMean_, const TOut& OutSD_) const
	//	{
	//		// Get Price Level
	//		double HighPrice = 0.0;
	//		double LowPrice = 0.0;
	//		double End = OutMean_[DataCnt_ - 1];
	//		for (size_t i = 0; i < DataCnt_; ++i)
	//		{
	//			if (OutMean_[i] > HighPrice)
	//				HighPrice = OutMean_[i];
	//			if (OutMean_[i] < LowPrice)
	//				LowPrice = OutMean_[i];
	//		}

	//		return _SOutInfo(HighPrice, LowPrice, (-LowPrice) * 100.0 / (HighPrice - LowPrice), End, OutSD_[DataCnt_ - 1]);
	//	}
	//	void	CStgDynamicChart::_NeuronCallback(size_t DataCnt_, const TOut& OutMean_, const TOut& OutSD_)
	//	{
	//		auto TimeT = DateTimeToTimeT(_DayData.CurData().Time());

	//		if (_WaitingOrder)
	//			return;

	//		//_SMAOutInfo.push_back(_GetOutInfo(DataCnt_, OutMean_, OutSD_));
	//		//if (_SMAOutInfo.size() == 0)
	//		//	return;
	//		//auto OutInfo = _SMAOutInfo.at(0);
	//		auto OutInfo = _GetOutInfo(DataCnt_, OutMean_, OutSD_);

	//		if (_Asset.GetOpenInterest() != 0)
	//		{
	//			try
	//			{
	//				if (TimeT - _EnteredTimeT > 60 * 60)
	//					throw(1);

	//				if (_Asset.GetOpenInterest() > 0)	// Sell Exit
	//				{
	//					//if (OutInfo.Low < -2.0)
	//					//	throw(1);

	//					//if ((OutInfo.High - OutInfo.Low) < c_LeaveHighLowGap)
	//					//	throw(2);

	//					//if (OutInfo.PriceLevel > c_LeavePriceLevel)
	//					//	throw(3);
	//				}
	//				else if (_Asset.GetOpenInterest() < 0)	// Buy Exit
	//				{
	//					//if (OutInfo.High > 2.0)
	//					//	throw(1);

	//					//if ((OutInfo.High - OutInfo.Low) < c_LeaveHighLowGap)
	//					//	throw(2);

	//					//if (OutInfo.PriceLevel < (100.0 - c_LeavePriceLevel))
	//					//	throw(3);
	//				}
	//			}
	//			catch (const int ExitCode_)
	//			{
	//				wstringstream OutMeanStr(L"OutMeanDatas : ");
	//				for (size_t i = 0; i < DataCnt_; ++i)
	//					OutMeanStr << OutMean_[i] << L" ";
	//				_Log.Push(GetFormat(L"DateTime[%d] ExitCode[%d] HighLowGap[%f] PriceLevel[%f] End[%f]", _InData.Time, ExitCode_, (OutInfo.High - OutInfo.Low), OutInfo.PriceLevel, OutInfo.End));
	//				_Log.Push(OutMeanStr.str());
	//				_Callback(TimeT, -_Asset.GetOpenInterest());
	//			}
	//		}
	//		else	// Enter
	//		{
	//			if (!_CanOrder())
	//				return;

	//			if (DataCnt_ < 6)	// 종료까지 대략 1시간 이하 남았으면 진입금지.
	//				return;

	//			//if ((OutInfo.High - OutInfo.Low) < c_EnterHighLowGap)
	//			//	return;

	//			if (OutMean_[5] > c_EnterHighLowGap)
	//			{
	//				//if (OutInfo.Low - CurPrice < 0.0)
	//				//	return;

	//				//if (_Predictor.GetDayData().High() - _Predictor.GetDayData().Low() > 200 &&
	//				//	GetLevel(_Predictor.GetDayData().High(), _Predictor.GetDayData().Low(), _Predictor.CurData().Price()) > 90)
	//				//	return;

	//				//if (OutInfo.EndSD > 0.5)
	//				//	return;

	//				wstringstream OutMeanStr(L"OutMeanDatas : ");
	//				for (size_t i = 0; i < DataCnt_; ++i)
	//					OutMeanStr << OutMean_[i] << L" ";
	//				_Log.Push(GetFormat(L"DateTime[%d] Enter Buy HighLowGap[%f] PriceLevel[%f] End[%f]", _InData.Time, (OutInfo.High - OutInfo.Low), OutInfo.PriceLevel, OutInfo.End));
	//				_Log.Push(OutMeanStr.str());
	//				_Callback(TimeT, _TradeCntUnit);
	//			}
	//			else if (OutMean_[5] < -c_EnterHighLowGap)
	//			{
	//				//if (OutInfo.High - CurPrice > 0.0)
	//				//	return;

	//				//if (_Predictor.GetDayData().High() - _Predictor.GetDayData().Low() > 200 &&
	//				//	GetLevel(_Predictor.GetDayData().High(), _Predictor.GetDayData().Low(), _Predictor.CurData().Price()) < 10)
	//				//	return;

	//				//if (OutInfo.EndSD > 0.5)
	//				//	return;

	//				wstringstream OutMeanStr(L"OutMeanDatas : ");
	//				for (size_t i = 0; i < DataCnt_; ++i)
	//					OutMeanStr << OutMean_[i] << L" ";
	//				_Log.Push(GetFormat(L"DateTime[%d] Enter Sell HighLowGap[%f] PriceLevel[%f] End[%f]", _InData.Time, (OutInfo.High - OutInfo.Low), OutInfo.PriceLevel, OutInfo.End));
	//				_Log.Push(OutMeanStr.str());
	//				_Callback(TimeT, -_TradeCntUnit);
	//			}
	//		}
	//	}
	//	void CStgDynamicChart::_ChangeDate(void)
	//	{
	//		_PrevDayClose = _DayData.Close();
	//		ClearRemind();
	//		_SMADynamicOut.clear();
	//	}
	//	CStgDynamicChart::CStgDynamicChart(TValue TradeCntUnit_, TValue DayTradeCntMax_, _TCallback Callback_, const double MaxDistance_, size_t SampleCnt_) :
	//		CStg(TradeCntUnit_, DayTradeCntMax_),
	//		_Neurons(std::bind(&CStgDynamicChart::_NeuronCallback, this, _1, _2, _3), MaxDistance_, SampleCnt_),
	//		_Callback(Callback_)
	//	{
	//		ASSERTION(TradeCntUnit_ > 0);
	//	}
	//	void CStgDynamicChart::Remind(const SDataDb& Data_)
	//	{
	//		_InData = Data_;

	//		_DayData.Set(Data_);

	//		if (!_PushIn(SPriceVolume{ Data_.Close, Data_.Volume }))
	//			return;

	//		switch (_SMADynamicIn.size())
	//		{
	//		case 1: _Remind<1>(); return;
	//		case 2: _Remind<2>(); return;
	//		case 3: _Remind<3>(); return;
	//		case 4: _Remind<4>(); return;
	//		case 5: _Remind<5>(); return;
	//		case 6: _Remind<6>(); return;
	//		case 7: _Remind<7>(); return;
	//		case 8: _Remind<8>(); return;
	//		case 9: _Remind<9>(); return;
	//		case 10: _Remind<10>(); return;
	//		case 11: _Remind<11>(); return;
	//		case 12: _Remind<12>(); return;
	//		case 13: _Remind<13>(); return;
	//		case 14: _Remind<14>(); return;
	//		case 15: _Remind<15>(); return;
	//		case 16: _Remind<16>(); return;
	//		case 17: _Remind<17>(); return;
	//		default:	return;
	//		}
	//	}
	//	void CStgDynamicChart::Input(const vector<SDataDb>& Datas_)
	//	{
	//		_SMADynamicOut.insert(Datas_);

	//		SDataDb PoppedData;
	//		for (auto Ret = _SMADynamicOut.pop_front(PoppedData); Ret; Ret = _SMADynamicOut.pop_front(PoppedData))
	//		{
	//			_DayData.Set(PoppedData);

	//			if (_SMADynamicOut.size() == 0)
	//				continue;

	//			if (!_PushIn(SPriceVolume{ PoppedData.Close, PoppedData.Volume }))
	//				continue;

	//			TOut Out;
	//			Out.reserve(_SMADynamicOut.size());
	//			//Out.emplace_back(double(Datas_.back().Close - _DayData.CurData().Price()));
	//			//Out.emplace_back(double(PoppedData.Time));

	//			for (size_t i = 0; i < _SMADynamicOut.size(); ++i)
	//				//Out.emplace_back(double(_SMADynamicOut.at(i).Close - _PrevDayClose));
	//				Out.emplace_back(double(_SMADynamicOut.at(i).Close - _DayData.CurData().Price()));

	//			switch (_SMADynamicIn.size())
	//			{
	//			case 1: _Input<1>(std::move(Out)); continue;
	//			case 2: _Input<2>(std::move(Out)); continue;
	//			case 3: _Input<3>(std::move(Out)); continue;
	//			case 4: _Input<4>(std::move(Out)); continue;
	//			case 5: _Input<5>(std::move(Out)); continue;
	//			case 6: _Input<6>(std::move(Out)); continue;
	//			case 7: _Input<7>(std::move(Out)); continue;
	//			case 8: _Input<8>(std::move(Out)); continue;
	//			case 9: _Input<9>(std::move(Out)); continue;
	//			case 10: _Input<10>(std::move(Out)); continue;
	//			case 11: _Input<11>(std::move(Out)); continue;
	//			case 12: _Input<12>(std::move(Out)); continue;
	//			case 13: _Input<13>(std::move(Out)); continue;
	//			case 14: _Input<14>(std::move(Out)); continue;
	//			case 15: _Input<15>(std::move(Out)); continue;
	//			case 16: _Input<16>(std::move(Out)); continue;
	//			case 17: _Input<17>(std::move(Out)); continue;
	//			default:			continue;
	//			}
	//		}

	//		_ChangeDate();
	//	}
	//	void CStgDynamicChart::InputAbnormal(const vector<SDataDb>& Datas_)
	//	{
	//		_DayData.Set(Datas_.back());
	//		_ChangeDate();
	//	}
	//	void CStgDynamicChart::ClearRemind(void)
	//	{
	//		_SMADynamicIn.clear();
	//		_DayData.Clear();
	//	}
	//	void CStgDynamicChart::clear(void)
	//	{
	//		_PrevDayClose = 0;
	//		ClearRemind();
	//		_SMADynamicOut.clear();

	//		Get<0>(_Neurons).clear();
	//		Get<1>(_Neurons).clear();
	//		Get<2>(_Neurons).clear();
	//		Get<3>(_Neurons).clear();
	//		Get<4>(_Neurons).clear();
	//		Get<5>(_Neurons).clear();
	//		Get<6>(_Neurons).clear();
	//		Get<7>(_Neurons).clear();
	//		Get<8>(_Neurons).clear();
	//		Get<9>(_Neurons).clear();
	//		Get<10>(_Neurons).clear();
	//		Get<11>(_Neurons).clear();
	//		Get<12>(_Neurons).clear();
	//		Get<13>(_Neurons).clear();
	//		Get<14>(_Neurons).clear();
	//		Get<15>(_Neurons).clear();
	//		Get<16>(_Neurons).clear();
	//	}
	}
}