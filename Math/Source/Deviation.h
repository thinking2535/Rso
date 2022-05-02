#pragma once

#include "Average.h"

namespace rso
{
	namespace math
	{
		class CDeviation
		{
			CAverage _Average;
			CAverage _Deviation;

		public:
			CDeviation(double Count_) :
				_Average(Count_),
				_Deviation(Count_)
			{
			}
			inline void operator += (double Value_)
			{
				_Average += Value_;
				_Deviation += [](double v) { return sqrt(v * v); }(double(_Average) - Value_);
			}
			inline operator double() const
			{
				return _Deviation;
			}
			inline double Average(void) const
			{
				return double(_Average);
			}
		};
	}
}