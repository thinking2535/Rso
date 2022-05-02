#pragma once


#include "Base.h"
#include <Rso/Base/Queue.h>


namespace rso
{
	namespace stock
	{
		using namespace base;

		template<typename TDuration, typename TValue>
		class CStochastic
		{
			struct _SData
			{
				TTime Time;
				TValue Value;
			};
			using TDatas = CQueue<_SData>;

			TDuration _Duration;
			TDatas _Datas;

		public:
			CStochastic(const TDuration& Duration_, const TValue& Value_) :
				_Duration(Duration_), _Value(Value_)
			{
			}
			void push(const TTime& Time_, const TValue& Value_)
			{
				auto BeginTime = Time_ - _Duration;
				for (auto it = _Datas.begin(); it != _Datas.end(); it = _Datas.begin())
				{
					if (it->Time > BeginTime)
						break;

					_Datas.pop();
				}

				_Datas.emplace(_SData{ Time_, Value_ });
			}
			double get(void) const
			{
				auto Begin = _Datas.begin();

				ÀÛ¾÷Áß
				
				TValue Min
			}
			void clear(void)
			{
				_Datas.clear();
			}
		};
	}
}
