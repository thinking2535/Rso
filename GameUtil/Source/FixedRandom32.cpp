#include "FixedRandom32.h"

namespace rso::gameutil
{
	CFixedRandom32::CFixedRandom32(uint32 Seed_) :
		_Seed(Seed_)
	{
	}
	uint32 CFixedRandom32::Get(void)
	{
		uint32 Result;

		_Seed *= 1103515245;
		_Seed += 12345;
		Result = (uint32)(_Seed / 65536) % 2048;

		_Seed *= 1103515245;
		_Seed += 12345;
		Result <<= 10;
		Result ^= (uint32)(_Seed / 65536) % 1024;

		_Seed *= 1103515245;
		_Seed += 12345;
		Result <<= 10;
		Result ^= (uint32)(_Seed / 65536) % 1024;

		return Result;
	}
}