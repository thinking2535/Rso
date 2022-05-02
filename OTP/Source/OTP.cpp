#pragma once

#include "OTP.h"

namespace rso
{
	namespace otp
	{
		COTP::_TValue COTP::_Get(_TValue Minutes_) const
		{
			_TValue Number = Minutes_;

			// Minutes 의 상위 자리수중 0 인 것을 채우기 위해 Minutes 의 유효한 값의 총 Bit 수를 얻음.
			_TValue Buffer = 0; // For Valid Bit Count

			while (Number != 0)
			{
				++Buffer;
				Number >>= 1;
			}

			// Minutes 의 상위 자리수중 0 인것을 유효한 Minutes 의 유효한 값으로 채움
			for (_TValue i = 0; i < 64; i += Buffer)
			{
				Number += Minutes_;
				Number <<= Buffer;
			}

			Minutes_ = Number;

			Buffer = 0; // For Shift

			while (Number != 0)
			{
				Buffer ^= (Number % 64);
				Number /= 64;
			}

			// Minutes_ 의 값만큼 Shift 하여 Minutes_ 를 변조시킬 Mask 를 생성
			Number = Minutes_ ^ ((_Seed << Buffer) + (_Seed >> (64 - Buffer)));

			Buffer = 0;

			while (Number != 0)
			{
				Buffer ^= (Number % 1000000);
				Number /= 1000000;
			}

			return (Buffer % 1000000);
		}
		COTP::COTP(_TValue Seed_) :
			_Seed(Seed_)
		{
		}
		bool COTP::IsValid(_TValue Value_) const
		{
			auto Minutes = _GetMinutes();

			// 시간차로 Minutes 가 차이 있을 수 있으므로 앞 뒤로 같으면 OK
			if (Value_ == _Get(Minutes) ||
				Value_ == _Get(Minutes + 1) ||
				Value_ == _Get(Minutes - 1))
				return true;

			return false;
		}
	}
}
