#pragma once

#include "Base.h"
#include <Rso/Base/Period.h>

namespace rso
{
	namespace mobileutil
	{
		using namespace base;

		class CFCMClient
		{
			string _Token;

		public:
			void SetToken(const string& Token_);
			string GetToken(void);
		};
	}
}