#pragma once

#include "Neuron.h"

namespace rso::neuralnetwork
{
	class CNeuronGroupUnion : public INeuronGroup
	{
	protected:
		TNeuronGroups _NeuronGroups;

	public:
		CNeuronGroupUnion(const TNeuronGroups& NeuronGroups_);
		void FeedForward(void);
		void FeedBackward(void);
		void SetFreeze(bool Freeze_);
	};
}
