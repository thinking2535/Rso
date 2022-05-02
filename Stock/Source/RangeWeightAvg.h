#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		template<typename TDuration>
		class CRangeWeightAvg
		{
			double _OneMinusEndWeight = 0.0;
			double _Duration = 0.0;
			double _Value = 0.0;
			TTime _LastInsertedTime;

		public:
			CRangeWeightAvg(double EndWeight_, const TDuration& Duration_) :
				_OneMinusEndWeight(EndWeight_ < 0.0 ? 1.0 : (EndWeight_ > 1.0 ? 0.0 : 1.0 - EndWeight_)),
				_Duration(double(Duration_.count()))
			{
			}
			void push(const TTime& Time_, double Value_)
			{
				auto Elapsed = double(duration_cast<milliseconds>(Time_ - _LastInsertedTime).count());
				if (Elapsed >= _Duration)
					_Value = _Value - _OneMinusEndWeight * _Value;
				else
					_Value = _Value - _OneMinusEndWeight * Elapsed * _Value / _Duration;

				_Value += Value_;

				_LastInsertedTime = Time_;
			}
			double get(void) const
			{
				return _Value;
			}
			void clear(void)
			{
				_Value = 0.0;
			}
		};
	}
}
