#pragma once

#include <Rso/Core/Base.h>
#include "Protocol.h"
#include <Rso/Base/List.h>

#pragma comment( lib, "Rso/Mobile_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

namespace rso::mobile
{
	using namespace base;

	using TSendProtos = CListB<CStream>;
}
