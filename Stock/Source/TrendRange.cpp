#include "TrendRange.h"


namespace rso
{
	namespace stock
	{
		CTrendRange::CTrendRange(time_t Range_) :
			_Range(Range_)
		{
		}

		TValue	CTrendRange::emplace(time_t TimeT_, TValue Value_)
		{
			TValue Trnd{ 0 };

			if (_Datas.size() > 0)
				Trnd = Value_ - _Datas.front().Value;

			_Datas.emplace(_Counter, TimeT_, Value_);

			if (Value_ >= _High.Value)
				new(&_High)_SData(_Counter, TimeT_, Value_);

			if (Value_ <= _Low.Value)
				new(&_Low)_SData(_Counter, TimeT_, Value_);

			for (; _Datas.back().TimeT - _Datas.front().TimeT >= _Range;)
			{
				if (_Datas.front().Index == _High.Index)
					new(&_High)_SData(MinValue<TValue>());

				if (_Datas.front().Index == _Low.Index)
					new(&_Low)_SData(MaxValue<TValue>());

				_Datas.pop();
			}

			if (!_High.Valid)
			{
				for (auto& Data : _Datas)
				{
					if (Data.Value >= _High.Value)
						_High = Data;
				}
			}

			if (!_Low.Valid)
			{
				for (auto& Data : _Datas)
				{
					if (Data.Value <= _Low.Value)
						_Low = Data;
				}
			}

			++_Counter;

			if (Trnd > 0)
				return Value_ - _Low.Value;
			else if (Trnd < 0)
				return Value_ - _High.Value;
			else
				return 0;
		}

		void		CTrendRange::clear(void)
		{
			_Datas.clear();
			new(&_High)_SData(MinValue<TValue>());
			new(&_Low)_SData(MaxValue<TValue>());
		}
	}
}
