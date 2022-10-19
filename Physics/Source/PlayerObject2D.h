#pragma once

#include "MovingObject2D.h"

namespace rso::physics
{
	struct SContactPoint2D
	{
		CCollider2D* const pCollider;
		CCollider2D* const pOtherCollider;

		SContactPoint2D(CCollider2D* const pCollider_, CCollider2D* const pOtherCollider_) :
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
			return ((size_t)Value_.pCollider) ^ ((size_t)Value_.pOtherCollider);
		}
	};

	class CPlayerObject2D : public CMovingObject2D
	{
	public:
		using TContactPoint2Ds = unordered_map<SContactPoint2D, CMovingObject2D* const, SContactPoint2DHasher>;
		using FCollision = function<bool(int64 tick, const SCollision2D& Collision_)>;
		using FTrigger = function<bool(CCollider2D* pCollider_)>;

		FCollision fCollisionEnter;
		FCollision fCollisionStay;
		FCollision fCollisionExit;
		FTrigger fTriggerEnter;
		FTrigger fTriggerStay;
		FTrigger fTriggerExit;

		// key를 SContactPoint2D으로 쓰는 이유는 여러개의 Collider를 가지는 객체의 OnCollisionStay 에서
		// 마찰 처리등이 모든 _ContactPoint2Ds 에 대하여 계산되어야 하고,
		// FixedUpdate당 한번 계산되어야 하기 때문에 _ContactPoint2Ds 가 CMovingObject2D 에 존재해야하고
		// 다수의 Collider를 가지는 CMovingObject2D 는 다대다로 Contact가 일어나기 때문에
		TContactPoint2Ds _ContactPoint2Ds;

		CPlayerObject2D(const STransform& Transform_, const list<shared_ptr<CCollider2D>>& Colliders_, const SPoint& Velocity_);
		CPlayerObject2D* GetPlayerObject2D(void) override;

		bool CheckOverlapped(int64 tick, CCollider2D* pOtherCollider_);
		bool CheckOverlapped(int64 tick, CMovingObject2D* pOtherMovingObject_);
		bool Collided(int64 tick, const SCollision2D& Collision_);
		bool Triggered(CCollider2D* pCollider_, CCollider2D* pOtherCollider_, CMovingObject2D* pOtherMovingObject_);
		void NotOverlapped(int64 tick, CCollider2D* pCollider_, CCollider2D* pOtherCollider_);
		void NotOverlapped(int64 tick, CCollider2D* pOtherCollider_);
	};
}
