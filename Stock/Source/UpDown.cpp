#include "UpDown.h"


namespace rso
{
	namespace stock
	{
		void CUpDown::set(const SDataDb& Data_)
		{
			CCandle::set(Data_);

			if (Data_.Close - Low() > _MaxUp)
				_MaxUp = Data_.Close - Low();
			if (High() - Data_.Close > _MaxDown)
				_MaxDown = High() - Data_.Close;
		}
		TValue CUpDown::Trend(void) const
		{
			return _MaxUp - _MaxDown;
		}
	}
}
