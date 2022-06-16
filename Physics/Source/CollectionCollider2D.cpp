#include "CollectionCollider2D.h"

namespace rso::physics
{
	CCollectionCollider2D::CCollectionCollider2D(const STransform& Transform_, int32 Number_) :
		CCollider2D(Transform_, Number_)
	{
	}
	void CCollectionCollider2D::OverlappedCheck(int64 Tick_, const shared_ptr<CCollider2D>& /*pThisCollider_*/, const shared_ptr<CMovingObject2D>& pMovingObject_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_)
	{
		if (!LocalEnabled)
			return;

		for (auto& o : Colliders)
			o->OverlappedCheck(Tick_, o, pMovingObject_, pOtherCollider_, pOtherMovingObject_);
	}
	void CCollectionCollider2D::OverlappedCheck(int64 Tick_, const shared_ptr<CCollider2D>& /*pThisCollider_*/, const shared_ptr<CMovingObject2D>& pMovingObject_, const shared_ptr<CRectCollider2D>& pOtherRectCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_)
	{
		if (!LocalEnabled)
			return;

		for (auto& o : Colliders)
			o->OverlappedCheck(Tick_, o, pMovingObject_, pOtherRectCollider_, pOtherMovingObject_);
	}
	bool CCollectionCollider2D::IsOverlapped(int64 /*Tick_*/, CCollider2D* /*pOtherCollider_*/)
	{
		throw exception();
	}
	bool CCollectionCollider2D::IsOverlapped(int64 /*Tick_*/, CRectCollider2D* /*pOtherRectCollider_*/)
	{
		throw exception();
	}

	void SetCollectionToCollectionCollider2D(const list<shared_ptr<CCollider2D>>& Collection_, const shared_ptr<CCollectionCollider2D>& pCollider_)
	{
		pCollider_->Colliders = Collection_;

		for (auto& i : pCollider_->Colliders)
			i->SetParentCollider(pCollider_);
	}
	void SetCollectionToCollectionCollider2D(list<shared_ptr<CCollider2D>>&& Collection_, const shared_ptr<CCollectionCollider2D>& pCollider_)
	{
		pCollider_->Colliders = std::move(Collection_);

		for (auto& i : pCollider_->Colliders)
			i->SetParentCollider(pCollider_);
	}
}
