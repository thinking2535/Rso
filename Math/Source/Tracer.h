// Up, Down 만의 단서로 목표값을 추적
// 보정치 Error 는 Up, Down 에 따라 각각 양수, 음수가 되며 FixedAvg 를 사용하여 평균을 적용

#pragma once


#include "Base.h"


namespace rso
{
	namespace math
	{
		template<typename TType>
		class CTracer
		{
			TType _Accel{};
			TType _Velocity{};
			TType _Value{};

		public:
			CTracer(const TType& Accel_, const TType& Velocity_, const TType& Value_) :
				_Accel(Accel_), _Velocity(Velocity_), _Value(Value_)
			{
			}
			TType operator ++ ()
			{
				if (_Velocity < TType{})
					_Velocity = TType{};

				return (_Value += (_Velocity += _Accel));
			}
			TType operator -- ()
			{
				if (_Velocity > TType{})
					_Velocity = TType{};

				return (_Value += (_Velocity -= _Accel));
			}
			explicit operator TType()
			{
				return _Value;
			}
		};
	}
}