#include "PlayerObject2D.h"

namespace rso::physics
{
	CPlayerObject2D::CPlayerObject2D(const STransform& Transform_, const list<shared_ptr<CCollider2D>>& Colliders_, const SPoint& Velocity_) :
		CMovingObject2D(Transform_, Colliders_, Velocity_)
	{
	}
	bool CPlayerObject2D::isPlayerObject2D()
	{
		return true;
	}
	bool CPlayerObject2D::CheckOverlapped(int64 tick, CCollider2D* pOtherCollider_)
	{
		for (auto& c : Colliders)
		{
			if (c->checkOverlapped(tick, this, pOtherCollider_))
				return true;
		}

		return false;
	}
	bool CPlayerObject2D::CheckOverlapped(int64 tick, CMovingObject2D* pOtherMovingObject_)
	{
		for (auto& c : Colliders)
		{
			for (auto& o : pOtherMovingObject_->Colliders)
			{
				if (c->checkOverlapped(tick, this, o.get(), pOtherMovingObject_))
					return true;
			}
		}

		return false;
	}
	bool CPlayerObject2D::Collided(int64 tick, const SCollision2D& Collision_)
	{
		if (_ContactPoint2Ds.emplace(SContactPoint2D(Collision_.pCollider, Collision_.pOtherCollider), Collision_.pOtherMovingObject).second)
		{
			if (fCollisionEnter)
				return fCollisionEnter(tick, Collision_);
			else
				return false;
		}

		if (fCollisionStay)
			return fCollisionStay(tick, Collision_);
		else
			return false;
	}
	bool CPlayerObject2D::Triggered(int64 tick, CCollider2D* pCollider_, CCollider2D* pOtherCollider_, CMovingObject2D* pOtherMovingObject_)
	{
		if (_ContactPoint2Ds.emplace(SContactPoint2D(pCollider_, pOtherCollider_), pOtherMovingObject_).second)
		{
			if (fTriggerEnter)
				return fTriggerEnter(tick, pOtherCollider_);
			else
				return false;
		}

		if (fTriggerStay)
			return fTriggerStay(tick, pOtherCollider_);
		else
			return false;
	}
	void CPlayerObject2D::NotOverlapped(int64 tick, CCollider2D* const pCollider_, CCollider2D* const pOtherCollider_)
	{
		auto it = _ContactPoint2Ds.find(SContactPoint2D(pCollider_, pOtherCollider_));
		if (it != _ContactPoint2Ds.end())
		{
			auto movingObject = it->second;

			_ContactPoint2Ds.erase(it);

			if (pCollider_->IsTrigger || pOtherCollider_->IsTrigger)
			{
				if (fTriggerExit)
					fTriggerExit(tick, pOtherCollider_);
			}
			else
			{
				if (fCollisionExit)
					fCollisionExit(tick, SCollision2D(SPoint(), SPoint(), pCollider_, pOtherCollider_, movingObject));
			}
		}
	}
	void CPlayerObject2D::NotOverlapped(int64 tick, CCollider2D* pOtherCollider_)
	{
		for (auto it = _ContactPoint2Ds.begin(); it != _ContactPoint2Ds.end(); )
		{
			auto itCheck = it;
			++it;

			if (itCheck->first.pOtherCollider != pOtherCollider_)
				continue;

			auto contactPoint = itCheck->first;
			auto movingObject = itCheck->second;

			_ContactPoint2Ds.erase(itCheck);

			if (contactPoint.pCollider->IsTrigger || pOtherCollider_->IsTrigger)
			{
				if (fTriggerExit)
					fTriggerExit(tick, pOtherCollider_);
			}
			else
			{
				if (fCollisionExit)
					fCollisionExit(tick, SCollision2D(SPoint(), SPoint(), contactPoint.pCollider, pOtherCollider_, movingObject));
			}
		}
	}
}
