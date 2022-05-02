#include "AesCrypto.h"

namespace rso
{
	namespace crypto
	{
		bool	CAesCrypto::SetKey(unsigned char aKey_[32])
		{
			if (aes_setkey_dec(&m_DecCTX, aKey_, m_KeySize) < 0)
				return false;

			if (aes_setkey_enc(&m_EncCTX, aKey_, m_KeySize) < 0)
				return false;

			return true;
		}

		void	CAesCrypto::Encrypt(const unsigned char* pBuf_, std::size_t Size_, unsigned char* pBuf_o)
		{
			for (std::size_t i = 0;
				(i + 16) <= Size_;
				i += 16)
			{
				aes_crypt_ecb(&m_DecCTX, 0, &pBuf_[i], &pBuf_o[i]);
			}
		}

		void	CAesCrypto::Decrypt(const unsigned char* pBuf_, std::size_t Size_, unsigned char* pBuf_o)
		{
			for (std::size_t i = 0;
				(i + 16) <= Size_;
				i += 16)
			{
				aes_crypt_ecb(&m_EncCTX, 1, &pBuf_[i], &pBuf_o[i]);
			}
		}
	}
}
