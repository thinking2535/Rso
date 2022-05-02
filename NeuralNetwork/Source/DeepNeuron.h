#pragma once

#include "Neuron.h"

namespace rso::neuralnetwork
{
	class CDeepNeuron : public CNeuron
	{
	public:
		void FeedForward(double Input_) noexcept override;
		void FeedBackward(double Error_) noexcept override;
	};
}
