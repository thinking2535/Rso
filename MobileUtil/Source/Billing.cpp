#include "Billing.h"
#include <string>
#include <Rso/Crypto/Base64.h>


namespace rso
{
	namespace mobileutil
	{
		using namespace crypto;

		CBilling::CBilling(const string& PublicKey_)
		{
			_MD_CTX = std::shared_ptr<EVP_MD_CTX>(EVP_MD_CTX_create(), EVP_MD_CTX_destroy);
			OpenSSL_add_all_algorithms();

			_MD = EVP_get_digestbyname("SHA1");
			if (NULL == _MD)
				THROWEX();

			_PublicKey = PublicKey_;
		}
		int CBilling::Verify_GoogleInappBilling_Signature(const string& Data_, const string& Signiture_)
		{
			if (0 == EVP_VerifyInit_ex(_MD_CTX.get(), _MD, NULL))
				return -1;

			if (0 == EVP_VerifyUpdate(_MD_CTX.get(), (void*)Data_.c_str(), Data_.length()))
				return -1;

			auto b64 = std::shared_ptr<BIO>(BIO_new(BIO_f_base64()), BIO_free);
			BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);

			std::shared_ptr<BIO> bPubKey = std::shared_ptr<BIO>(BIO_new(BIO_s_mem()), BIO_free);
			BIO_puts(bPubKey.get(), _PublicKey.c_str());
			BIO_push(b64.get(), bPubKey.get());

			auto PublicKey = std::shared_ptr<EVP_PKEY>(d2i_PUBKEY_bio(b64.get(), NULL), EVP_PKEY_free);

			string decoded_signature = CBase64::Decode(Signiture_);
			return EVP_VerifyFinal(_MD_CTX.get(), (unsigned char*)decoded_signature.c_str(), (unsigned int)decoded_signature.length(), PublicKey.get());
		}
	}
}
