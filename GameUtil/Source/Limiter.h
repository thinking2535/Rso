#pragma once

#include "Base.h"

namespace rso::gameutil
{
    class Limiter
    {
        int64 _durationTicks;
        int64 _lastTick = 0;

    public:
        Limiter(int64 durationTicks);
        bool push(int64 tick);
    };
}
