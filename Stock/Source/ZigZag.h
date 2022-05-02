#pragma once

#include "Base.h"


namespace rso
{
	namespace stock
	{
		using namespace std;

		// 데이터 적용시 SCandle의 High, Low 를 적용하지 않고 Close 만 사용하는 이유는 High, Low 의 차가 ZigZag 그래프 Gap 을 넘어서는 경우
		// High, Low 의 선후 관계를 알 수 없으므로 그래프를 그릴 수 없음.
		class CZigZag
		{
		public:
			struct SZigZag
			{
				size_t Index = 0;
				int32 Price = -1;

				SZigZag() {}
				SZigZag(size_t Index_, int32 Price_) :
					Index(Index_), Price(Price_) {}
			};

		private:
			const double _Gap = 0.0;
			int _Direction = 0; // 1 : up,  -1 : down,  0 : unknown
			SZigZag _FirstHigh;
			SZigZag _FirstLow;
			vector<SZigZag> _Datas;

		public:
			CZigZag(double Gap_) : _Gap(Gap_) {}
			bool insert(size_t Index_, const SCandle& Data_); // true : data changed
			inline size_t size(void) const { return _Datas.size(); }
			inline const SZigZag& at(size_t Index_) const { return _Datas.at(Index_); }
			inline const SZigZag& front(void) const { return _Datas.front(); }
			inline const SZigZag& back(void) const { return _Datas.back(); }
			inline bool empty(void) const { return _Datas.empty(); }
			void clear(void);
		};
	}
}
