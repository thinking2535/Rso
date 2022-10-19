#include "RectCollider2D.h"
#include "Physics.h"
#include "Engine.h"

namespace rso::physics
{
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
	bool CRectCollider2D::CheckOverlapped(int64 tick, CMovingObject2D* pMovingObject_, CCollider2D* pOtherCollider_, CMovingObject2D* pOtherMovingObject_)
	{
		return pOtherCollider_->CheckOverlapped(tick, pOtherMovingObject_, this, pMovingObject_);
	}
	bool CRectCollider2D::CheckOverlapped(int64 tick, CMovingObject2D* pMovingObject_, CRectCollider2D* pOtherRectCollider_, CMovingObject2D* pOtherMovingObject_)
	{
		CPlayerObject2D* pPlayerObject = pMovingObject_ ? pMovingObject_->GetPlayerObject2D() : nullptr;
		CPlayerObject2D* pOtherPlayerObject = pOtherMovingObject_ ? pOtherMovingObject_->GetPlayerObject2D() : nullptr;

		if (!GetEnabled() || !pOtherRectCollider_->GetEnabled() || !IsOverlappedRectRect(GetRect(), pOtherRectCollider_->GetRect()))
		{
			if (pPlayerObject)
				pPlayerObject->NotOverlapped(tick, this, pOtherRectCollider_);

			if (pOtherPlayerObject)
				pOtherPlayerObject->NotOverlapped(tick, pOtherRectCollider_, this);

			return false;
		}

		if (IsTrigger || pOtherRectCollider_->IsTrigger)
		{
			if (pPlayerObject)
			{
				auto DoRemove = pPlayerObject->Triggered(this, pOtherRectCollider_, pOtherMovingObject_);

				if (!pOtherPlayerObject)
					return DoRemove;

				pOtherPlayerObject->Triggered(pOtherRectCollider_, this, pMovingObject_);
				return false;
			}
			else if (pOtherPlayerObject)
			{
				return pOtherPlayerObject->Triggered(pOtherRectCollider_, this, pMovingObject_);
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (pPlayerObject != nullptr)
			{
				auto Normal = _FixPositionAndGetNormal(pMovingObject_, GetRect(), pOtherRectCollider_->GetRect());

				auto DoRemove = pPlayerObject->Collided(
					tick,
					SCollision2D(
						pOtherMovingObject_ != nullptr ? pOtherMovingObject_->Velocity - pMovingObject_->Velocity : -pMovingObject_->Velocity,
						Normal,
						this,
						pOtherRectCollider_,
						pOtherMovingObject_));

				if (!pOtherPlayerObject)
					return DoRemove;

				pOtherPlayerObject->Collided(
					tick,
					SCollision2D(
						pMovingObject_->Velocity - pOtherMovingObject_->Velocity,
						-Normal,
						pOtherRectCollider_,
						this,
						pMovingObject_));
				return false;
			}
			else if (pOtherPlayerObject != nullptr)
			{
				auto Normal = _FixPositionAndGetNormal(pOtherMovingObject_, pOtherRectCollider_->GetRect(), GetRect());
				return pOtherPlayerObject->Collided(
					tick,
					SCollision2D(
						-pOtherMovingObject_->Velocity,
						Normal,
						pOtherRectCollider_,
						this,
						pMovingObject_));
			}
			else
			{
				return false;
			}
		}
	}
	SPoint CRectCollider2D::_FixPositionAndGetNormal(CMovingObject2D* pMovingObject_, const SRect& Rect_, const SRect& OtherRect_)
	{
		auto rl = OtherRect_.Right - Rect_.Left;
		auto lr = Rect_.Right - OtherRect_.Left;
		auto tb = OtherRect_.Top - Rect_.Bottom;
		auto bt = Rect_.Top - OtherRect_.Bottom;

		SPoint Normal;

		if (rl < lr) // Normal.X : +
		{
			if (tb < bt) // Normal.Y : +
			{
				if (rl < tb) // select Normal.X
				{
					if (rl > CEngine::GetContactOffset())
						pMovingObject_->LocalPosition.X += (rl - CEngine::GetContactOffset());

					Normal.X = 1.0f;
				}
				else // select Normal.Y
				{
					if (tb > CEngine::GetContactOffset())
						pMovingObject_->LocalPosition.Y += (tb - CEngine::GetContactOffset());

					Normal.Y = 1.0f;
				}
			}
			else // Normal.Y : -
			{
				if (rl < bt) // select Normal.X
				{
					if (rl > CEngine::GetContactOffset())
						pMovingObject_->LocalPosition.X += (rl - CEngine::GetContactOffset());

					Normal.X = 1.0f;
				}
				else // select Normal.Y
				{
					if (bt > CEngine::GetContactOffset())
						pMovingObject_->LocalPosition.Y += (CEngine::GetContactOffset() - bt);

					Normal.Y = -1.0f;
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
						pMovingObject_->LocalPosition.X += (CEngine::GetContactOffset() - lr);

					Normal.X = -1.0f;
				}
				else // select Normal.Y
				{
					if (tb > CEngine::GetContactOffset())
						pMovingObject_->LocalPosition.Y += (tb - CEngine::GetContactOffset());

					Normal.Y = 1.0f;
				}
			}
			else // Normal.Y : -
			{
				if (lr < bt) // select Normal.X
				{
					if (lr > CEngine::GetContactOffset())
						pMovingObject_->LocalPosition.X += (CEngine::GetContactOffset() - lr);

					Normal.X = -1.0f;
				}
				else // select Normal.Y
				{
					if (bt > CEngine::GetContactOffset())
						pMovingObject_->LocalPosition.Y += (CEngine::GetContactOffset() - bt);

					Normal.Y = -1.0f;
				}
			}
		}

		return Normal;
	}
}
