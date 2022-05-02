#pragma once


#include "ExtremeValue.h"


namespace rso
{
	namespace stock
	{
		class CHighPenetrate : public CExtremeValue<greater<TValue>>
		{
		};
	}
}
