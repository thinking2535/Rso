#pragma once


#include "Base.h"
#include <limits>


namespace rso
{
	namespace core
	{
		using namespace std;

		template<typename TType, TType Min_ = MinValue<TType>(), TType Max_ = MaxValue<TType>()>
		class CBounded
		{
			static_assert(Min_ <= Max_, "Min_ can not be greater than Max_");
			TType _Value = (TType() < Min_ ? Min_ : TType() > Max_ ? Max_ : TType());

		public:
			operator TType()
			{
				return _Value;
			}
			CBounded operator += (const CBounded& Value_)
			{
				if (Value_ > TType())
					_Value = (Max_ - _Value < Value_ ? Max_ : _Value + Value_);
				else if (Value_ < TType())
					_Value = (_Value - Min_ < Value_ ? Max_ : _Value + Value_);


				_Value = (_Value + Value_ > Max_ || _Value + Value_ > Max_ ? Max_ : _Value + Value_);
				else if (Value_ < TType())
					_Value = (_Value - Value_ < Min_ ? Max_ : _Value + Value_);

				return *this;
			}
		};
	}
}