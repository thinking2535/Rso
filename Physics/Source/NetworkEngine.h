#pragma once

#include "Engine.h"

namespace rso::physics
{
	class CNetworkEngine : public CEngine
	{
	protected:
		int64 _NetworkTickSync;

	public:
		CNetworkEngine(int64 CurTick_, float ContactOffset_, int32 FPS_, int64 NetworkTickSync_);
	};
}