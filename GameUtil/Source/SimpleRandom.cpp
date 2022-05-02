#include "SimpleRandom.h"

namespace rso
{
	namespace gameutil
	{
		CSimpleRandom::CSimpleRandom(uint64 Seed_) :
			_Seed(0x1f3a49b72c8d5ef6 ^ Seed_)
		{
		}
		uint64 CSimpleRandom::Get(uint64 Value_)
		{
			uint64 ShiftCntSeed = ((Value_ % 128) >> 1);
			uint64 ShiftCntValue = (Value_ % 64);
			return ((_Seed << ShiftCntSeed) | (_Seed >> (64 - ShiftCntSeed)) ^
				(Value_ << ShiftCntValue) | (Value_ >> (64 - ShiftCntValue)));
		}
	}
}