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

		// key�� SContactPoint2D���� ���� ������ �������� Collider�� ������ ��ü�� OnCollisionStay ����
		// ���� ó������ ��� _ContactPoint2Ds �� ���Ͽ� ���Ǿ�� �ϰ�,
		// FixedUpdate�� �ѹ� ���Ǿ�� �ϱ� ������ _ContactPoint2Ds �� CMovingObject2D �� �����ؾ��ϰ�
		// �ټ��� Collider�� ������ CMovingObject2D �� �ٴ�ٷ� Contact�� �Ͼ�� ������
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
