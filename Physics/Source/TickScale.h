#pragma once

#include "Tick.h"

namespace rso::physics
{
    using namespace std;

    class CTickScale : public CTick
	{
        double _Scale = 1.0;
        int64 _AccTick = 0; // C++ 코드 통일 위함.

    public:
        CTickScale() noexcept
        {
        }
        CTickScale(int64 Tick_) :
            CTick(Tick_)
        {
        }
        CTickScale(double Scale_) noexcept
        {
            _Scale = Scale_;
        }
        CTickScale(int64 Tick_, double Scale_) :
            CTick(Tick_)
        {
            _Scale = Scale_;
        }
        void SetScale(double Scale_) noexcept
        {
            _AccTick = Get();
            Set(0);
            _Scale = Scale_;


            Set(static_cast<int64>(Get() * _Scale));
            _Scale = Scale_;
        }
        inline int64 Get(void) const noexcept
        {
            return _AccTick + (int64)(CTick::Get() * _Scale);
        }
	};
}