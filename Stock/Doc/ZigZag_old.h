#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		using namespace base;

		template<typename _TData>
		class CZigZag
		{
		public:
			using TData = SData<_TData>;

		private:
			using _TDatas = vector<TData>;
			using _TDatasIt = typename _TDatas::iterator;

			_TDatas	_Datas;
			TValue	_MinAmp{ 0 };

		public:
			CZigZag(TValue MinAmp_) :
				_MinAmp(MinAmp_)
			{
			}
			__inline void	clear(void)
			{
				_Datas.clear();
			}
			__inline const TData& operator[] (size_t Index_) const
			{
				return _Datas[Index_];
			}
			__inline const TData& rat(size_t Index_) const // reverse at
			{
				return _Datas[_Datas.size() - Index_ - 1];
			}
			__inline _TDatasIt begin(void)
			{
				return _Datas.begin();
			}
			__inline _TDatasIt end(void)
			{
				return _Datas.end();
			}
			__inline size_t	size(void) const
			{
				return _Datas.size();
			}
			__inline TValue	MinAmp(void) const
			{
				return _MinAmp;
			}
			void emplace(const TData& Data_)
			{
				if (_Datas.size() == 0)
				{
					_Datas.emplace_back(Data_);
				}
				else if (_Datas.size() == 1)
				{
					if (_Datas.back().Data != Data_.Data)
						_Datas.emplace_back(Data_);
				}
				else if (_Datas.size() == 2 &&
					_Datas[_Datas.size() - 1].Data - _Datas[_Datas.size() - 2].Data <= _MinAmp &&
					_Datas[_Datas.size() - 1].Data - _Datas[_Datas.size() - 2].Data >= -_MinAmp)
				{
					// 두 점이 상승중이면
					if (_Datas[_Datas.size() - 1].Data - _Datas[_Datas.size() - 2].Data > 0)
					{
						// 위 점을 뚫었으면
						if (_Datas[_Datas.size() - 1].Data < Data_.Data)
						{
							_Datas[_Datas.size() - 1] = Data_;
						}
						else if (Data_.Data - _Datas[_Datas.size() - 1].Data < -_MinAmp)
						{
							_Datas.emplace_back(Data_);
						}
						else if (_Datas[_Datas.size() - 2].Data >= Data_.Data)
						{
							_Datas[_Datas.size() - 2] = _Datas[_Datas.size() - 1];
							_Datas[_Datas.size() - 1] = Data_;
						}
					}
					else
					{
						// 아래 점을 뚫었으면
						if (_Datas[_Datas.size() - 1].Data > Data_.Data)
						{
							_Datas[_Datas.size() - 1] = Data_;
						}
						else if (Data_.Data - _Datas[_Datas.size() - 1].Data > _MinAmp)
						{
							_Datas.emplace_back(Data_);
						}
						else if (_Datas[_Datas.size() - 2].Data <= Data_.Data)
						{
							_Datas[_Datas.size() - 2] = _Datas[_Datas.size() - 1];
							_Datas[_Datas.size() - 1] = Data_;
						}
					}
				}
				else
				{
					// 두 점이 상승중이면
					if (_Datas[_Datas.size() - 1].Data - _Datas[_Datas.size() - 2].Data > 0)
					{
						// 위점 뚫었으면
						if (_Datas[_Datas.size() - 1].Data < Data_.Data)
							_Datas[_Datas.size() - 1] = Data_;
						// Data_ - 마지막 점 이 -기준 미만이면
						else if (Data_.Data - _Datas[_Datas.size() - 1].Data < -_MinAmp)
							_Datas.emplace_back(Data_);
					}
					else
					{
						// 아래점 뚫었으면
						if (_Datas[_Datas.size() - 1].Data > Data_.Data)
							_Datas[_Datas.size() - 1] = Data_;
						// Data_ - 마지막 점이 기준 초과면
						else if (Data_.Data - _Datas[_Datas.size() - 1].Data > _MinAmp)
							_Datas.emplace_back(Data_);
					}
				}
			}
			TValue RTrend(size_t Index_) const
			{
				if (_Datas.size() < 2 + Index_)
					return 0;

				auto Trend = _Datas[_Datas.size() - Index_ - 1].Data - _Datas[_Datas.size() - Index_ - 2].Data;
				if (Trend < -_MinAmp || Trend > _MinAmp)
					return Trend;
				else
					return 0;
			}
		};
	}
}
