#include "NeuronNormal.h"
#include "Synapse.h"

namespace rso
{
	namespace brain
	{
		void CNeuronNormal::FeedForward(double Alpha_)
		{
			_Sum = _Input + _Bias;
			_Output = Activate(Alpha_, _Sum);

			for (auto& s : _NextSynapses)
				s->FeedForward(_Output);
		}
		void CNeuronNormal::FeedBackward(double Alpha_, double LearningRate_)
		{
			// O = f(W * X + B)
			// S = W * X + B
			// O = f(S)
			// E = 1/2(O - T)^2
			// 
			// dE/dO = O - T
			// dO/dS = f'(S)
			// dS/dW = X
			// dS/dX = W
			// dS/dB = 1
			// 
			// dE/dW = dE/dO * dO/dS * dS/dW = (O - T) * f'(S) * X
			// dE/dX = dE/dO * dO/dS * dS/dW = (O - T) * f'(S) * W
			// dE/dB = dE/dO * dO/dS * dS/dB = (O - T) * f'(S)

			[&](double Gradient_)
			{
				_Bias += Gradient_ * LearningRate_;

				for (auto& s : _PrevSynapses)
					s->FeedBackward(Gradient_, LearningRate_);
			}
			(_Error * DerivativeActivate(Alpha_, _Sum));

			_Error = 0.0;
		}
	}
}