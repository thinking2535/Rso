#pragma once

#include "Neuron.h"

namespace rso::brain
{
	class CNeuronRecurrent : public CNeuron
	{
		double _State = 0.0;
		double _StateWeight = 1.0;

		void FeedForward(double Alpha_) override;
	};
}