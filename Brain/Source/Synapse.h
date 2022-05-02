#pragma once

#include "Base.h"

namespace rso::brain
{
	class CSynapse
	{
		double _Weight = 0.0;
		CNeuron* _PrevNeuron;
		CNeuron* _NextNeuron;

	public:
		CSynapse(size_t PrevLayerSize_, size_t NextLayerSize_, CNeuron* PrevNeuron_, CNeuron* NextNeuron_);
		inline double GetWeight(void) const noexcept
		{
			return _Weight;
		}
		inline void SetWeight(double Weight_) noexcept
		{
			_Weight = Weight_;
		}
		void FeedForward(double Input_);
		void FeedBackward(double Gradient_, double LearningRate_);
	};
}
