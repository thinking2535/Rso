#include "TrendTime.h"


namespace rso
{
	namespace stock
	{
		CTrendTime::CTrendTime(time_t Range_) :
			_Range(Range_)
		{
		}

		void	CTrendTime::emplace(const SData& Data_)
		{
			if (_Datas.size() == 0)
			{
				_Datas.emplace_back(Data_);
				_PrevData = Data_;

				return;
			}

			if (_PrevData.Value != Data_.Value)
			{
				if (_PrevData.TimeT - _Datas.back().TimeT >= _Range)
				{
					_Datas.emplace_back(_PrevData);
					_PrevData = Data_;
				}

				if (_Datas.size() == 1)
				{
					if (_Datas.back().Value < _PrevData.Value)
					{
						if (_PrevData.Value < Data_.Value)
							_PrevData = Data_;
						else if (_Datas.back().Value > Data_.Value)
						{
							_Datas.back().Value = _PrevData.Value;
							_PrevData = Data_;
						}
					}
					else if (_Datas.back().Value > _PrevData.Value)
					{
						if (_PrevData.Value > Data_.Value)
							_PrevData = Data_;
						else if (_Datas.back().Value < Data_.Value)
						{
							_Datas.back().Value = _PrevData.Value;
							_PrevData = Data_;
						}
					}
					else if (_PrevData.Value != Data_.Value)
					{
						_PrevData = Data_;
					}
				}
				else
				{
					if (_Datas[_Datas.size() - 2].Value < _Datas.back().Value)
					{
						if (_Datas.back().Value < _PrevData.Value)
						{
							_Datas.back() = _PrevData;
						}
						else
						{
							if (_PrevData.Value > Data_.Value)
								_PrevData = Data_;
							else if (_Datas.back().Value < Data_.Value)
								_Datas.back() = _PrevData = Data_;
						}
					}
					else
					{
						if (_Datas.back().Value > _PrevData.Value)
						{
							_Datas.back() = _PrevData;
						}
						else
						{
							if (_PrevData.Value < Data_.Value)
								_PrevData = Data_;
							else if (_Datas.back().Value > Data_.Value)
								_Datas.back() = _PrevData = Data_;
						}
					}
				}
			}
		}
		void	CTrendTime::clear(void)
		{
			_Datas.clear();
		}
		size_t CTrendTime::size(void) const
		{
			return _Datas.size();
		}
		TValue	CTrendTime::get(void) const
		{
			if (_Datas.size() < 2)
				return 0;
			else
				return _Datas.back().Value - _Datas[_Datas.size() - 2].Value;
		}
	}
}
