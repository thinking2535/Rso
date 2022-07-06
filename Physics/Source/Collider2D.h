#pragma once

#include <functional>
#include "Object2D.h"

namespace rso::physics
{
	using namespace std;

	class CCollider2D;
	class CRectCollider2D;
	class CMovingObject2D;

	class CCollider2D abstract : public CObject2D
	{
	public:
		int32 Number = 0;
		bool LocalEnabled = true;
		inline bool GetEnabled(void) const
		{
			return LocalEnabled && (_pParentCollider == nullptr || _pParentCollider->GetEnabled());
		}
		shared_ptr<CCollider2D> _pParentCollider;
		inline void SetParentCollider(const shared_ptr<CCollider2D>& pParentCollider_)
		{
			_pParentCollider = pParentCollider_;
			SetParent(pParentCollider_);
		}
		CCollider2D(const STransform& Transform_, int32 Number_);

		// 결국 CRectCollider2D 과 CRectCollider2D 에 대한 계산만 가능하고, 파라미터가 CEngineObject일 때는 주체가 되어야 CRectCollider2D인지 CRigidbody2D인지 파악이 가능하므로
		// 먼저 주체가 CRectCollider2D일때까지 들어간 후 이후 파라미터를 다시 주체로 바꾸어 CRectCollider2D 일때 까지 CollisionCheck 를 호출하여
		// 주체와 파라미터를 모두 CRectCollider2D 로 만든 후 처리
		virtual void OverlappedCheck(int64 Tick_, const shared_ptr<CCollider2D>& pThisCollider_, const shared_ptr<CMovingObject2D>& pMovingObject_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_) = 0;
		virtual void OverlappedCheck(int64 Tick_, const shared_ptr<CCollider2D>& pThisCollider_, const shared_ptr<CMovingObject2D>& pMovingObject_, const shared_ptr<CRectCollider2D>& pOtherRectCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_) = 0;
		virtual bool IsOverlapped(int64 Tick_, CCollider2D* pOtherCollider_) = 0;
		virtual bool IsOverlapped(int64 Tick_, CRectCollider2D* pOtherRectCollider_) = 0;
	};
}
