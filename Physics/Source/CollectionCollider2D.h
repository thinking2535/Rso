#pragma once

#include "Collider2D.h"

namespace rso::physics
{
	class CCollectionCollider2D : public CCollider2D
	{
	public:
		list<shared_ptr<CCollider2D>> Colliders;

		CCollectionCollider2D(const STransform& Transform_, int32 Number_);
		void OverlappedCheck(int64 Tick_, const shared_ptr<CCollider2D>& pThisCollider_, const shared_ptr<CMovingObject2D>& pMovingObject_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_) override;
		void OverlappedCheck(int64 Tick_, const shared_ptr<CCollider2D>& pThisCollider_, const shared_ptr<CMovingObject2D>& pMovingObject_, const shared_ptr<CRectCollider2D>& pOtherRectCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_) override;
		bool IsOverlapped(int64 Tick_, CCollider2D* pOtherCollider_) override;
		bool IsOverlapped(int64 Tick_, CRectCollider2D* pOtherRectCollider_) override;
	};

	void SetCollectionToCollectionCollider2D(const list<shared_ptr<CCollider2D>>& Collection_, const shared_ptr<CCollectionCollider2D>& pCollider_);
	void SetCollectionToCollectionCollider2D(list<shared_ptr<CCollider2D>>&& Collection_, const shared_ptr<CCollectionCollider2D>& pCollider_);
}
