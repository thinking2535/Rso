#include "ChartEx.h"


namespace rso
{
	namespace stock
	{
		void	CChartEx::emplace(const SData& Data_)
		{
			_LastData = Data_;

			if (_Datas.size() == 0)
			{
				_Datas.emplace(Data_);
			}
			else if (_Datas.size() == 1)
			{
				if (_Datas.back().Value != Data_.Value)
					_Datas.emplace(Data_);
			}
			else if (_Datas.size() == 2 &&
				_Datas[_Datas.size() - 1].Value - _Datas[_Datas.size() - 2].Value <= _MinAmp &&
				_Datas[_Datas.size() - 1].Value - _Datas[_Datas.size() - 2].Value >= -_MinAmp)
			{
				// 두 점이 상승중이면
				if (_Datas[_Datas.size() - 1].Value - _Datas[_Datas.size() - 2].Value > 0)
				{
					// 위 점을 뚫었으면
					if (_Datas[_Datas.size() - 1].Value < _LastData.Value)
					{
						_Datas[_Datas.size() - 1] = _LastData;
					}
					else if (_LastData.Value - _Datas[_Datas.size() - 1].Value < -_MinAmp)
					{
						_Datas.emplace(Data_);
					}
					else if (_Datas[_Datas.size() - 2].Value >= _LastData.Value)
					{
						_Datas[_Datas.size() - 2] = _Datas[_Datas.size() - 1];
						_Datas[_Datas.size() - 1] = _LastData;
					}
				}
				else
				{
					// 아래 점을 뚫었으면
					if (_Datas[_Datas.size() - 1].Value > _LastData.Value)
					{
						_Datas[_Datas.size() - 1] = _LastData;
					}
					else if (_LastData.Value - _Datas[_Datas.size() - 1].Value > _MinAmp)
					{
						_Datas.emplace(Data_);
					}
					else if (_Datas[_Datas.size() - 2].Value <= _LastData.Value)
					{
						_Datas[_Datas.size() - 2] = _Datas[_Datas.size() - 1];
						_Datas[_Datas.size() - 1] = _LastData;
					}
				}
			}
			else
			{
				// 두 점이 상승중이면
				if (_Datas[_Datas.size() - 1].Value - _Datas[_Datas.size() - 2].Value > 0)
				{
					// 위점 뚫었으면
					if (_Datas[_Datas.size() - 1].Value < _LastData.Value)
						_Datas[_Datas.size() - 1] = _LastData;
					// _LastData - 마지막 점 이 -기준 미만이면
					else if (_LastData.Value - _Datas[_Datas.size() - 1].Value < -_MinAmp)
						_Datas.emplace(_LastData);
				}
				else
				{
					// 아래점 뚫었으면
					if (_Datas[_Datas.size() - 1].Value > _LastData.Value)
						_Datas[_Datas.size() - 1] = _LastData;
					// _LastData - 마지막 점이 기준 초과면
					else if (_LastData.Value - _Datas[_Datas.size() - 1].Value > _MinAmp)
						_Datas.emplace(_LastData);
				}
			}

			while (_Datas.size() > 1)
			{
				if (Data_.TimeT - _Datas.front().TimeT < _Range)
					break;

				_Datas.pop();
			}
		}
		void	CChartEx::clear(void)
		{
			_Datas.clear();
		}
		TValue	CChartEx::Trend(size_t Index_)
		{
			if (_Datas.size() < 2 + Index_)
				return 0;

			TValue Trend = 0;

			if (Index_ == 0)
				Trend = _LastData.Value - _Datas[_Datas.size() - 2].Value;
			else
				Trend = _Datas[_Datas.size() - Index_ - 1].Value - _Datas[_Datas.size() - Index_ - 2].Value;

			if (Trend < -_MinAmp || Trend > _MinAmp)
				return Trend;
			else
				return 0;
		}
	}
}
