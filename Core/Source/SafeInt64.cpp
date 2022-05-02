#include "SafeInt64.h"

namespace rso
{
	namespace core
	{
		CSafeInt64 CSafeInt64::operator += (int64 Data_) noexcept
		{
			if (Data_ > 0)
			{
				if (_Data + Data_ > _Data)
					_Data += Data_;
				else
					_Data = MaxValue<int64>();
			}
			else if (Data_ < 0)
			{
				if (_Data + Data_ < _Data)
					_Data += Data_;
				else
					_Data = MinValue<int64>();
			}

			return *this;
		}
		CSafeInt64 CSafeInt64::operator *= (int64 Data_) noexcept
		{
			if (_Data == 0)
				return *this;

			if (Data_ == 0)
			{
				_Data = 0;
				return *this;
			}

			int64 NewData = _Data * Data_;

			if (NewData / Data_ == _Data)
				_Data = NewData;
			else if (_Data < 0 && Data_ < 0 || _Data > 0 && Data_ > 0)
				_Data = MaxValue<int64>();
			else
				_Data = MinValue<int64>();

			return *this;
		}
		CSafeInt64 CSafeInt64::operator /= (int64 Data_) noexcept
		{
			_Data /= Data_;
			return *this;
		}
	}
}
