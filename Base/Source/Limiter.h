#pragma once


#include "Base.h"
#include <chrono>


namespace rso
{
    namespace base
    {
        using namespace chrono;

        template<typename TPeriod, typename TCount>
        class CLimiter
        {
        private:
            TCount _LimitCount{ 0 };
            TCount _AccCount{ 0 };
            TPeriod _Period;
            TTime _NextCheckTime;

        public:
            CLimiter(TPeriod Period_, TCount LimitCount_) :
                _LimitCount(LimitCount_),
                _Period(Period_),
                _NextCheckTime(system_clock::now() + Period_)
            {
            }
            bool Set(TCount Count_)
            {
                auto Now = system_clock::now();

                for (; _NextCheckTime < Now; _NextCheckTime += _Period, _AccCount -= (_AccCount > _LimitCount ? _LimitCount : _AccCount));

                if (_AccCount >= _LimitCount)
                    return false;

                _AccCount += Count_;

                return true;
            }
        };
    }
}