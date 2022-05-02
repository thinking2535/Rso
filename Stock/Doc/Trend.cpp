#include "Trend.h"


namespace rso
{
	namespace stock
	{
		TValue	CTrend::emplace(TValue Value_)
		{
			TValue Trnd{ 0 };

			if (_Datas.size() > 0)
				Trnd = Value_ - _Datas.front();

			_Datas.emplace_back(Value_);

			if (Value_ >= _High)
				_High = Value_;

			if (Value_ <= _Low)
				_Low = Value_;

			if (Trnd > 0)
				return Value_ - _Low;
			else if (Trnd < 0)
				return Value_ - _High;
			else
				return 0;
		}

		void		CTrend::clear(void)
		{
			_Datas.clear();
			_High = MinValue<TValue>();
			_Low = MaxValue<TValue>();
		}
	}
}
