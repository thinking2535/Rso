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
		if (_ContactPoint2Ds.emplace(SContactPoint2D(pCollider_, pOtherCollider_), SNormalOtherMovingObject(Normal_, pOtherMovingObject_)).second)
		{
			if (fCollisionEnter)
				fCollisionEnter(Tick_, Normal_, pCollider_, pOtherCollider_, pOtherMovingObject_);
		}
	}
	void CPlayerObject2D::CollisionEnterCheck(int64 Tick_, const shared_ptr<CMovingObject2D>& pThisMovingObject_, const shared_ptr<CCollider2D>& pOtherCollider_)
	{
		pCollider->OverlappedCheck(Tick_, pCollider, pThisMovingObject_, pOtherCollider_, nullptr);
	}
	void CPlayerObject2D::CollisionEnterCheck(int64 Tick_, const shared_ptr<CMovingObject2D>& pThisMovingObject_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_)
	{
		pCollider->OverlappedCheck(Tick_, pCollider, pThisMovingObject_, pOtherMovingObject_->pCollider, pOtherMovingObject_);
	}
	void CPlayerObject2D::CollisionStayCheck(int64 Tick_)
	{
		if (fCollisionStay)
			fCollisionStay(Tick_, _ContactPoint2Ds);
	}
	void CPlayerObject2D::CollisionExitCheck(int64 Tick_)
	{
		for (auto it = _ContactPoint2Ds.begin(); it != _ContactPoint2Ds.end();)
		{
			auto itCheck = it;
			++it;

			if (!itCheck->first.pCollider->IsOverlapped(Tick_, itCheck->first.pOtherCollider.get()))
			{
				auto Key = itCheck->first;
				auto Value = itCheck->second;

				_ContactPoint2Ds.erase(itCheck);

				if (fCollisionExit)
					fCollisionExit(Tick_, Value.Normal, Key.pCollider, Key.pOtherCollider, Value.pOtherMovingObject);
			}
		}
	}
}
