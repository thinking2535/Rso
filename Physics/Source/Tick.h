#pragma once

#include "Base.h"
#include <chrono>

namespace rso::physics
{
    using namespace std;

    class CTick
	{
        TTime _StartTime = system_clock::now();
        TTime _StopTime = _StartTime;

    public:
        CTick() noexcept
        {
        }
        CTick(int64 Tick_)
        {
            Set(Tick_);
        }
        inline void Set(int64 Tick_) noexcept
        {
            _StartTime = system_clock::now() - t_duration(Tick_);
        }
        inline void Stop(void) noexcept
        {
            _StopTime = system_clock::now();
        }
        inline void Start(void) noexcept
        {
            _StartTime = system_clock::now() - _StopTime + _StartTime;
            _StopTime = TTime();
        }
        inline int64 Get(void) const noexcept
        {
            if (_StopTime == TTime())
                return (system_clock::now() - _StartTime).count();
            else
                return (_StopTime - _StartTime).count();
        }
        inline bool IsStarted(void) const
        {
            return _StopTime == TTime();
        }
	};
}