#pragma once


#include "ExtremeValue.h"


namespace rso
{
	namespace stock
	{
		class CLowPenetrate : public CExtremeValue<less<TValue>>
		{
		};
	}
}
