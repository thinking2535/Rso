// 고정 개수를 유지하는 요소들 간의 평균 효과

#pragma once


#include "Base.h"


namespace rso
{
	namespace math
	{
		class CFixedAvg
		{
			double _Cnt = 1.0;
			double _Avg = 0.0;

		public:
			CFixedAvg(unsigned __int32 Cnt_) :
				_Cnt(double(Cnt_))
			{
			}
			CFixedAvg(unsigned __int32 Cnt_, double Avg_) :
				_Cnt(double(Cnt_)), _Avg(Avg_)
			{
			}
			double operator += (double Value_)
			{
				return (_Avg = (_Avg * _Cnt + Value_) / (_Cnt + 1.0));
			}
			double operator -= (double Value_)
			{
				return (_Avg = (_Avg * _Cnt - Value_) / (_Cnt + 1.0));
			}
			explicit operator double() const
			{
				return _Avg;
			}
		};
	}
}