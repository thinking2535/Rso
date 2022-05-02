#include "TrendLine.h"


namespace rso
{
	namespace stock
	{
		void		CTrendLine::_TrendUp(void)
		{
			if (_Trend < 0)
				_Trend = 1;
			else
				++_Trend;
		}
		void		CTrendLine::_TrendDown(void)
		{
			if (_Trend > 0)
				_Trend = -1;
			else
				--_Trend;
		}
		void	CTrendLine::emplace(const SData& Data_)
		{
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
					if (_Datas[_Datas.size() - 1].Value < Data_.Value)
					{
						_Datas[_Datas.size() - 1] = Data_;
					}
					else if (Data_.Value - _Datas[_Datas.size() - 1].Value < -_MinAmp)
					{
						_Datas.emplace(Data_);
					}
					else if (_Datas[_Datas.size() - 2].Value >= Data_.Value)
					{
						_Datas[_Datas.size() - 2] = _Datas[_Datas.size() - 1];
						_Datas[_Datas.size() - 1] = Data_;
					}
				}
				else
				{
					// 아래 점을 뚫었으면
					if (_Datas[_Datas.size() - 1].Value > Data_.Value)
					{
						_Datas[_Datas.size() - 1] = Data_;
					}
					else if (Data_.Value - _Datas[_Datas.size() - 1].Value > _MinAmp)
					{
						_Datas.emplace(Data_);
					}
					else if (_Datas[_Datas.size() - 2].Value <= Data_.Value)
					{
						_Datas[_Datas.size() - 2] = _Datas[_Datas.size() - 1];
						_Datas[_Datas.size() - 1] = Data_;
					}
				}
			}
			else
			{
				// 두 점이 상승중이면
				if (_Datas[_Datas.size() - 1].Value - _Datas[_Datas.size() - 2].Value > 0)
				{
					// 위점 뚫었으면
					if (_Datas[_Datas.size() - 1].Value < Data_.Value)
					{
						if (_Datas.size() >= 3)
						{
							// 전 고점을 뚫었으면
							if (_Datas[_Datas.size() - 3].Value >= _Datas[_Datas.size() - 1].Value &&
								_Datas[_Datas.size() - 3].Value < Data_.Value)
								_TrendUp();
						}
							
						_Datas[_Datas.size() - 1] = Data_;
					}
					// Data_ - 마지막 점이 -기준 초과이면
					else if (Data_.Value - _Datas[_Datas.size() - 1].Value < -_MinAmp)
					{
						if (_Datas.size() >= 3)
						{
							// 전 고점이 하락했으면 ( 상승한 경우는 이전에 처리했으므로 하락한 경우만 처리 )
							if (_Datas[_Datas.size() - 3].Value > _Datas[_Datas.size() - 1].Value)
								_TrendDown();
						}

						// 전 저점을 뚫었으면
						if (_Datas[_Datas.size() - 2].Value > Data_.Value)
							_TrendDown();

						_Datas.emplace(Data_);
					}
				}
				else
				{
					// 아래점 뚫었으면
					if (_Datas[_Datas.size() - 1].Value > Data_.Value)
					{
						if (_Datas.size() >= 3)
						{
							// 전 저점을 뚫었으면
							if (_Datas[_Datas.size() - 3].Value <= _Datas[_Datas.size() - 1].Value &&
								_Datas[_Datas.size() - 3].Value > Data_.Value)
								_TrendDown();
						}

						_Datas[_Datas.size() - 1] = Data_;
					}
					// Data_ - 마지막 점이 기준 초과면
					else if (Data_.Value - _Datas[_Datas.size() - 1].Value > _MinAmp)
					{
						if (_Datas.size() >= 3)
						{
							// 전 저점이 상승했으면 ( 하락한 경우는 이전에 처리했으므로 상승한 경우만 처리 )
							if (_Datas[_Datas.size() - 3].Value < _Datas[_Datas.size() - 1].Value)
								_TrendUp();
						}

						// 전 고점을 뚫었으면
						if (_Datas[_Datas.size() - 2].Value < Data_.Value)
							_TrendUp();

						_Datas.emplace(Data_);
					}
				}
			}

			if (_Datas.size() > 3)
				_Datas.pop();
		}
		TValue		CTrendLine::Trend(void) const
		{
			if (_Datas.size() < 2)
				return 0;

			return _Trend;
		}
	}
}
