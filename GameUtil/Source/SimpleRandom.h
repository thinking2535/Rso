#pragma once

#include "Base.h"

namespace rso
{
	namespace gameutil
	{
		class CSimpleRandom
		{
			uint64 _Seed = 0;

		public:
			CSimpleRandom(uint64 Seed_);
			uint64 Get(uint64 Value_);
		};
	}
}