#include "ObjectStraight.h"

namespace rso::physics
{
	CObjectStraight::CObjectStraight(const SObjectStraight& ObjectStraight_) :
		SObjectStraight(ObjectStraight_)
	{
	}
	void CObjectStraight::SetPosTheta(float Time_, const SPosTheta& PosTheta_)
	{
		PosTheta = PosTheta_;
		Time = Time_;
	}
	void CObjectStraight::SetSpeed(float Time_, float Speed_)
	{
		if (Dist > 0.0)
		{
			auto CurDist = Speed * (Time_ - Time);
			if (CurDist > Dist)
			{
				CurDist = Dist;
				Dist = 0.0;
			}

			Time = Time_;
			PosTheta.Pos += (Vector(PosTheta.Theta, CurDist));
		}
		Speed = Speed_;
	}
}