#include "FixedRandom.h"

namespace rso
{
	namespace gameutil
	{
		CFixedRandom::CFixedRandom(uint64 Seed_, uint64 Counter_) :
			_Seed(Seed_),
			_Counter(Counter_)
		{
		}
		uint64 CFixedRandom::Get(void)
		{
			++_Counter;
			uint64 ShiftCntSeed = ((_Counter % 128) >> 1);
			uint64 ShiftCntValue = (_Counter % 64);
			return ((_Seed << ShiftCntSeed) | (_Seed >> (64 - ShiftCntSeed)) ^
				(_Counter << ShiftCntValue) | (_Counter >> (64 - ShiftCntValue)));
		}
	}
}