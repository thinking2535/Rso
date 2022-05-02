#pragma once


#include "CountSum.h"


namespace rso
{
	namespace stock
	{
		using namespace base;

		template<typename _TData>
		class CSMA
		{
		private:
			using _TRange = CCountSum<_TData>;
		public:
			using TData = typename _TRange::TData;
		private:
			using _TDatas = CQueue<TData>;
			using _TDatasIt = typename _TDatas::SIterator;

			_TRange		_Range;
			_TDatas		_Datas;

		public:
			CSMA(size_t Count_) :
				_Range(Count_)
			{
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
				__super::clear();
				_Datas.clear();
			}
			void	emplace(time_t Time_, const _TData& Data_)
			{
				_Range.emplace(Time_, Data_);
				_Datas.emplace(Time_, _Range.Avg());
			}
			typename CSMA<_TData>::TData&	operator[](size_t Index_)
			{
				return _Datas[_Datas.size() - Index_ - 1];
			}
		};
	}
}
