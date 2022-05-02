#pragma once


#include "Base.h"
#include <Rso/Base/Queue.h>


namespace rso
{
	namespace stock
	{
		template<typename TDuration>
		class CRangeWeightSum
		{
		public:
			using value_type = STimeTData<double>;

		private:
			using _TDatas = CQueue<value_type>;

			TDuration _Duration;
			_TDatas _Datas;

		public:
			CRangeWeightSum(const TDuration& Duration_) :
				_Duration(Duration_)
			{
			}
			void push(const TTime& Time_, double Value_)
			{
				_Datas.emplace(value_type(Time_, Value_));

				auto FromTime = Time_ - _Duration;
				for (auto it = _Datas.begin(); it != _Datas.end(); it = _Datas.begin())
				{
					if (it->Time >= FromTime)
						break;

					_Datas.pop();
				}
			}
			double get(void) const
			{
				if (_Datas.empty())
					return 0.0;

				double Sum = 0.0;

				auto BeginTime = _Datas.back().Time - _Duration;
				for (auto& it : _Datas)
					Sum += (it.Data * static_cast<double>(duration_cast<TDuration>(it.Time - BeginTime).count()));

				return (Sum / static_cast<double>(_Duration.count()));
			}
			void clear(void)
			{
				_Datas.clear();
			}
		};
	}
}
