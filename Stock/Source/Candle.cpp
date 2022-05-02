#include "Candle.h"


namespace rso
{
	namespace stock
	{
		void	CCandle::set(const SDataDb& Data_)
		{
			if (_Open == 0)
				Data_.Open;

			if (Data_.High > _High)
				_High = Data_.High;
			if (Data_.Low < _Low)
				_Low = Data_.Low;

			_Close = Data_.Close;
		}
	}
}