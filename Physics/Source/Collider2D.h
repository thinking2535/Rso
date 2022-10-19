#pragma once

#include <functional>
#include "Object2D.h"

namespace rso::physics
{
	using namespace std;

	class CCollider2D;
	class CRectCollider2D;
	class CMovingObject2D;

	struct SCollision2D
	{
		const SPoint& RelativeVelocity;
		const SPoint& Normal;
		CCollider2D* const pCollider;
		CCollider2D* const pOtherCollider;
		CMovingObject2D* const pOtherMovingObject;
		SCollision2D(const SPoint& RelativeVelocity_, const SPoint& Normal_, CCollider2D* const pCollider_, CCollider2D* const pOtherCollider_, CMovingObject2D* const pOtherMovingObject_) :
			RelativeVelocity(RelativeVelocity_),
			Normal(Normal_),
			pCollider(pCollider_),
			pOtherCollider(pOtherCollider_),
			pOtherMovingObject(pOtherMovingObject_)
		{
		}
	};

	class CCollider2D abstract : public CObject2D
	{
	public:
		int32 Number = 0;
		bool IsTrigger = false;
		CCollider2D(const STransform& Transform_, int32 Number_);
		CCollider2D(const STransform& Transform_, const CObject2D* pParent_, int32 Number_);

		// �ᱹ CRectCollider2D �� CRectCollider2D �� ���� ��길 �����ϰ�, �Ķ���Ͱ� CEngineObject�� ���� ��ü�� �Ǿ�� CRectCollider2D���� CRigidbody2D���� �ľ��� �����ϹǷ�
		// ���� ��ü�� CRectCollider2D�϶����� �� �� ���� �Ķ���͸� �ٽ� ��ü�� �ٲپ� CRectCollider2D �϶� ���� CollisionCheck �� ȣ���Ͽ�
		// ��ü�� �Ķ���͸� ��� CRectCollider2D �� ���� �� ó��
		virtual bool CheckOverlapped(int64 tick, CMovingObject2D* pMovingObject_, CCollider2D* pOtherCollider_, CMovingObject2D* pOtherMovingObject_) = 0;
		virtual bool CheckOverlapped(int64 tick, CMovingObject2D* pMovingObject_, CRectCollider2D* pOtherRectCollider_, CMovingObject2D* pOtherMovingObject_) = 0;
	};
}
