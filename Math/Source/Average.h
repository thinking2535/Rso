#pragma once

#include "Base.h"

namespace rso
{
	namespace math
	{
		class CAverage
		{
			double _Count = 1.0;
			double _Sum = 0.0;

		public:
			CAverage(double Count_) :
				_Count(Count_)
			{
			}
			CAverage(double Count_, double Average_) :
				_Count(Count_),
				_Sum(Count_ * Average_)
			{
			}
			inline void SetAverage(double Average_)
			{
				_Sum = _Count * Average_;
			}
			inline double GetCount(void) const
			{
				return _Count;
			}
			inline double GetSum(void) const
			{
				return _Sum;
			}
			inline void operator += (double Value_)
			{
				_Sum *= (_Count - 1.0);
				_Sum /= _Count;
				_Sum += Value_;
			}
			inline double operator + (double Value_)
			{
				auto Ret = *this;
				Ret += Value_;
				return Ret;
			}
			inline operator double() const
			{
				return _Sum / _Count;
			}
		};
	}
}