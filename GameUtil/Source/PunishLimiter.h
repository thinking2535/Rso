#pragma once


#include "Base.h"
#include <Rso/Base/Limiter.h>


namespace rso
{
	namespace gameutil
	{
		using namespace base;

		template<typename TPeriod, typename TCount>
		class CPunishLimiter
		{
			using _TLimiter = CLimiter<TPeriod, TCount>;

			struct _SLimiter
			{
				_TLimiter Limiter;
				TPeriod PunishPeriod;

				_SLimiter(const _TLimiter& Limiter_, const TPeriod& PunishPeriod_) :
					Limiter(Limiter_), PunishPeriod(PunishPeriod_)
				{
				}
			};
			list<_SLimiter>	_Limiters;
			TTime _PunishTime;

		public:
			void Add(TPeriod Period_, TCount LimitCount_, TPeriod PunishPeriod_)
			{
				_Limiters.emplace_back(_SLimiter(_TLimiter(Period_, LimitCount_), PunishPeriod_));
			}
			bool Set(TCount Count_)
			{
				auto Now = system_clock::now();

				if (Now < _PunishTime)
					return false;

				for (auto& i : _Limiters)
				{
					if (!i.Limiter.Set(Count_))
					{
						_PunishTime = Now + i.PunishPeriod;
						return false;
					}
				}

				return true;
			}
		};
	}
}
