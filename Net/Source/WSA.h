#pragma once

#include "Base.h"

namespace rso::net
{
	class CWSA
	{
		bool _Initialized = false;

	public:
		CWSA();
		CWSA(CWSA&& Var_);
		virtual ~CWSA();
		CWSA& operator = (CWSA&& Var_);
	};
}