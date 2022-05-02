#include "Timer.h"


namespace rso
{
    namespace base
    {
        void CTimer::_Worker(const volatile bool* Exit_)
        {
            while (*Exit_ == false)
            {
                if (system_clock::now() > _ExpireTime)
                {
                    _Callback();
                    return;
                }
                this_thread::sleep_for(_Precision);
            }
        }
        CTimer::CTimer(TCallback Callback_, const TTime& ExpireTime_, const TSpan& Precision_) :
            _Callback(Callback_),
			_ExpireTime(ExpireTime_),
			_Precision(Precision_),
			_Thread(std::bind(&CTimer::_Worker, this, _1))
        {
        }
    }
}