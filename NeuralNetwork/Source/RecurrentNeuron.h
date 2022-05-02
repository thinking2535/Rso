#pragma once

#include "Neuron.h"

namespace rso::neuralnetwork
{
	class CRecurrentNeuron : public CNeuron
	{
		double _StateBias = 0.0;
		double _StateInput = 0.0;
		double _StateInputWeight = GetRandom(1, 1);
		double _StateOutput = 0.0;
		double _StateOutputWeight = GetRandom(1, 1);

	public:
		void FeedForward(double Input_) noexcept override;
		void FeedBackward(double Error_) noexcept override;
	};
}
