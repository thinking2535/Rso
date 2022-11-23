#pragma once

#include <chrono>
#include "Base.h"

namespace rso::gameutil
{
    using namespace chrono;

    template<typename Duration>
    class PeriodicUpdater
    {
        Duration _period;
        system_clock::time_point _lastUpdatedTime;

    public:
        inline Duration getPeriod() const { return _period; }
        inline system_clock::time_point getLastUpdatedTime() const { return _lastUpdatedTime; }

        PeriodicUpdater(const Duration& period, const system_clock::time_point& lastUpdatedTime) :
            _period(period),
            _lastUpdatedTime(lastUpdatedTime)
        {
        }
        bool update(const system_clock::time_point& now)
        {
            auto elapsed = now - _lastUpdatedTime;
            auto elapsedPeriodCount = elapsed / _period;
            if (elapsedPeriodCount <= 0)
                return false;

            _lastUpdatedTime += (_period * elapsedPeriodCount);
            return true;
        }
        Duration getLeftDurationForUpdate(const system_clock::time_point& now) const
        {
            auto mm = milliseconds(0);

            auto leftTimeSpanForUpdate = (_lastUpdatedTime + _period) - now;
            if (leftTimeSpanForUpdate < Duration())
                return Duration();
            else
                return duration_cast<Duration>(leftTimeSpanForUpdate);
        }
    };
}
