#pragma once

#include <optional>
#include <unordered_set>
#include "Collider2D.h"

namespace rso::physics
{
	using namespace std;

	class CPlayerObject2D;

	class CRectCollider2D : public CCollider2D
	{
		SRectCollider2D _Collider;

	public:
		SRect GetRect(void) const;
		CRectCollider2D(const STransform& Transform_, int32 Number_, const SRectCollider2D& Collider_);
		CRectCollider2D(const STransform& Transform_, int32 Number_, const SRectCollider2D& Collider_, const CObject2D* pParent_);
		inline void SetSize(const SPoint& Size_) { _Collider.Size = Size_; }
		inline void SetSizeX(float X_) { _Collider.Size.X = X_; }
		inline void SetSizeY(float Y_) { _Collider.Size.Y = Y_; }
		bool CheckOverlapped(int64 tick, CMovingObject2D* pMovingObject_, CCollider2D* pOtherCollider_, CMovingObject2D* pOtherMovingObject_) override;
		bool CheckOverlapped(int64 tick, CMovingObject2D* pMovingObject_, CRectCollider2D* pOtherRectCollider_, CMovingObject2D* pOtherMovingObject_) override;
	private:
		SPoint _FixPositionAndGetNormal(CMovingObject2D* pMovingObject_, const SRect& Rect_, const SRect& OtherRect_);
	};
}
