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
	struct SNormalOtherMovingObject
	{
		SPoint Normal;
		shared_ptr<CMovingObject2D> pOtherMovingObject;

		SNormalOtherMovingObject(const SPoint& Normal_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_) :
			Normal(Normal_),
			pOtherMovingObject(pOtherMovingObject_)
		{
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
		using TContactPoint2Ds = unordered_map<SContactPoint2D, SNormalOtherMovingObject, SContactPoint2DHasher>;
		using FCollisionEnter = function<void(int64 Tick_, const SPoint& Normal_, const shared_ptr<CCollider2D>& pCollider_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_)>;
		using FCollisionStay = function<void(int64 Tick_, const TContactPoint2Ds& ContactPoint2Ds_)>;
		using FCollisionExit = function<void(int64 Tick_, const SPoint& Normal_, const shared_ptr<CCollider2D>& pCollider_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_)>;

		FCollisionEnter fCollisionEnter;
		FCollisionStay fCollisionStay;
		FCollisionExit fCollisionExit;

		// key�� SContactPoint2D���� ���� ������ �������� Collider�� ������ ��ü�� OnCollisionStay ����
		// ���� ó������ ��� _ContactPoint2Ds �� ���Ͽ� ���Ǿ�� �ϰ�,
		// FixedUpdate�� �ѹ� ���Ǿ�� �ϱ� ������ _ContactPoint2Ds �� CMovingObject2D �� �����ؾ��ϰ�
		// �ټ��� Collider�� ������ CMovingObject2D �� �ٴ�ٷ� Contact�� �Ͼ�� ������
		TContactPoint2Ds _ContactPoint2Ds;

		CPlayerObject2D(const STransform& Transform_, const SPoint& Velocity_);
		CPlayerObject2D* GetPlayerObject2D(void) override;

		void Overlapped(int64 Tick_, const SPoint& Normal_, const shared_ptr<CCollider2D>& pCollider_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_);
		void CollisionEnterCheck(int64 Tick_, const shared_ptr<CMovingObject2D>& pThisMovingObject_, const shared_ptr<CCollider2D>& pOtherCollider_);
		void CollisionEnterCheck(int64 Tick_, const shared_ptr<CMovingObject2D>& pThisMovingObject_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_);
		// Stay �� ���� �ݹ��Լ� ȣ��ȸ���� FixedUpdate ȣ��ȸ���� ���� �ϱ����� CollisionEnter�ÿ� _ContactPoint2Ds�� �̹� �����ϸ� Stay �ݹ��� ȣ���ϴ� ����� ������� ����
		void CollisionStayCheck(int64 Tick_);
		void CollisionExitCheck(int64 Tick_);
	};
}
