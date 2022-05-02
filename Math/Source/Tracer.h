// Up, Down ���� �ܼ��� ��ǥ���� ����
// ����ġ Error �� Up, Down �� ���� ���� ���, ������ �Ǹ� FixedAvg �� ����Ͽ� ����� ����

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