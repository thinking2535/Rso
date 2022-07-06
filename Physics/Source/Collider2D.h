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

		// �ᱹ CRectCollider2D �� CRectCollider2D �� ���� ��길 �����ϰ�, �Ķ���Ͱ� CEngineObject�� ���� ��ü�� �Ǿ�� CRectCollider2D���� CRigidbody2D���� �ľ��� �����ϹǷ�
		// ���� ��ü�� CRectCollider2D�϶����� �� �� ���� �Ķ���͸� �ٽ� ��ü�� �ٲپ� CRectCollider2D �϶� ���� CollisionCheck �� ȣ���Ͽ�
		// ��ü�� �Ķ���͸� ��� CRectCollider2D �� ���� �� ó��
		virtual void OverlappedCheck(int64 Tick_, const shared_ptr<CCollider2D>& pThisCollider_, const shared_ptr<CMovingObject2D>& pMovingObject_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_) = 0;
		virtual void OverlappedCheck(int64 Tick_, const shared_ptr<CCollider2D>& pThisCollider_, const shared_ptr<CMovingObject2D>& pMovingObject_, const shared_ptr<CRectCollider2D>& pOtherRectCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_) = 0;
		virtual bool IsOverlapped(int64 Tick_, CCollider2D* pOtherCollider_) = 0;
		virtual bool IsOverlapped(int64 Tick_, CRectCollider2D* pOtherRectCollider_) = 0;
	};
}
