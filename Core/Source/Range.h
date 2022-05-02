#pragma once


#include "Base.h"
#include <limits>


namespace rso
{
	namespace core
	{
		using namespace std;

		template<typename TType>
		class CRange
		{
			TType _Min = MaxValue<TType>();
			TType _Max = MinValue<TType>();

		public:
			inline const TType& Min(void) const
			{
				return _Min;
			}
			inline const TType& Max(void) const
			{
				return _Max;
			}
			TType Range(void) const
			{
				if (_Max < _Min)
					return TType{};

				return (_Max - _Min);
			}
			void Set(const TType& Value_)
			{
				if (Value_ < _Min)
					_Min = Value_;
				if (_Max < Value_)
					_Max = Value_;
			}
			void clear(void)
			{
				_Min = MaxValue<TType>();
				_Max = MinValue<TType>();
			}
		};
	}
}