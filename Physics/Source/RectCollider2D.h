#pragma once

#include <optional>
#include <unordered_set>
#include "Collider2D.h"

namespace rso::physics
{
	using namespace std;

	class CMovingObject2D;
	class CPlayerObject2D;

	class CRectCollider2D : public CCollider2D
	{
		static SPoint _getRelativeVelocity(CMovingObject2D* pMovingObject);
		static SPoint _getRelativeVelocity(CMovingObject2D* pMovingObject, CMovingObject2D* pOtherMovingObject);

	private:
		SRectCollider2D _Collider;

	public:
		SRect GetRect(void) const;
		CRectCollider2D(const STransform& Transform_, int32 Number_, const SRectCollider2D& Collider_);
		CRectCollider2D(const STransform& Transform_, int32 Number_, const SRectCollider2D& Collider_, const CObject2D* pParent_);
		inline void SetSize(const SPoint& Size_) { _Collider.Size = Size_; }
		inline void SetSizeX(float X_) { _Collider.Size.X = X_; }
		inline void SetSizeY(float Y_) { _Collider.Size.Y = Y_; }
		bool checkOverlapped(int64 tick, CMovingObject2D* pMovingObject, CCollider2D* pOtherCollider) override;
		bool checkOverlapped(int64 tick, CMovingObject2D* pMovingObject, CCollider2D* pOtherCollider, CMovingObject2D* pOtherMovingObject) override;
		bool checkOverlapped(int64 tick, CMovingObject2D* pMovingObject, CRectCollider2D* pOtherRectCollider) override;
		bool checkOverlapped(int64 tick, CRectCollider2D* pOtherRectCollider, CMovingObject2D* pOtherMovingObject) override;
		bool checkOverlapped(int64 tick, CMovingObject2D* pMovingObject, CRectCollider2D* pOtherRectCollider, CMovingObject2D* pOtherMovingObject) override;
	private:
		bool _isOverlapped(const SRect& rect, CRectCollider2D* pOtherRectCollider, const SRect& otherRect);
		SPoint _fixPositionAndGetNormal(const SPoint& relativeVelocity, const SRect& rect, CMovingObject2D* pMovingObject, const SRect& otherRect);
		SPoint _fixPositionAndGetNormal(const SPoint& relativeVelocity, const SRect& rect, CMovingObject2D* pMovingObject, const SRect& otherRect, CMovingObject2D* pOtherMovingObject);
		static void _moveAwayX(float overlapped, CMovingObject2D* pMovingObject);
		static void _moveAwayY(float overlapped, CMovingObject2D* pMovingObject);
		static void _moveAwayX(float overlapped, CMovingObject2D* pMovingObject, CMovingObject2D* pOtherMovingObject);
		static void _moveAwayY(float overlapped, CMovingObject2D* pMovingObject, CMovingObject2D* pOtherMovingObject);
		static const SPoint& _inelasticCollisionX(const SPoint& normal, const SPoint& relativeVelocity, CMovingObject2D* pMovingObject);
		static const SPoint& _inelasticCollisionY(const SPoint& normal, const SPoint& relativeVelocity, CMovingObject2D* pMovingObject);
		static const SPoint& _inelasticCollisionX(const SPoint& normal, const SPoint& relativeVelocity, CMovingObject2D* pMovingObject, CMovingObject2D* pOtherMovingObject);
		static const SPoint& _inelasticCollisionY(const SPoint& normal, const SPoint& relativeVelocity, CMovingObject2D* pMovingObject, CMovingObject2D* pOtherMovingObject);
	};
}
