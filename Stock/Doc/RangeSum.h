#pragma once


#include "Base.h"
#include <Rso/Base/Queue.h>


namespace rso
{
	namespace stock
	{
		using namespace base;

		template<typename _TData>
		class CRangeSum
		{
		public:
			using TData = SData<_TData>;

		private:
			using _TDatas = CQueue<TData>;
			using _TDatasIt = typename _TDatas::SIterator;

			time_t		_Range{ 0 };
			_TDatas		_Datas;
			_TData		_Sum{};

		public:

			CRangeSum(time_t Range_) :
				_Range(Range_)
			{
			}
			inline const time_t Range(void) const
			{
				return _Range;
			}
			inline const size_t size(void) const
			{
				return _Datas.size();
			}
			inline _TDatasIt begin(void)
			{
				return _Datas.begin();
			}
			inline _TDatasIt end(void)
			{
				return _Datas.end();
			}
			inline TData& front(void)
			{
				return _Datas.front();
			}
			inline TData& back(void)
			{
				return _Datas.back();
			}
			void	clear(void)
			{
				new(&_Sum) _TData();
				_Datas.clear();
			}
			void	emplace(time_t Time_, const _TData& Data_)
			{
				_Datas.emplace(Time_, Data_);

				_Sum += Data_;

				while (_Datas.size() > 1)
				{
					auto& Data = _Datas.front();
					if (Time_ - Data.TimeT < _Range)
						break;

					_Sum -= Data.Data;

					_Datas.pop();
				}
			}
			inline const _TData&	Sum(void) const
			{
				return _Sum;
			}
			inline _TData	Avg(void) const
			{
				return (_Sum / _Datas.size());
			}
		};
	}
}
