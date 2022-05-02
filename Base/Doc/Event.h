#pragma once

#include <Rso/Core/Time.h>

namespace rso
{
    namespace base
    {
        using namespace core;

        class CEvent
        {
        public:
            enum EPeriod
            {
                Year,
                Month,
                Week,
                Day,
                Hour,
                Minute,
                Second,
                Max,
                Null
            };

        private:
            EPeriod _Period = EPeriod::Null;
            CTime _Next{ nowtime };

        public:
            CEvent(EPeriod Period_, const CTime& Next_);
            CEvent(int Month_, int Day_, int Hour_, int Minute_, int Second_);
            CEvent(int Day_, int Hour_, int Minute_, int Second_);
            CEvent(EDayOfWeek DayOfWeek_, int Hour_, int Minute_, int Second_);
            CEvent(int Hour_, int Minute_, int Second_);
            CEvent(int Minute_, int Second_);
            CEvent(int Second_);
            CEvent();
            const CTime& GetNext(void) const;
            bool CheckAndNextFixed(void);
        };
    }
}
