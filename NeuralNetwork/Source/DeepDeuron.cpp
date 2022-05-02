#include "DeepNeuron.h"

namespace rso::neuralnetwork
{
	void CDeepNeuron::FeedForward(double Input_) noexcept
	{
		_Input = Input_ + _Bias;
		_Output = ELU(_Input);
	}
	void CDeepNeuron::FeedBackward(double Error_) noexcept
	{
		_Gradient = DerivativeELU(_Input) * Error_;
		_Bias += _Gradient;

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
	}
}
