#pragma once

#include "Physics.h"

namespace rso::physics
{
	class CObjectStraight : public SObjectStraight
	{
	public:
		CObjectStraight(const SObjectStraight& ObjectStraight_);
		void SetPosTheta(float Time_, const SPosTheta& PosTheta_);
		void SetSpeed(float Time_, float Speed_);
	};
}