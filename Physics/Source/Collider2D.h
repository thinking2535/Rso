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

		// 결국 CRectCollider2D 과 CRectCollider2D 에 대한 계산만 가능하고, 파라미터가 CEngineObject일 때는 주체가 되어야 CRectCollider2D인지 CRigidbody2D인지 파악이 가능하므로
		// 먼저 주체가 CRectCollider2D일때까지 들어간 후 이후 파라미터를 다시 주체로 바꾸어 CRectCollider2D 일때 까지 CollisionCheck 를 호출하여
		// 주체와 파라미터를 모두 CRectCollider2D 로 만든 후 처리
		virtual bool CheckOverlapped(int64 tick, CMovingObject2D* pMovingObject_, CCollider2D* pOtherCollider_, CMovingObject2D* pOtherMovingObject_) = 0;
		virtual bool CheckOverlapped(int64 tick, CMovingObject2D* pMovingObject_, CRectCollider2D* pOtherRectCollider_, CMovingObject2D* pOtherMovingObject_) = 0;
	};
}
