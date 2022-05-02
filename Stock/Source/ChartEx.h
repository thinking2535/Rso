// 정해진 구간에서의 정해진 진폭 이상을 추출

#pragma once


#include "Base.h"
#include <Rso/Base/Queue.h>


namespace rso
{
	namespace stock
	{
		using namespace base;

		class CChartEx
		{
		public:
			struct SData
			{
				time_t	TimeT{ 0 };
				TValue	Value{ 0 };

				SData()
				{
				}
				SData(time_t TimeT_, TValue Value_) :
					TimeT(TimeT_), Value(Value_)
				{
				}
			};

		private:
			using _TDatas = CQueue<SData>;

			_TDatas	_Datas;
			time_t	_Range{ 0 };
			TValue	_MinAmp{ 0 };
			SData	_LastData;

		public:
			CChartEx(time_t Range_, TValue MinAmp_) :
				_Range(Range_), _MinAmp(MinAmp_)
			{
			}
			void	emplace(const SData& Value_);
			void	clear(void);
			TValue	Trend(size_t Index_);
			SData	operator[] (size_t Index_)
			{
				return _Datas[_Datas.size() - Index_ - 1];
			}
			_TDatas::iterator begin(void)
			{
				return _Datas.begin();
			}
			_TDatas::iterator end(void)
			{
				return _Datas.end();
			}
			size_t	size(void) const
			{
				return _Datas.size();
			}
			TValue	MinAmp(void) const
			{
				return _MinAmp;
			}
		};
	}
}
