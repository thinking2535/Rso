#pragma once


#include "Base.h"
#include <Rso/Base/Queue.h>


namespace rso
{
	namespace math
	{
		using namespace base;

		template<typename _TType>
		class CSumRange
		{
		private:
			struct _SData
			{
				time_t	TimeT{ 0 };
				_TType	Value;

				_SData(time_t TimeT_, const _TType& Value_) :
					TimeT(TimeT_), Value(Value_)
				{
				}
			};
			using _TDatas = CQueue<_SData>;

			_TDatas		_Datas;
			_TType		_Sum{ 0 };
			time_t		_Range{ 0 };

		public:
			CSumRange(time_t Range_) :
				_Range(Range_)
			{
			}

			_TType	emplace(time_t Time_, _TType Value_)
			{
				_Datas.emplace(_SData(Time_, Value_));

				_Sum += Value_;

				while (_Datas.size() > 1)
				{
					auto& Data = _Datas.front();
					if (Time_ - Data.TimeT < _Range)
						break;

					_Sum -= Data.Value;

					_Datas.pop();
				}

				return _Sum;
			}
			void	clear(void)
			{
				_Sum = 0;
				_Datas.clear();
			}
		};
	}
}