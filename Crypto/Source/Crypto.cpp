#include "Crypto.h"

namespace rso
{
	namespace crypto
	{
		unsigned char CCrypto::_GetBit(void)
		{
			_Bit += _aKey[_Index];
			++_Index;
			_Index %= 8;

			return _Bit;
		}

		void CCrypto::_SetKey(uint64 Key_)
		{
			_Bit = 0;

			_aKey[0] = (((Key_) & 0xFF) == 0 ? 0xAA : ((Key_) & 0xFF) == 0xFF ? 0x55 : ((Key_) & 0xFF));
			_aKey[1] = (((Key_ >> 8) & 0xFF) == 0 ? 0xAA : ((Key_ >> 8) & 0xFF) == 0xFF ? 0x55 : ((Key_ >> 8) & 0xFF));
			_aKey[2] = (((Key_ >> 16) & 0xFF) == 0 ? 0xAA : ((Key_ >> 16) & 0xFF) == 0xFF ? 0x55 : ((Key_ >> 16) & 0xFF));
			_aKey[3] = (((Key_ >> 24) & 0xFF) == 0 ? 0xAA : ((Key_ >> 24) & 0xFF) == 0xFF ? 0x55 : ((Key_ >> 24) & 0xFF));
			_aKey[4] = (((Key_ >> 32) & 0xFF) == 0 ? 0xAA : ((Key_ >> 32) & 0xFF) == 0xFF ? 0x55 : ((Key_ >> 32) & 0xFF));
			_aKey[5] = (((Key_ >> 40) & 0xFF) == 0 ? 0xAA : ((Key_ >> 40) & 0xFF) == 0xFF ? 0x55 : ((Key_ >> 40) & 0xFF));
			_aKey[6] = (((Key_ >> 48) & 0xFF) == 0 ? 0xAA : ((Key_ >> 48) & 0xFF) == 0xFF ? 0x55 : ((Key_ >> 48) & 0xFF));
			_aKey[7] = (((Key_ >> 56) & 0xFF) == 0 ? 0xAA : ((Key_ >> 56) & 0xFF) == 0xFF ? 0x55 : ((Key_ >> 56) & 0xFF));

			_Index = 0;

			for (int32 i = 0; i < 8; ++i)
				_Bit ^= _aKey[i];
		}

		void CCrypto::Encode(unsigned char* pData_io, uint64 Size_, uint64 Key_)
		{
			_SetKey(Key_);

			for (uint64 i = 0; i < Size_; ++i)
				pData_io[i] ^= _GetBit();
		}

		void CCrypto::Decode(unsigned char* pData_io, uint64 Size_, uint64 Key_)
		{
			_SetKey(Key_);

			for (uint64 i = 0; i < Size_; ++i)
				pData_io[i] ^= _GetBit();
		}
	}
}
