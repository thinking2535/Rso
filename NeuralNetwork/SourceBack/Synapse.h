#pragma once

#include "Base.h"

namespace rso::neuralnetwork
{
	class CSynapse
	{
		friend CNeuron;

		double _Weight = 0.0;
		CNeuron* _PrevNeuron;

		void CopyTo(CSynapse* Synapse_);

	public:
		CSynapse(size_t PrevLayerSize_, CNeuron* PrevNeuron_);
		void UpdateWeight(double Gradient_);
		double FeedForward(void);
		void FeedBackward(double Gradient_);
	};

}