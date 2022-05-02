#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		using namespace base;

		template<typename _TData>
		class CChartPack
		{
		public:
			using TData = SDataPack<_TData>;

		private:
			using _TDatas = vector<TData>;
			using _TDatasIt = typename _TDatas::iterator;

			_TDatas	_Datas;

		public:
			inline const bool empty(void) const
			{
				return _Datas.empty();
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
			inline void clear(void)
			{
				_Datas.clear();
			}
			void emplace(time_t TimeT_, const _TData& Data_)
			{
				if (_Datas.empty() || _Datas.back().TimeT != TimeT_)
				{
					_Datas.emplace_back(TData(TimeT_, Data_, Data_, Data_, Data_));
				}
				else
				{
					if (Data_ > _Datas.back().High)
						_Datas.back().High = Data_;
					if (Data_ < _Datas.back().Low)
						_Datas.back().Low = Data_;

					_Datas.back().Close = Data_;
				}
			}
			typename CChartPack<_TData>::TData&	operator[](size_t Index_)
			{
				return _Datas[_Datas.size() - Index_ - 1];
			}
		};
	}
}
