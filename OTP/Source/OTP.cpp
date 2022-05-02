#pragma once

#include "OTP.h"

namespace rso
{
	namespace otp
	{
		COTP::_TValue COTP::_Get(_TValue Minutes_) const
		{
			_TValue Number = Minutes_;

			// Minutes �� ���� �ڸ����� 0 �� ���� ä��� ���� Minutes �� ��ȿ�� ���� �� Bit ���� ����.
			_TValue Buffer = 0; // For Valid Bit Count

			while (Number != 0)
			{
				++Buffer;
				Number >>= 1;
			}

			// Minutes �� ���� �ڸ����� 0 �ΰ��� ��ȿ�� Minutes �� ��ȿ�� ������ ä��
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

			// Minutes_ �� ����ŭ Shift �Ͽ� Minutes_ �� ������ų Mask �� ����
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

			// �ð����� Minutes �� ���� ���� �� �����Ƿ� �� �ڷ� ������ OK
			if (Value_ == _Get(Minutes) ||
				Value_ == _Get(Minutes + 1) ||
				Value_ == _Get(Minutes - 1))
				return true;

			return false;
		}
	}
}
