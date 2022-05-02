// 전체 구간에서 High, Low 를 추출

#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		class CTrend
		{
		private:
			using _TDatas = vector<TValue>;

			_TDatas		_Datas;
			TValue		_High = MinValue<TValue>();
			TValue		_Low = MaxValue<TValue>();

		public:
			TValue		emplace(TValue Value_);
			void		clear(void);
			TValue		front(void) const { return _Datas.front(); }
			TValue		back(void) const { return _Datas.back(); }
			inline size_t	Size(void) const { return _Datas.size(); }
			inline TValue High(void) const { return _High; }
			inline TValue Low(void) const { return _Low; }
		};
	}
}
