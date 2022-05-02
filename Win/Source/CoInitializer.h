#pragma once

#include "Base.h"

namespace rso
{
	namespace win
	{
		class CCoInitializer
		{
			HRESULT _Result = S_FALSE;

		public:
			CCoInitializer();
			CCoInitializer(CCoInitializer&& Var_);
			virtual ~CCoInitializer();
			CCoInitializer& operator = (CCoInitializer&& Var_);
		};
	}
}
