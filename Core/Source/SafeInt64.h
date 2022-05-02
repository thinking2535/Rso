#pragma once

#include "Core.h"

namespace rso
{
	namespace core
	{
		class CSafeInt64
		{
			int64 _Data = 0;

		public:
			inline CSafeInt64() noexcept {}
			inline CSafeInt64(int64 Data_) noexcept { _Data = Data_; }
			inline operator int64() noexcept { return _Data; }
			CSafeInt64 operator += (int64 Data_) noexcept;
			CSafeInt64 operator *= (int64 Data_) noexcept;
			CSafeInt64 operator /= (int64 Data_) noexcept;
		};
	}
}
