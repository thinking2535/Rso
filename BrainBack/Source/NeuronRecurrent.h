#pragma once

#include "Neuron.h"

namespace rso
{
	namespace brain
	{
		class CNeuronRecurrent : public CNeuron
		{
			double _OldH = 0.0;
			double _NewH = 0.0;
			double _WeightH = 0.0;
			double _WeightO = 0.0;
			double _BiasO = 0.0;

		public:
			void FeedForward(double Alpha_) override;
			void FeedBackward(double Alpha_, double LearningRate_) override;
		};
	}
}