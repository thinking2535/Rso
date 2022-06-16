#include "Collider2D.h"

namespace rso::physics
{
	CCollider2D::CCollider2D(const STransform& Transform_, int32 Number_) :
		CObject2D(Transform_),
		Number(Number_)
	{
	}
}
