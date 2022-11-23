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
		bool isKinematic = false;

		CMovingObject2D(const STransform& Transform_, const list<shared_ptr<CCollider2D>>& Colliders_, const SPoint& Velocity_);
		virtual bool isPlayerObject2D();
		virtual bool Collided(int64 /*tick*/, const SCollision2D& /*Collision_*/)
		{
			return false;
		}
		virtual bool Triggered(int64 /*tick*/, CCollider2D* /*pCollider_*/, CCollider2D* /*pOtherCollider_*/, CMovingObject2D* /*pOtherMovingObject_*/)
		{
			return false;
		}
		virtual void NotOverlapped(int64 /*tick*/, CCollider2D* /*pCollider_*/, CCollider2D* /*pOtherCollider_*/)
		{
		}
		virtual void NotOverlapped(int64 /*tick*/, CCollider2D* /*pOtherCollider_*/)
		{
		}
	public:
		function<void(int64 tick)> fFixedUpdate;
	};
}
