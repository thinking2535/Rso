#include "FCMClient.h"

namespace rso
{
	namespace mobileutil
	{
		void CFCMClient::SetToken(const string& Token_)
		{
			_Token = Token_;
		}
		string CFCMClient::GetToken(void)
		{
			return std::move(_Token);
		}
	}
}