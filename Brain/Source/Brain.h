#pragma once

#include "NeuronGroup.h"
#include "NeuronGroupUnionD.h"
#include "NeuronGroupUnionW.h"

namespace rso::brain
{
	class CBrain
	{
		double _Alpha = 1.0;
		INeuronGroup* _NeuronGroup = nullptr;

	public:
		CBrain(double Alpha_, INeuronGroup* NeuronGroup_);
		inline void FeedForward(void)
		{
			_NeuronGroup->FeedForward(_Alpha);
		}
		inline void FeedBackward(void)
		{
			_NeuronGroup->FeedBackward(_Alpha);
		}
		inline const TEndLayer& GetInputLayer(void) const
		{
			return _NeuronGroup->GetInputLayer();
		}
		inline const TEndLayer& GetOutputLayer(void) const
		{
			return _NeuronGroup->GetOutputLayer();
		}
	};
}
