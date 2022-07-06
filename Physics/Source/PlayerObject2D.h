#pragma once

#include "MovingObject2D.h"

namespace rso::physics
{
	struct SContactPoint2D
	{
		shared_ptr<CCollider2D> pCollider;
		shared_ptr<CCollider2D> pOtherCollider;

		SContactPoint2D(const shared_ptr<CCollider2D>& pCollider_, const shared_ptr<CCollider2D>& pOtherCollider_) :
			pCollider(pCollider_),
			pOtherCollider(pOtherCollider_)
		{
		}
		bool operator == (const SContactPoint2D& Value_) const
		{
			return pCollider == Value_.pCollider && pOtherCollider == Value_.pOtherCollider;
		}
	};
	class SContactPoint2DHasher
	{
	public:
		size_t operator()(const SContactPoint2D& Value_) const
		{
			return ((size_t)Value_.pCollider.get()) ^ ((size_t)Value_.pOtherCollider.get());
		}
	};

	class CPlayerObject2D : public CMovingObject2D
	{
	public:
		using TContactPoint2Ds = unordered_map<SContactPoint2D, shared_ptr<CMovingObject2D>, SContactPoint2DHasher>;
		using FCollision = function<void(int64 Tick_, const SPoint& Normal_, const shared_ptr<CCollider2D>& pCollider_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_)>;
		using FCollisionExit = function<void(int64 Tick_, const shared_ptr<CCollider2D>& pCollider_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_)>;

		FCollision fCollisionEnter;
		FCollision fCollisionStay;
		FCollisionExit fCollisionExit;

		// key를 SContactPoint2D으로 쓰는 이유는 여러개의 Collider를 가지는 객체의 OnCollisionStay 에서
		// 마찰 처리등이 모든 _ContactPoint2Ds 에 대하여 계산되어야 하고,
		// FixedUpdate당 한번 계산되어야 하기 때문에 _ContactPoint2Ds 가 CMovingObject2D 에 존재해야하고
		// 다수의 Collider를 가지는 CMovingObject2D 는 다대다로 Contact가 일어나기 때문에
		TContactPoint2Ds _ContactPoint2Ds;

		CPlayerObject2D(const STransform& Transform_, const SPoint& Velocity_);
		CPlayerObject2D* GetPlayerObject2D(void) override;

		void Overlapped(int64 Tick_, const SPoint& Normal_, const shared_ptr<CCollider2D>& pCollider_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_);
		void NotOverlapped(int64 Tick_, const shared_ptr<CCollider2D>& pCollider_, const shared_ptr<CCollider2D>& pOtherCollider_);
		void OverlappedCheck(int64 Tick_, const shared_ptr<CMovingObject2D>& pThisMovingObject_, const shared_ptr<CCollider2D>& pOtherCollider_);
		void OverlappedCheck(int64 Tick_, const shared_ptr<CMovingObject2D>& pThisMovingObject_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_);
	};
}
