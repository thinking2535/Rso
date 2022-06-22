#pragma once

#include "Base.h"

namespace rso::gameutil
{
	class CFixedRandom32
	{
		uint32 _Seed;
	public:
		CFixedRandom32(uint32 Seed_);
		inline uint32 GetSeed(void) const { return _Seed; }
		uint32 Get(void);
	};
}
