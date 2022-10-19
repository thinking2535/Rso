#include "NetworkEngine.h"

namespace rso::physics
{
	CNetworkEngine::CNetworkEngine(int64 CurTick_, float ContactOffset_, int32 FPS_, int64 NetworkTickSync_) :
		CEngine(CurTick_, ContactOffset_, FPS_),
		_NetworkTickSync(NetworkTickSync_)
	{
	}
}