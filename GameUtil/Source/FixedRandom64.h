#pragma once

#include "FixedRandom32.h"

namespace rso::gameutil
{
	class CFixedRandom64
	{
		CFixedRandom32 _Random32_0;
		CFixedRandom32 _Random32_1;

	public:
		CFixedRandom64(uint64 Seed_) :
			_Random32_0((uint32)(Seed_ & 0xFFFFFFFF)),
			_Random32_1((uint32)(Seed_ >> 32))
		{
		}
		inline uint64 GetSeed(void) const
		{
			return (((uint64)_Random32_1.GetSeed()) << 32) + (uint64)_Random32_0.GetSeed();
		}
		inline uint64 Get(void)
		{
			return (((uint64)_Random32_1.Get()) << 32) + (uint64)_Random32_0.Get();
		}
	};
}
