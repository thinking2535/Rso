#pragma once

#include "Base.h"

namespace rso
{
	namespace otp
	{
		class COTP
		{
			using _TValue = uint64;

			_TValue _Seed = 0;

			inline _TValue _GetMinutes(void) const
			{
				return system_clock::now().time_since_epoch().count() / 600000000;
			}
			_TValue _Get(_TValue Minutes_) const;

		public:
			COTP(_TValue Seed_);
			inline _TValue Get(void) const
			{
				return _Get(_GetMinutes());
			}
			bool IsValid(_TValue Value_) const;
		};
	}
}
