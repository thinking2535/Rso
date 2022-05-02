#pragma once

#include "Binder.h"

namespace rso::net
{
	class CBinderServer : public CBinder
	{
	public:
		CBinderServer(CBinder&& Binder_, int BackLog_);
	};
}