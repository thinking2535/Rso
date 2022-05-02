// 정해진 주기 이상의 진폭을 추출

#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		using namespace base;

		class CTrendTime
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
			using _TDatas = vector<SData>;

			_TDatas		_Datas;
			time_t		_Range{ 0 };
			SData		_PrevData;

		public:
			CTrendTime(time_t Range_);
			void emplace(const SData& Data_);
			void clear(void);
			size_t size(void) const;
			inline time_t Range(void) const{ return _Range; }
			TValue get(void) const;
			inline const SData&	operator[] (size_t Index_) const
			{
				return _Datas[Index_];
			}
			inline _TDatas::iterator begin(void)
			{
				return _Datas.begin();
			}
			inline _TDatas::iterator end(void)
			{
				return _Datas.end();
			}
		};
	}
}
