#include "Object2D.h"

namespace rso::physics
{
	bool CObject2D::GetEnabled() const
	{
		return _Enabled && (_pParent == nullptr || _pParent->GetEnabled());
	}
	SPoint CObject2D::GetPosition() const
	{
		if (_pParent != nullptr)
			return _pParent->GetPosition() + LocalPosition;
		else
			return LocalPosition;
	}
	void CObject2D::SetEnabled(bool Enabled_)
	{
		_Enabled = Enabled_;
	}
	CObject2D::CObject2D(const STransform& Transform_) :
		STransform(Transform_)
	{
	}
	CObject2D::CObject2D(const STransform& Transform_, const CObject2D* pParent_) :
		STransform(Transform_),
		_pParent(pParent_)
	{
	}
}
