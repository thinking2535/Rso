#pragma once

#include "Base.h"

namespace rso::physics
{
	// Collider �� Transform ���迡���� Collider�� Offset, Size �� ���� ����ϰ� ���� Transform �� ����Ұ�.
	// Transform ������ Rotation, Scale�� ���� ����ϰ� ���� Position �� ����Ұ�.
	// Rotation ������ Z, X, Y �� ������ �ϵ� ���� World �� ���� �����̶�� �����ϰ� �𵨸� ȸ���ϴ� ������� ó���Ұ�.
	// ��ü�� ��Ӱ��迡���� �ڽĿ��� �θ������� ������� ó���Ұ�.
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
