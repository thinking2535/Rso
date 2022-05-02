#pragma once

#include <Rso/Core/Base.h>
#pragma comment( lib, "Rso/Ranking_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include "Protocol.h"

namespace rso::ranking
{
	template<typename _TSP> struct SDBBinder {};
	template<> struct SDBBinder<SConfigLoadDBIn> { static const EProtoDB SpNum = EProtoDB::ConfigLoad; };
	template<> struct SDBBinder<SRefreshDBIn> { static const EProtoDB SpNum = EProtoDB::Refresh; };
	template<> struct SDBBinder<SUpdateDBIn> { static const EProtoDB SpNum = EProtoDB::Update; };
}
