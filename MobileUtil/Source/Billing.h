#pragma once

#include "Base.h"
#pragma comment( lib, "openssl/libeay32.lib" )

#include <openssl/evp.h>
#include <openssl/x509.h>

namespace rso
{
	namespace mobileutil
	{
		using namespace std;

		class CBilling
		{
		private:
			std::shared_ptr<EVP_MD_CTX> _MD_CTX;
			const EVP_MD* _MD;
			string _PublicKey;

		public:
			CBilling(const string& PublicKey_);
			int Verify_GoogleInappBilling_Signature(const string& Data_, const string& Signiture_);
		};
	}
}
