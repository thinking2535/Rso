#include "RectCollider2D.h"
#include "Physics.h"
#include "Engine.h"

namespace rso::physics
{
	SPoint CRectCollider2D::_getRelativeVelocity(CMovingObject2D* pMovingObject)
	{
		return -pMovingObject->Velocity;
	}
	SPoint CRectCollider2D::_getRelativeVelocity(CMovingObject2D* pMovingObject, CMovingObject2D* pOtherMovingObject)
	{
		return pOtherMovingObject->Velocity - pMovingObject->Velocity;
	}

	SRect CRectCollider2D::GetRect(void) const
	{
		auto Rect = RectCollider2DToRect(_Collider);
		Rect *= LocalScale;
		Rect += GetPosition();
		return Rect;
	}

	CRectCollider2D::CRectCollider2D(const STransform& Transform_, int32 Number_, const SRectCollider2D& Collider_) :
		CCollider2D(Transform_, Number_),
		_Collider(Collider_)
	{
	}
	CRectCollider2D::CRectCollider2D(const STransform& Transform_, int32 Number_, const SRectCollider2D& Collider_, const CObject2D* pParent_) :
		CCollider2D(Transform_, pParent_, Number_),
		_Collider(Collider_)
	{
	}
	bool CRectCollider2D::checkOverlapped(int64 tick, CMovingObject2D* pMovingObject, CCollider2D* pOtherCollider)
	{
		return pOtherCollider->checkOverlapped(tick, this, pMovingObject);
	}
	bool CRectCollider2D::checkOverlapped(int64 tick, CMovingObject2D* pMovingObject, CCollider2D* pOtherCollider, CMovingObject2D* pOtherMovingObject)
	{
		return pOtherCollider->checkOverlapped(tick, pOtherMovingObject, this, pMovingObject);
	}
	bool CRectCollider2D::checkOverlapped(int64 tick, CMovingObject2D* pMovingObject, CRectCollider2D* pOtherRectCollider)
	{
		auto rect = GetRect();
		auto otherRect = pOtherRectCollider->GetRect();

		if (!_isOverlapped(rect, pOtherRectCollider, otherRect))
		{
			pMovingObject->NotOverlapped(tick, this, pOtherRectCollider);
			return false;
		}

		if (IsTrigger || pOtherRectCollider->IsTrigger)
		{
			return pMovingObject->Triggered(tick, this, pOtherRectCollider, nullptr);
		}
		else
		{
			if (!pMovingObject->isKinematic)
			{
				auto relativeVelocity = _getRelativeVelocity(pMovingObject);
				auto normal = _fixPositionAndGetNormal(relativeVelocity, rect, pMovingObject, otherRect);

				return pMovingObject->Collided(
					tick,
					SCollision2D(
						relativeVelocity,
						normal,
						this,
						pOtherRectCollider,
						nullptr));
			}
		}

		return false;
	}
	bool CRectCollider2D::checkOverlapped(int64 tick, CRectCollider2D* pOtherRectCollider, CMovingObject2D* pOtherMovingObject)
	{
		return pOtherRectCollider->checkOverlapped(tick, pOtherMovingObject, this);
	}
	bool CRectCollider2D::checkOverlapped(int64 tick, CMovingObject2D* pMovingObject, CRectCollider2D* pOtherRectCollider, CMovingObject2D* pOtherMovingObject)
	{
		auto rect = GetRect();
		auto otherRect = pOtherRectCollider->GetRect();

		if (!_isOverlapped(rect, pOtherRectCollider, otherRect))
		{
			pMovingObject->NotOverlapped(tick, this, pOtherRectCollider);
			pOtherMovingObject->NotOverlapped(tick, pOtherRectCollider, this);
			return false;
		}

		if (IsTrigger || pOtherRectCollider->IsTrigger)
		{
			pOtherMovingObject->Triggered(tick, pOtherRectCollider, this, pMovingObject);
			return pMovingObject->Triggered(tick, this, pOtherRectCollider, pOtherMovingObject);
		}
		else
		{
			if (!pMovingObject->isKinematic && !pOtherMovingObject->isKinematic)
			{
				auto relativeVelocity = _getRelativeVelocity(pMovingObject, pOtherMovingObject);
				auto normal = _fixPositionAndGetNormal(relativeVelocity, rect, pMovingObject, otherRect, pOtherMovingObject);

				pOtherMovingObject->Collided(
					tick,
					SCollision2D(
						-relativeVelocity,
						-normal,
						pOtherRectCollider,
						this,
						pMovingObject));

				return pMovingObject->Collided(
					tick,
					SCollision2D(
						relativeVelocity,
						normal,
						this,
						pOtherRectCollider,
						pOtherMovingObject));
			}
			else
			{
				if (!pMovingObject->isKinematic)
				{
					auto relativeVelocity = _getRelativeVelocity(pMovingObject, pOtherMovingObject);
					auto normal = _fixPositionAndGetNormal(relativeVelocity, rect, pMovingObject, otherRect, pOtherMovingObject);

					return pMovingObject->Collided(
						tick,
						SCollision2D(
							relativeVelocity,
							normal,
							this,
							pOtherRectCollider,
							pOtherMovingObject));
				}
				else if (!pOtherMovingObject->isKinematic)
				{
					auto relativeVelocity = _getRelativeVelocity(pOtherMovingObject, pMovingObject);
					auto normal = _fixPositionAndGetNormal(relativeVelocity, otherRect, pOtherMovingObject, rect, pMovingObject);

					return pOtherMovingObject->Collided(
						tick,
						SCollision2D(
							relativeVelocity,
							normal,
							pOtherRectCollider,
							this,
							pMovingObject));
				}
				else
				{
					return false;
				}
			}
		}
	}
	bool CRectCollider2D::_isOverlapped(const SRect& rect, CRectCollider2D* pOtherRectCollider, const SRect& otherRect)
	{
		return GetEnabled() && pOtherRectCollider->GetEnabled() && IsOverlappedRectRect(rect, otherRect);
	}
	// 상대속도의 X, Y와 중첩된 X, Y 의 거리를 감안하여 Normal을 구하는 것이 이상적이나 상대속도가 0이거나 0에 가까우면 대소 비교가 안되어 오동작함
	// 따라서 상대속도는 고려하지 않고 중첩된 X, Y의 거리만으로 Normal을 구함
	SPoint CRectCollider2D::_fixPositionAndGetNormal(const SPoint& relativeVelocity, const SRect& rect, CMovingObject2D* pMovingObject, const SRect& otherRect)
	{
		auto rl = otherRect.Right - rect.Left;
		auto lr = rect.Right - otherRect.Left;
		auto tb = otherRect.Top - rect.Bottom;
		auto bt = rect.Top - otherRect.Bottom;

		if (rl < lr) // Normal.X : +
		{
			if (tb < bt) // Normal.Y : +
			{
				if (rl < tb) // select Normal.X
				{
					if (rl > CEngine::GetContactOffset())
						_moveAwayX(rl - CEngine::GetContactOffset(), pMovingObject);

					return _inelasticCollisionX(SPoint(1.0f, 0.0f), relativeVelocity, pMovingObject);
				}
				else // select Normal.Y
				{
					if (tb > CEngine::GetContactOffset())
						_moveAwayY(tb - CEngine::GetContactOffset(), pMovingObject);

					return _inelasticCollisionY(SPoint(0.0f, 1.0f), relativeVelocity, pMovingObject);
				}
			}
			else // Normal.Y : -
			{
				if (rl < bt) // select Normal.X
				{
					if (rl > CEngine::GetContactOffset())
						_moveAwayX(rl - CEngine::GetContactOffset(), pMovingObject);

					return _inelasticCollisionX(SPoint(1.0f, 0.0f), relativeVelocity, pMovingObject);
				}
				else // select Normal.Y
				{
					if (bt > CEngine::GetContactOffset())
						_moveAwayY(CEngine::GetContactOffset() - bt, pMovingObject);

					return _inelasticCollisionY(SPoint(0.0f, -1.0f), relativeVelocity, pMovingObject);
				}
			}
		}
		else // Normal.X : -
		{
			if (tb < bt) // Normal.Y : +
			{
				if (lr < tb) // select Normal.X
				{
					if (lr > CEngine::GetContactOffset())
						_moveAwayX(CEngine::GetContactOffset() - lr, pMovingObject);

					return _inelasticCollisionX(SPoint(-1.0f, 0.0f), relativeVelocity, pMovingObject);
				}
				else // select Normal.Y
				{
					if (tb > CEngine::GetContactOffset())
						_moveAwayY(tb - CEngine::GetContactOffset(), pMovingObject);

					return _inelasticCollisionY(SPoint(0.0f, 1.0f), relativeVelocity, pMovingObject);
				}
			}
			else // Normal.Y : -
			{
				if (lr < bt) // select Normal.X
				{
					if (lr > CEngine::GetContactOffset())
						_moveAwayX(CEngine::GetContactOffset() - lr, pMovingObject);

					return _inelasticCollisionX(SPoint(-1.0f, 0.0f), relativeVelocity, pMovingObject);
				}
				else // select Normal.Y
				{
					if (bt > CEngine::GetContactOffset())
						_moveAwayY(CEngine::GetContactOffset() - bt, pMovingObject);

					return _inelasticCollisionY(SPoint(0.0f, -1.0f), relativeVelocity, pMovingObject);
				}
			}
		}
	}
	SPoint CRectCollider2D::_fixPositionAndGetNormal(const SPoint& relativeVelocity, const SRect& rect, CMovingObject2D* pMovingObject, const SRect& otherRect, CMovingObject2D* pOtherMovingObject)
	{
		auto rl = otherRect.Right - rect.Left;
		auto lr = rect.Right - otherRect.Left;
		auto tb = otherRect.Top - rect.Bottom;
		auto bt = rect.Top - otherRect.Bottom;

		if (rl < lr) // Normal.X : +
		{
			if (tb < bt) // Normal.Y : +
			{
				if (rl < tb) // select Normal.X
				{
					if (rl > CEngine::GetContactOffset())
						_moveAwayX(rl - CEngine::GetContactOffset(), pMovingObject, pOtherMovingObject);

					return _inelasticCollisionX(SPoint(1.0f, 0.0f), relativeVelocity, pMovingObject, pOtherMovingObject);
				}
				else // select Normal.Y
				{
					if (tb > CEngine::GetContactOffset())
						_moveAwayY(tb - CEngine::GetContactOffset(), pMovingObject, pOtherMovingObject);

					return _inelasticCollisionY(SPoint(0.0f, 1.0f), relativeVelocity, pMovingObject, pOtherMovingObject);
				}
			}
			else // Normal.Y : -
			{
				if (rl < bt) // select Normal.X
				{
					if (rl > CEngine::GetContactOffset())
						_moveAwayX(rl - CEngine::GetContactOffset(), pMovingObject, pOtherMovingObject);

					return _inelasticCollisionX(SPoint(1.0f, 0.0f), relativeVelocity, pMovingObject, pOtherMovingObject);
				}
				else // select Normal.Y
				{
					if (bt > CEngine::GetContactOffset())
						_moveAwayY(CEngine::GetContactOffset() - bt, pMovingObject, pOtherMovingObject);

					return _inelasticCollisionY(SPoint(0.0f, -1.0f), relativeVelocity, pMovingObject, pOtherMovingObject);
				}
			}
		}
		else // Normal.X : -
		{
			if (tb < bt) // Normal.Y : +
			{
				if (lr < tb) // select Normal.X
				{
					if (lr > CEngine::GetContactOffset())
						_moveAwayX(CEngine::GetContactOffset() - lr, pMovingObject, pOtherMovingObject);

					return _inelasticCollisionX(SPoint(-1.0f, 0.0f), relativeVelocity, pMovingObject, pOtherMovingObject);
				}
				else // select Normal.Y
				{
					if (tb > CEngine::GetContactOffset())
						_moveAwayY(tb - CEngine::GetContactOffset(), pMovingObject, pOtherMovingObject);

					return _inelasticCollisionY(SPoint(0.0f, 1.0f), relativeVelocity, pMovingObject, pOtherMovingObject);
				}
			}
			else // Normal.Y : -
			{
				if (lr < bt) // select Normal.X
				{
					if (lr > CEngine::GetContactOffset())
						_moveAwayX(CEngine::GetContactOffset() - lr, pMovingObject, pOtherMovingObject);

					return _inelasticCollisionX(SPoint(-1.0f, 0.0f), relativeVelocity, pMovingObject, pOtherMovingObject);
				}
				else // select Normal.Y
				{
					if (bt > CEngine::GetContactOffset())
						_moveAwayY(CEngine::GetContactOffset() - bt, pMovingObject, pOtherMovingObject);

					return _inelasticCollisionY(SPoint(0.0f, -1.0f), relativeVelocity, pMovingObject, pOtherMovingObject);
				}
			}
		}
	}
	void CRectCollider2D::_moveAwayX(float overlapped, CMovingObject2D* pMovingObject)
	{
		pMovingObject->LocalPosition.X += overlapped;
	}
	void CRectCollider2D::_moveAwayY(float overlapped, CMovingObject2D* pMovingObject)
	{
		pMovingObject->LocalPosition.Y += overlapped;
	}
	void CRectCollider2D::_moveAwayX(float overlapped, CMovingObject2D* pMovingObject, CMovingObject2D* pOtherMovingObject)
	{
		if (pOtherMovingObject->isKinematic)
			return _moveAwayX(overlapped, pMovingObject);

		pMovingObject->LocalPosition.X += (overlapped * pOtherMovingObject->Mass / (pMovingObject->Mass + pOtherMovingObject->Mass));
		pOtherMovingObject->LocalPosition.X -= (overlapped * pMovingObject->Mass / (pMovingObject->Mass + pOtherMovingObject->Mass));
	}
	void CRectCollider2D::_moveAwayY(float overlapped, CMovingObject2D* pMovingObject, CMovingObject2D* pOtherMovingObject)
	{
		if (pOtherMovingObject->isKinematic)
			return _moveAwayY(overlapped, pMovingObject);

		pMovingObject->LocalPosition.Y += (overlapped * pOtherMovingObject->Mass / (pMovingObject->Mass + pOtherMovingObject->Mass));
		pOtherMovingObject->LocalPosition.Y -= (overlapped * pMovingObject->Mass / (pMovingObject->Mass + pOtherMovingObject->Mass));
	}
	const SPoint& CRectCollider2D::_inelasticCollisionX(const SPoint& normal, const SPoint& relativeVelocity, CMovingObject2D* pMovingObject)
	{
		if (normal.X * relativeVelocity.X > 0.0f)
			pMovingObject->Velocity.X = 0.0f;

		return normal;
	}
	const SPoint& CRectCollider2D::_inelasticCollisionY(const SPoint& normal, const SPoint& relativeVelocity, CMovingObject2D* pMovingObject)
	{
		if (normal.Y * relativeVelocity.Y > 0.0f)
			pMovingObject->Velocity.Y = 0.0f;

		return normal;
	}
	const SPoint& CRectCollider2D::_inelasticCollisionX(const SPoint& normal, const SPoint& relativeVelocity, CMovingObject2D* pMovingObject, CMovingObject2D* pOtherMovingObject)
	{
		if (normal.X * relativeVelocity.X > 0.0f)
		{
			if (pOtherMovingObject->isKinematic)
				pMovingObject->Velocity.X = pOtherMovingObject->Velocity.X;
			else
				pMovingObject->Velocity.X = pOtherMovingObject->Velocity.X = (pMovingObject->Velocity.X * pMovingObject->Mass + pOtherMovingObject->Velocity.X * pOtherMovingObject->Mass) / (pMovingObject->Mass + pOtherMovingObject->Mass);
		}

		return normal;
	}
	const SPoint& CRectCollider2D::_inelasticCollisionY(const SPoint& normal, const SPoint& relativeVelocity, CMovingObject2D* pMovingObject, CMovingObject2D* pOtherMovingObject)
	{
		if (normal.Y * relativeVelocity.Y > 0.0f)
		{
			if (pOtherMovingObject->isKinematic)
				pMovingObject->Velocity.Y = pOtherMovingObject->Velocity.Y;
			else
				pMovingObject->Velocity.Y = pOtherMovingObject->Velocity.Y = (pMovingObject->Velocity.Y * pMovingObject->Mass + pOtherMovingObject->Velocity.Y * pOtherMovingObject->Mass) / (pMovingObject->Mass + pOtherMovingObject->Mass);
		}

		return normal;
	}
}
