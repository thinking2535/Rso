#include "Statistics.h"


namespace rso
{
	namespace stock
	{
		TValue	CStatistics::SetAsset(TValue Asset_)
		{
			auto Ret = Asset_ - _LastAsset;
			if (Ret > 0)
				++_WinCnt;
			else
				++_LoseCnt;

			_LastAsset = Asset_;

			return Ret;
		}
		TValue CStatistics::GetAsset(void) const
		{
			return _LastAsset;
		}
		TValue CStatistics::TradeCnt(void) const
		{
			return (_WinCnt + _LoseCnt);
		}
		double CStatistics::WinRatio(void) const
		{
			if (_WinCnt + _LoseCnt == 0)
				return 0.0;

			return (double(_WinCnt) / (double(_WinCnt) + double(_LoseCnt)));
		}
		double CStatistics::Commission(void) const
		{
			return (((_InitAsset + _LastAsset) / 2.0) * _CommissionRatio) * TradeCnt();
		}
		double CStatistics::Gain(void) const
		{
			return (_LastAsset - (((_InitAsset + _LastAsset) / 2.0) * _CommissionRatio) * TradeCnt()) - _InitAsset;
		}
		wstring CStatistics::ToString(void) const
		{
			return StringFormat(L"WinCnt[%d] LoseCnt[%d] WinRatio[%f] Commision[%f] Gain[%f]", _WinCnt, _LoseCnt, WinRatio(), Commission(), Gain());
		}
	}
}