#pragma once


#include "Base.h"
#include <Rso/Base/Queue.h>


namespace rso
{
	namespace stock
	{
		using namespace base;

		template<typename TData>
		class CRangeDiff
		{
		public:
			using value_type = STimeTData<TData>;

		private:
			using _TDatas = CQueue<value_type>;

			time_t _Duration;
			_TDatas _Datas;

		public:
			CRangeDiff(time_t Duration_) :
				_Duration(Duration_)
			{
			};
			void push(const value_type& Data_)
			{
				_Datas.emplace(Data_);

				auto FromTimeT = Data_.TimeT - _Duration;

				for (auto it = _Datas.begin(); it != _Datas.end();)
				{
					auto itCheck = it;
					++it;

					if (itCheck->TimeT >= FromTimeT)
						break;

					_Datas.pop();
				}
			}
			inline size_t size(void) const
			{
				return _Datas.size();
			}
			inline const TData& front(void) const
			{
				return _Datas.front().Data;
			}
			inline const TData& back(void) const
			{
				return _Datas.back().Data;
			}
			inline void clear(void)
			{
				_Datas.clear();
			}
		};
	}
}
