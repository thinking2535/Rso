#pragma once

#include "Collider2D.h"

namespace rso::physics
{
	class CPlayerObject2D;

	class CMovingObject2D : public CObject2D
	{
	public:
		list<shared_ptr<CCollider2D>> Colliders;
		SPoint Velocity;
		float Mass = 1.0f;

		CMovingObject2D(const STransform& Transform_, const list<shared_ptr<CCollider2D>>& Colliders_, const SPoint& Velocity_);
		virtual CPlayerObject2D* GetPlayerObject2D(void);
	public:
		function<void(int64 tick)> fFixedUpdate;
	};
}
