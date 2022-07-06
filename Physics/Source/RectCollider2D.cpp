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
		CCollider2D(Transform_, Number_), _Collider(Collider_)
	{
	}
	optional<SPoint> CRectCollider2D::_OverlappedCheck(CPlayerObject2D* pPlayerObject_, const SRect& Rect_, const SRect& OtherRect_)
	{
		if (!IsOverlappedRectRect(Rect_, OtherRect_))
			return {};

		auto rl = OtherRect_.Right - Rect_.Left;
		auto lr = Rect_.Right - OtherRect_.Left;
		auto tb = OtherRect_.Top - Rect_.Bottom;
		auto bt = Rect_.Top - OtherRect_.Bottom;

		optional<SPoint> Normal = SPoint();

		if (rl < lr) // Normal.X : +
		{
			if (tb < bt) // Normal.Y : +
			{
				if (rl < tb) // select Normal.X
				{
					if (rl > CEngine::GetContactOffset())
						pPlayerObject_->LocalPosition.X += (rl - CEngine::GetContactOffset());

					Normal.value().X = 1.0f;
				}
				else // select Normal.Y
				{
					if (tb > CEngine::GetContactOffset())
						pPlayerObject_->LocalPosition.Y += (tb - CEngine::GetContactOffset());

					Normal.value().Y = 1.0f;
				}
			}
			else // Normal.Y : -
			{
				if (rl < bt) // select Normal.X
				{
					if (rl > CEngine::GetContactOffset())
						pPlayerObject_->LocalPosition.X += (rl - CEngine::GetContactOffset());

					Normal.value().X = 1.0f;
				}
				else // select Normal.Y
				{
					if (bt > CEngine::GetContactOffset())
						pPlayerObject_->LocalPosition.Y += (CEngine::GetContactOffset() - bt);

					Normal.value().Y = -1.0f;
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
						pPlayerObject_->LocalPosition.X += (CEngine::GetContactOffset() - lr);

					Normal.value().X = -1.0f;
				}
				else // select Normal.Y
				{
					if (tb > CEngine::GetContactOffset())
						pPlayerObject_->LocalPosition.Y += (tb - CEngine::GetContactOffset());

					Normal.value().Y = 1.0f;
				}
			}
			else // Normal.Y : -
			{
				if (lr < bt) // select Normal.X
				{
					if (lr > CEngine::GetContactOffset())
						pPlayerObject_->LocalPosition.X += (CEngine::GetContactOffset() - lr);

					Normal.value().X = -1.0f;
				}
				else // select Normal.Y
				{
					if (bt > CEngine::GetContactOffset())
						pPlayerObject_->LocalPosition.Y += (CEngine::GetContactOffset() - bt);

					Normal.value().Y = -1.0f;
				}
			}
		}

		return Normal;
	}
	void CRectCollider2D::OverlappedCheck(int64 Tick_, const shared_ptr<CCollider2D>& pThisCollider_, const shared_ptr<CMovingObject2D>& pMovingObject_, const shared_ptr<CCollider2D>& pOtherCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_)
	{
		if (!LocalEnabled)
			return;

		pOtherCollider_->OverlappedCheck(Tick_, pOtherCollider_, pOtherMovingObject_, dynamic_pointer_cast<CRectCollider2D>(pThisCollider_), pMovingObject_);
	}
	optional<SPoint> CRectCollider2D::_OverlappedCheck(const shared_ptr<CRectCollider2D> pOtherRectCollider_, CPlayerObject2D* pPlayerObject_, CPlayerObject2D* pOtherPlayerObject_)
	{
		if (!LocalEnabled)
			return {};

		optional<SPoint> Normal;

		if (pPlayerObject_ != nullptr)
		{
			Normal = _OverlappedCheck(pPlayerObject_, GetRect(), pOtherRectCollider_->GetRect());
			if (!Normal)
				return {};
		}
		else if (pOtherPlayerObject_ != nullptr)
		{
			Normal = _OverlappedCheck(pOtherPlayerObject_, pOtherRectCollider_->GetRect(), GetRect());
			if (!Normal)
				return {};

			Normal.value() *= -1.0f;
		}
		else
		{
			return {};
		}

		return Normal;
	}
	void CRectCollider2D::OverlappedCheck(int64 Tick_, const shared_ptr<CCollider2D>& pThisCollider_, const shared_ptr<CMovingObject2D>& pMovingObject_, const shared_ptr<CRectCollider2D>& pOtherRectCollider_, const shared_ptr<CMovingObject2D>& pOtherMovingObject_)
	{
		auto pThisCollider = dynamic_pointer_cast<CRectCollider2D>(pThisCollider_);

		CPlayerObject2D* pPlayerObject = pMovingObject_ == nullptr ? nullptr : pMovingObject_->GetPlayerObject2D();
		CPlayerObject2D* pOtherPlayerObject = pOtherMovingObject_ == nullptr ? nullptr : pOtherMovingObject_->GetPlayerObject2D();

		auto Normal = _OverlappedCheck(pOtherRectCollider_, pPlayerObject, pOtherPlayerObject);
		if (Normal)
		{
			if (pPlayerObject != nullptr)
				pPlayerObject->Overlapped(Tick_, Normal.value(), pThisCollider, pOtherRectCollider_, pOtherMovingObject_);

			if (pOtherPlayerObject != nullptr)
				pOtherPlayerObject->Overlapped(Tick_, -Normal.value(), pOtherRectCollider_, pThisCollider, pMovingObject_);
		}
		else
		{
			if (pPlayerObject != nullptr)
				pPlayerObject->NotOverlapped(Tick_, pThisCollider, pOtherRectCollider_);

			if (pOtherPlayerObject != nullptr)
				pOtherPlayerObject->NotOverlapped(Tick_, pOtherRectCollider_, pThisCollider);
		}
	}
	bool CRectCollider2D::IsOverlapped(int64 Tick_, CCollider2D* pOtherCollider_)
	{
		return pOtherCollider_->IsOverlapped(Tick_, this);
	}
	bool CRectCollider2D::IsOverlapped(int64 /*Tick_*/, CRectCollider2D* pOtherRectCollider_)
	{
		return (GetEnabled() && pOtherRectCollider_->GetEnabled()  && IsOverlappedRectRect(GetRect(), pOtherRectCollider_->GetRect()));
	}
}
