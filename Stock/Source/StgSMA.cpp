#include "StgSMA.h"


namespace rso
{
	namespace stock
	{
		//CStgSMA::CStgSMA(TValue TradeCntUnit_, TValue DayTradeCntMax_, _TCallback Callback_, const double MaxDistance_, size_t SampleCnt_) :
		//	CStg(TradeCntUnit_, DayTradeCntMax_),
		//	_Callback(Callback_)
		//{
		//	ASSERTION(TradeCntUnit_ > 0);
		//}
		//void CStgSMA::Remind(const SDataDb& Data_)
		//{
		//	_InData = Data_;
		//	_DayData.Set(Data_);
		//	_SMAs.insert(Data_.Close);

		//	auto TimeT = DateTimeToTimeT(_DayData.CurData().Time());

		//	if (_WaitingOrder)
		//		return;

		//	auto Avg0 = _SMAs.avg(0);
		//	auto Avg1 = _SMAs.avg(1);
		//	auto Avg2 = _SMAs.avg(2);
		//	auto Avg3 = _SMAs.avg(3);
		//	auto Diff0 = _SMAs.diff(0);
		//	auto Diff1 = _SMAs.diff(1);
		//	auto Diff2 = _SMAs.diff(2);
		//	auto Diff3 = _SMAs.diff(3);
		//	auto Diff2_0 = _SMAs.diff2(0);
		//	auto Diff2_1 = _SMAs.diff2(1);
		//	auto Diff2_2 = _SMAs.diff2(2);
		//	auto Diff2_3 = _SMAs.diff2(3);

		//	if (_Asset.GetOpenInterest() != 0)
		//	{
		//		try
		//		{
		//			if (_Asset.GetOpenInterest() > 0)	// Sell Exit
		//			{
		//				auto itHave = _HaveItems.begin();
		//				if (Data_.Close - itHave->first > 100)	// 일정 이상 이득을 봤으면
		//				{
		//					if (Diff0 + Diff1 + Diff2 + Diff3 < 0.0 &&
		//						Diff2_0 < 0.0 &&
		//						Diff2_1 < 0.0 &&
		//						Diff2_2 < 0.0 &&
		//						Diff2_3 < 0.0)
		//						throw(1);
		//				}
		//				else
		//				{
		//					if (Diff0 + Diff1 + Diff2 + Diff3 < 0.0 &&
		//						Diff2_0 < 0.0 &&
		//						Diff2_1 < 0.0 &&
		//						Diff2_2 < 0.0 &&
		//						Diff2_3 < 0.0 &&
		//						Avg3 < Avg0)  // Avg3 < Avg0 이거나 일정시간 지났으면 
		//						throw(2);
		//				}
		//			}
		//			else if (_Asset.GetOpenInterest() < 0)	// Buy Exit
		//			{
		//				auto itHave = _HaveItems.begin();
		//				if (Data_.Close - itHave->first < -100)	// 일정 이상 이득을 봤으면
		//				{
		//					if (Diff0 + Diff1 + Diff2 + Diff3 > 0.0 &&
		//						Diff2_0 > 0.0 &&
		//						Diff2_1 > 0.0 &&
		//						Diff2_2 > 0.0 &&
		//						Diff2_3 > 0.0)
		//						throw(1);
		//				}
		//				else
		//				{
		//					if (Diff0 + Diff1 + Diff2 + Diff3 > 0.0 &&
		//						Diff2_0 > 0.0 &&
		//						Diff2_1 > 0.0 &&
		//						Diff2_2 > 0.0 &&
		//						Diff2_3 > 0.0 &&
		//						Avg3 > Avg0) // Avg3 > Avg0 이거나 일정시간 지났으면 
		//						throw(2);
		//				}
		//			}
		//		}
		//		catch (const int ExitCode_)
		//		{
		//			_Log.Push(GetFormat(L"DateTime[%d]", _InData.Time));
		//			_Callback(TimeT, -_Asset.GetOpenInterest());
		//		}
		//	}
		//	else	// Enter
		//	{
		//		if (!_CanOrder())
		//			return;

		//		if (Data_.Time % 10000000000 > 1400000000)
		//			return;

		//		//Diff들의 분산?
		//		//어느 방향으로 일정 강도 이상 움직이면 반대진입 금지.
		//		//당일 강한추세인지, 진동하는지 추세 판단.
		//		//산, 골 의 연장선으로 trendLine 파악? 그것에 반하면 진입금지?

		//		if (_DayData.CurData().Price() - _DayData.Low() < 100 &&
		//			Avg0 + 2.0 < Avg1 && Avg0 + 7.0 > Avg1 &&
		//			Avg1 + 2.0 < Avg2 && Avg1 + 7.0 > Avg2 &&
		//			Avg2 + 2.0 < Avg3 && Avg2 + 7.0 > Avg3 &&
		//			Diff0 > 0.0 &&
		//			Diff1 > 0.0 &&
		//			Diff2 > 0.0 &&
		//			Diff3 > 0.0 &&
		//			Diff0 + Diff1 + Diff2 + Diff3 > 3.5)
		//		{
		//			_Log.Push(GetFormat(L"DateTime[%d] Enter Buy", _InData.Time));
		//			_Callback(TimeT, _TradeCntUnit);
		//		}
		//		else if (_DayData.High() - _DayData.CurData().Price() < 100 &&
		//			Avg0 - 2.0 > Avg1 && Avg0 - 7.0 < Avg1 &&
		//			Avg1 - 2.0 > Avg2 && Avg1 - 7.0 < Avg2 &&
		//			Avg2 - 2.0 > Avg3 && Avg2 - 7.0 < Avg3 &&
		//			Diff0 < 0.0 &&
		//			Diff1 < 0.0 &&
		//			Diff2 < 0.0 &&
		//			Diff3 < 0.0 &&
		//			Diff0 + Diff1 + Diff2 + Diff3 < -3.5)
		//		{
		//			_Log.Push(GetFormat(L"DateTime[%d] Enter Sell", _InData.Time));
		//			_Callback(TimeT, -_TradeCntUnit);
		//		}
		//	}
		//}
		//void CStgSMA::Input(const vector<SDataDb>& Datas_)
		//{
		//	_DayData.Clear();
		//}
	}
}