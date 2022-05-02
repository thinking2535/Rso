// ������ �ֱ�, size���� ������, ������ �߼����� �̿��� �߼� ���� ��ȯ ( ..., -2, -1, 0, 1, 2, ... ) ���밪�� Ŭ���� ���� �߼�

#pragma once


#include "Base.h"
#include <Rso/Base/Queue.h>


namespace rso
{
	namespace stock
	{
		using namespace base;

		class CTrendLine
		{
		public:
			struct SData
			{
				time_t	TimeT{ 0 };
				TValue	Value{ 0 };

				SData() {}
				SData(time_t TimeT_, TValue Value_) :
					TimeT(TimeT_), Value(Value_)
				{}
			};

		private:
			using _TDatas = CQueue<SData>;

			_TDatas		_Datas;
			TValue		_MinAmp{ 0 };
			TValue		_Trend{ 0 };

			void		_TrendUp(void);
			void		_TrendDown(void);

		public:
			CTrendLine(TValue MinAmp_) :
				_MinAmp(MinAmp_)
			{

			}
			void		emplace(const SData& Data_);
			TValue		Trend(void) const;
			inline void		clear(void) { _Datas.clear(); }
		};
	}
}
