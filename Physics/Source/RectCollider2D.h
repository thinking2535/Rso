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
		inline void SetSize(const SPoint& Size_) { _Collider.Size = Size_; }
		inline void SetSizeX(float X_) { _Collider.Size.X = X_; }
		inline void SetSizeY(float Y_) { _Collider.Size.Y = Y_; }
	private:
		optional<SPoint> _OverlappedCheck(CPlayerObject2D* pPlayerObject_, const SRect& Rect_, const SRect& OtherRect_);
	public:
		void OverlappedCheck(int64 Tick_, const shared_ptr<CCollider2D>& pThisCollider_, const shared_ptr<CMovingObject2D>& pMovingObject_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_) override;
	private:
		optional<SPoint> _OverlappedCheck(const shared_ptr<CRectCollider2D> pOtherRectCollider_, CPlayerObject2D* pPlayerObject_, CPlayerObject2D* pOtherPlayerObject_);
	public:
		void OverlappedCheck(int64 Tick_, const shared_ptr<CCollider2D>& pThisCollider_, const shared_ptr<CMovingObject2D>& pMovingObject_, const shared_ptr<CRectCollider2D>& pOtherRectCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_) override;
		bool IsOverlapped(int64 Tick_, CCollider2D* pOtherCollider_) override;
		bool IsOverlapped(int64 Tick_, CRectCollider2D* pOtherRectCollider_) override;
	};
}
