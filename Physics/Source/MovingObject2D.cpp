#include "MovingObject2D.h"

namespace rso::physics
{
	CMovingObject2D::CMovingObject2D(const STransform& Transform_, const SPoint& Velocity_) :
		CObject2D(Transform_),
		Velocity(Velocity_)
	{
	}
	CPlayerObject2D* CMovingObject2D::GetPlayerObject2D(void)
	{
		return nullptr;
	}

	void SetColliderToMovingObject2D(const shared_ptr<CCollider2D>& pCollider_, const shared_ptr<CMovingObject2D>& MovingObject_)
	{
		MovingObject_->pCollider = pCollider_;
		pCollider_->SetParent(MovingObject_);
	}
}
