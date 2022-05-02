#pragma once

#include "Base.h"
#include <optional>

namespace rso
{
	namespace stock
	{
		using namespace std;

		// 틱 데이터를 1초 단위로 묶어서 반환
		class CTickMerge
		{
		private:
			SChartTime _CurData; // 1초단위로 집계
			__int64 _LastSeconds = 0;

		public:
			optional<SChartTime> Insert(const SChartTime& Data_);
			inline const SChartTime& CurData(void) const noexcept { return _CurData; }
		};
	}
}
