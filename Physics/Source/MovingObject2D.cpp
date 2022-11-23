#include "MovingObject2D.h"

namespace rso::physics
{
	CMovingObject2D::CMovingObject2D(const STransform& Transform_, const list<shared_ptr<CCollider2D>>& Colliders_, const SPoint& Velocity_) :
		CObject2D(Transform_),
		Colliders(Colliders_),
		Velocity(Velocity_)
	{
		for (auto& c : Colliders)
			c->SetParent(this);
	}
	bool CMovingObject2D::isPlayerObject2D()
	{
		return false;
	}
}
