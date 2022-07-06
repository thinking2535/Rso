#include "PlayerObject2D.h"

namespace rso::physics
{
	CPlayerObject2D::CPlayerObject2D(const STransform& Transform_, const SPoint& Velocity_) :
		CMovingObject2D(Transform_, Velocity_)
	{
	}
	CPlayerObject2D* CPlayerObject2D::GetPlayerObject2D(void)
	{
		return this;
	}
	void CPlayerObject2D::Overlapped(int64 Tick_, const SPoint& Normal_, const shared_ptr<CCollider2D>& pCollider_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_)
	{
		if (_ContactPoint2Ds.emplace(SContactPoint2D(pCollider_, pOtherCollider_), pOtherMovingObject_).second)
		{
			if (fCollisionEnter)
				fCollisionEnter(Tick_, Normal_, pCollider_, pOtherCollider_, pOtherMovingObject_);
		}
		else
		{
			if (fCollisionStay)
				fCollisionStay(Tick_, Normal_, pCollider_, pOtherCollider_, pOtherMovingObject_);
		}
	}
	void CPlayerObject2D::NotOverlapped(int64 Tick_, const shared_ptr<CCollider2D>& pCollider_, const shared_ptr<CCollider2D>& pOtherCollider_)
	{
		auto it = _ContactPoint2Ds.find(SContactPoint2D(pCollider_, pOtherCollider_));
		if (it != _ContactPoint2Ds.end())
		{
			if (fCollisionExit)
				fCollisionExit(Tick_, pCollider_, pOtherCollider_, it->second);

			_ContactPoint2Ds.erase(it);
		}
	}
	void CPlayerObject2D::OverlappedCheck(int64 Tick_, const shared_ptr<CMovingObject2D>& pThisMovingObject_, const shared_ptr<CCollider2D>& pOtherCollider_)
	{
		pCollider->OverlappedCheck(Tick_, pCollider, pThisMovingObject_, pOtherCollider_, nullptr);
	}
	void CPlayerObject2D::OverlappedCheck(int64 Tick_, const shared_ptr<CMovingObject2D>& pThisMovingObject_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_)
	{
		pCollider->OverlappedCheck(Tick_, pCollider, pThisMovingObject_, pOtherMovingObject_->pCollider, pOtherMovingObject_);
	}
}
