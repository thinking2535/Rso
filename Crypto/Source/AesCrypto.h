#pragma once

#include "Base.h"
#include "AES.h"

namespace rso
{
	namespace crypto
	{
		class CAesCrypto final
		{
		public:
			enum EBit
			{
				Bit_128 = 128,
				Bit_192 = 192,
				Bit_256 = 256,
			};

		private:
			uint32	m_KeySize;
			aes_context		m_DecCTX;
			aes_context		m_EncCTX;

		public:
			CAesCrypto(EBit Bits_) : m_KeySize(Bits_) {}
			virtual ~CAesCrypto() {}

			bool	SetKey(unsigned char aKey_[32]);
			void	Encrypt(const unsigned char* pBuf_, std::size_t Size_, unsigned char* pBuf_o);
			void	Decrypt(const unsigned char* pBuf_, std::size_t Size_, unsigned char* pBuf_o);
		};
	}
}
