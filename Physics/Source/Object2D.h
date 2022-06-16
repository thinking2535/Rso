#pragma once

#include "Base.h"

namespace rso::physics
{
	// Collider 와 Transform 관계에서는 Collider의 Offset, Size 를 먼저 계산하고 이후 Transform 을 계산할것.
	// Transform 에서는 Rotation, Scale을 먼저 계산하고 이후 Position 을 계산할것.
	// Rotation 에서는 Z, X, Y 로 순서로 하되 축은 World 축 으로 고정이라고 가정하고 모델만 회전하는 방식으로 처리할것.
	// 객체간 상속관계에서는 자식에서 부모쪽으로 순서대로 처리할것.
	class CObject2D : public STransform
	{
		shared_ptr<CObject2D> _pParent;
	public:
		CObject2D(const STransform& Transform_);
		virtual ~CObject2D() {}

		inline void SetParent(const shared_ptr<CObject2D>& pParent_)
		{
			_pParent = pParent_;
		}
		SPoint  GetPosition(void) const;
	};
}
