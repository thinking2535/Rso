#include "Object2D.h"

namespace rso::physics
{
	CObject2D::CObject2D(const STransform& Transform_) :
		STransform(Transform_)
	{
	}
	SPoint CObject2D::GetPosition() const
	{
		if (_pParent != nullptr)
			return _pParent->GetPosition() + LocalPosition;
		else
			return LocalPosition;
	}
}
