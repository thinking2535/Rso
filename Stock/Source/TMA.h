#pragma once


#include "SMA.h"


namespace rso
{
	namespace stock
	{
		using namespace base;

		//template<typename _TData>
		//class CTMA
		//{
		//private:
		//	using _TRange = CSMA;
		//public:
		//	using TData = typename _TRange::TData;
		//private:
		//	using _TDatas = CQueue<TData>;
		//	using _TDatasIt = typename _TDatas::SIterator;

		//	size_t		_Count{ 0 };
		//	_TRange		_Range0;
		//	_TRange		_Range1;
		//	_TDatas		_Datas;

		//public:
		//	CTMA(size_t Count_) :
		//		_Count(Count_),
		//		_Range0(Count_ % 2 == 0 ? Count_ / 2 : Count_ / 2 + 1),
		//		_Range1(Count_ / 2 + 1)
		//	{
		//	}
		//	inline const time_t Count(void) const
		//	{
		//		return _Count;
		//	}
		//	inline const size_t size(void) const
		//	{
		//		return _Datas.size();
		//	}
		//	inline _TDatasIt begin(void)
		//	{
		//		return _Datas.begin();
		//	}
		//	inline _TDatasIt end(void)
		//	{
		//		return _Datas.end();
		//	}
		//	inline TData& front(void)
		//	{
		//		return _Datas.front();
		//	}
		//	inline TData& back(void)
		//	{
		//		return _Datas.back();
		//	}
		//	void	clear(void)
		//	{
		//		_Range0.clear();
		//		_Range1.clear();
		//		_Datas.clear();
		//	}
		//	void	emplace(time_t Time_, const _TData& Data_)
		//	{
		//		_Range0.emplace(Time_, Data_);
		//		_Range1.emplace(Time_, _Range0.Avg());
		//		_Datas.emplace(Time_, _Range1.Avg());
		//	}
		//	typename CTMA<_TData>::TData&	operator[](size_t Index_)
		//	{
		//		return _Datas[_Datas.size() - Index_ - 1];
		//	}
		//};
	}
}
