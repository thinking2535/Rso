#pragma once

#include "Base.h"
#include <chrono>

namespace rso
{
    namespace base
    {
        using namespace chrono;

        template<typename TDuration>
        class CPeriod
        {
        private:
            TDuration _Period;
            TTime _Next;

        public:
            CPeriod()
            {}
            CPeriod(const TDuration& Period_) :
                _Period(Period_),
                _Next(system_clock::now() + _Period)
            {}
            CPeriod(const TDuration& Period_, const TTime& Next_) :
                _Period(Period_),
                _Next(Next_)
            {}
			explicit operator bool() const
			{
				return (_Period != TDuration());
			}
            inline TDuration GetPeriod(void) const
            {
                return _Period;
            }
            void Reset(const TDuration& Period_, const TTime& Next_)
            {
                _Period = Period_;
                _Next = Next_;
            }
            inline void NextFixed(void)
            {
                _Next += _Period;
            }
            inline void NextLoose(void)
            {
                _Next = (system_clock::now() + _Period);
            }
            bool CheckAndNextFixed(void)
            {
                if (system_clock::now() < _Next)
                    return false;

                NextFixed();

                return true;
            }
            bool CheckAndNextLoose(void)
            {
                if (system_clock::now() < _Next)
                    return false;

                NextLoose();

                return true;
            }
        };
    }
}
