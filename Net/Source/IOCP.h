#pragma once

#include "Base.h"

namespace rso::net
{
	class CIOCP
	{
		size_t _ThreadCnt = 0;
		HANDLE _IOCP = NULL;

	public:
		CIOCP(size_t ThreadCnt_);
		CIOCP(CIOCP&& Var_);
		virtual ~CIOCP();
		CIOCP& operator = (CIOCP&& Var_);
		HANDLE IOCP(void) const { return _IOCP; }
	};
}