#pragma once

#include "Neuron.h"

namespace rso
{
	namespace brain
	{
		class CNeuronNormal : public CNeuron
		{
		public:
			void FeedForward(double Alpha_) override;
			void FeedBackward(double Alpha_, double LearningRate_) override;
		};
	}
}