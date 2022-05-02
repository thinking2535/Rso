#include "Base.h"

namespace rso
{
    namespace base
    {
		int32 GetAge(int32 Year_, int32 Month_, int32 Day_)
		{
			CDateTime Now(system_clock::now());

			if (CDateTime(SDateTime(
                static_cast<int16>(Year_),
                static_cast<uint16>(Month_),
                static_cast<uint16>(Day_),
                0, 0, 0)) >= Now)
				return 0;

			int32 Age = Now.year - Year_;

			if (Age <= 0)
				return 0;

			--Age;

            if (Now.month < Month_)
                return Age;
            else if (Now.month > Month_)
				return ++Age;
			else if (Now.day < Day_)
				return Age;
			else
				return ++Age;
		}
        int32 GetAgeWithSclNum(const string& SclNum_)
        {
            if (SclNum_.size() < 7)
                return 0;

            if (SclNum_[0] < '0' || SclNum_[0] > '9' ||
                SclNum_[1] < '0' || SclNum_[1] > '9' ||
                SclNum_[2] < '0' || SclNum_[2] > '9' ||
                SclNum_[3] < '0' || SclNum_[3] > '9' ||
                SclNum_[4] < '0' || SclNum_[4] > '9' ||
                SclNum_[5] < '0' || SclNum_[5] > '9') return 0;

            int32 BaseYear = 0;

            switch (SclNum_[6])
            {
            case '1':
            case '2':
            case '5':
            case '6':
                BaseYear = 1900;
                break;
            case '3':
            case '4':
            case '7':
            case '8':
                BaseYear = 2000;
                break;
            }

			return GetAge(
				BaseYear + (SclNum_[0] - '0') * 10 + (SclNum_[1] - '0'),
				(SclNum_[2] - '0') * 10 + (SclNum_[3] - '0'),
				(SclNum_[4] - '0') * 10 + (SclNum_[5] - '0'));
        }
    }
}
