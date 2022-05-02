#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		class CCandle
		{
			TValue _Open = 0;
			TValue _High = 0;
			TValue _Low = (numeric_limits<TValue>::max)();
			TValue _Close = 0;

		public:
			void set(const SDataDb& Data_);
			TValue	Open(void) const
			{
				return _Open;
			}
			TValue	High(void) const
			{
				return _High;
			}
			TValue	Low(void) const
			{
				return _Low;
			}
			TValue	Close(void) const
			{
				return _Close;
			}
			TValue Gap(void) const
			{
				return _High - _Low;
			}
			TValue	Level(void) const
			{
				return GetLevel(_High, _Low, _Close);
			}
		};
	}
}