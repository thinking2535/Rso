#pragma once

#include <Rso/Core/Core.h>

#pragma comment( lib, "Rso/Patch_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include "Protocol.h"
#include <Rso/Base/OptionStream.h>
#include <Rso/Net/Base.h>
#include <functional>

namespace rso
{
	using namespace base;
	using namespace net;

	namespace patch
	{
		using namespace std;
		using namespace placeholders;

		using TLoginFunc = function<void(const CKey& Key_, const SAmLogin& Proto_)>;

		bool operator == (const SVersion& Lhs_, const SVersion& Rhs_);
	}
}