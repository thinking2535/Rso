#pragma once

#include "Neuron.h"

namespace rso::brain
{
	class CNeuronGroupUnion : public INeuronGroup
	{
	protected:
		TNeuronGroups _NeuronGroups;

	public:
		CNeuronGroupUnion(const TNeuronGroups& NeuronGroups_);
		void FeedForward(double Alpha_) override;
		void FeedBackward(double Alpha_) override;
	};
}
