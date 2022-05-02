#pragma once

#include "Base.h"
#include <optional>

namespace rso
{
	namespace stock
	{
		using namespace std;

		// ƽ �����͸� 1�� ������ ��� ��ȯ
		class CTickMerge
		{
		private:
			SChartTime _CurData; // 1�ʴ����� ����
			__int64 _LastSeconds = 0;

		public:
			optional<SChartTime> Insert(const SChartTime& Data_);
			inline const SChartTime& CurData(void) const noexcept { return _CurData; }
		};
	}
}
