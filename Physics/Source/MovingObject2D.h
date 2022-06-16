#pragma once

#include "Collider2D.h"

namespace rso::physics
{
	class CPlayerObject2D;

	class CMovingObject2D : public CObject2D
	{
	public:
		using FFixedUpdate = function<void(int64 Tick_)>;
		shared_ptr<CCollider2D> pCollider;
		SPoint Velocity;

		CMovingObject2D(const STransform& Transform_, const SPoint& Velocity_);
		virtual CPlayerObject2D* GetPlayerObject2D(void);
	public:
		FFixedUpdate fFixedUpdate;
	};

	void SetColliderToMovingObject2D(const shared_ptr<CCollider2D>& pCollider_, const shared_ptr<CMovingObject2D>& MovingObject_);
}
