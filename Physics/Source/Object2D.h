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
		const CObject2D* _pParent = nullptr;
	private:
		bool _Enabled = true;
	public:
		bool GetEnabled() const;
		void SetEnabled(bool Enabled_);
		SPoint  GetPosition(void) const;
		CObject2D(const STransform& Transform_);
		CObject2D(const STransform& Transform_, const CObject2D* pParent_);
		virtual ~CObject2D() {}

		inline void SetParent(const CObject2D* pParent_)
		{
			_pParent = pParent_;
		}
	};
}
