#pragma once


#include "RangeSum.h"


namespace rso
{
	namespace stock
	{
		using namespace base;

		template<typename _TData>
		class CTMA2
		{
		private:
			using _TRange = CRangeSum<_TData>;
		public:
			using TData = typename _TRange::TData;
		private:
			using _TDatas = CQueue<TData>;
			using _TDatasIt = typename _TDatas::SIterator;

			time_t		_Range{ 0 };
			_TRange		_Range0;
			_TRange		_Range1;
			_TDatas		_Datas;

		public:
			CTMA2(time_t Range_) :
				_Range(Range_),
				_Range0(_Range % 2 == 0 ? Range_ / 2 : Range_ / 2 + 1),
				_Range1(Range_ / 2 + 1)
			{
			}
			__inline const time_t Range(void) const
			{
				return _Range;
			}
			__inline const size_t size(void) const
			{
				return _Datas.size();
			}
			__inline _TDatasIt begin(void)
			{
				return _Datas.begin();
			}
			__inline _TDatasIt end(void)
			{
				return _Datas.end();
			}
			__inline TData& front(void)
			{
				return _Datas.front();
			}
			__inline TData& back(void)
			{
				return _Datas.back();
			}
			void	clear(void)
			{
				_Range0.clear();
				_Range1.clear();
				_Datas.clear();
			}
			void	emplace(time_t Time_, const _TData& Data_)
			{
				_Range0.emplace(Time_, Data_);
				_Range1.emplace(Time_, _Range0.Avg());
				_Datas.emplace(Time_, _Range1.Avg());
			}
			typename CTMA2<_TData>::TData&	operator[](size_t Index_)
			{
				return _Datas[_Datas.size() - Index_ - 1];
			}
			typename CTMA2<_TData>::TData&	get(time_t Time_)
			{
				for (size_t i = 0; i < _Datas.size(); ++i)
				{
					if (_Datas[_Datas.size() - i - 1].TimeT < Time_)
						return _Datas[_Datas.size() - i - 1];
				}

				return _Datas.front();
			}
		};
	}
}
