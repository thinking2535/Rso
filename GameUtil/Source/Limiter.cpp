#include "Limiter.h"

namespace rso::gameutil
{
    Limiter::Limiter(int64 durationTicks) :
        _durationTicks(durationTicks)
    {
    }
    bool Limiter::push(int64 tick)
    {
        if (tick - _lastTick < _durationTicks)
            return false;

        _lastTick = tick;

        return true;
    }
}
