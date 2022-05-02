#pragma once


#include "Thread.h"


namespace rso
{
    namespace base
    {
        class CTimer
        {
        public:
            using TCallback = function<void()>;

        private:
            TCallback _Callback;
            TTime _ExpireTime;
            TSpan _Precision;
			CThread _Thread;

            void _Worker(const volatile bool* Exit_);

        public:
            CTimer(TCallback Callback_, const TTime& ExpireTime_, const TSpan& Precision_); // Multi thread callback
        };
    }
}
