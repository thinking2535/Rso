// 정해진 구간에서 High, Low 를 추출

#pragma once


#include "Base.h"
#include <Rso/Base/Queue.h>


namespace rso
{
	namespace stock
	{
		using namespace base;

		class CTrendRange
		{
		private:
			struct _SData
			{
				bool	Valid{ false };
				size_t	Index{ 0 };
				time_t	TimeT{ 0 };
				TValue	Value{ 0 };

				_SData(TValue Value_) :
					Value(Value_)
				{}
				_SData(size_t Index_, time_t TimeT_, TValue Value_) :
					Valid(true), Index(Index_), TimeT(TimeT_), Value(Value_)
				{}
			};

			using _TDatas = CQueue<_SData>;

			size_t		_Counter{ 0 };
			_TDatas		_Datas;
			time_t		_Range{ 0 };
			_SData		_High{ 0, 0, MinValue<TValue>() };
			_SData		_Low{ 0, 0, MaxValue<TValue>() };

		public:
			CTrendRange(time_t Range_);
			TValue		emplace(time_t TimeT_, TValue Value_);
			void		clear(void);
			const _SData&		front(void) const { return _Datas.front(); }
			const _SData&		back(void) const { return _Datas.back(); }
			inline size_t	Size(void) const { return _Datas.size(); }
			inline const _SData& High(void) const { return _High; }
			inline const _SData& Low(void) const { return _Low; }
			inline time_t	Range(void) const { return _Range; }
		};
	}
}
