#pragma once

#include "Base.h"

namespace rso
{
	namespace gameutil
	{
		class CFixedRandom
		{
			uint64 _Seed = 0;
			uint64 _Counter;

		public:
			CFixedRandom(uint64 Seed_, uint64 Counter_);
			inline uint64 GetSeed(void) const
			{
				return _Seed;
			}
			inline uint64 GetCounter(void) const
			{
				return _Counter;
			}
			uint64 Get(void);
		};
	}
}