#pragma once

#include "TickMerge.h"

namespace rso
{
	namespace stock
	{
		using namespace std;

		// [0] 가장 최신 이며 _Datas의 인덱스가 증가하는 방향으로 과거 데이터를 저장
		class CChart : protected CTickMerge
		{
		public:
			using TSizes = vector<size_t>;

		private:
			TSizes _Sizes;
			TSizes _Indices;
			vector<SChart> _Chart; // 축약
			vector<SChartTime> _Datas; // Raw Data
			size_t _StartIndex = 0;
			size_t _Size = 0;

			size_t _Index(size_t Index_) const;
			optional<SChartTime> _Add(const SChartTime& Data_);

		public:
			using CTickMerge::CurData;

			CChart(const TSizes& Sizes_);
			optional<SChartTime> Insert(const SChartTime& Data_); // return true : Full
			inline bool Full(void) const noexcept { return _Size == _Datas.size(); }
			inline size_t ChartSize(void) const noexcept { return _Chart.size(); }
			inline size_t DataSize(void) const noexcept { return _Datas.size(); }
			inline const SChartTime& GetData(size_t Index_) const noexcept { return _Datas[_Index(Index_)]; }
			inline double GetPrice(size_t Index_) const noexcept { return (double)_Chart[Index_].Price / (double)_Sizes[Index_]; }
			inline double GetVolume(size_t Index_) const noexcept { return (double)_Chart[Index_].Volume; }
		};
	}
}
