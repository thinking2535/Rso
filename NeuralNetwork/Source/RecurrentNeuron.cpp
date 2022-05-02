#include "RecurrentNeuron.h"

namespace rso::neuralnetwork
{
	void CRecurrentNeuron::FeedForward(double Input_) noexcept
	{
		_Input = Input_ + _StateInput * _StateInputWeight + _StateBias;
		_StateOutput = tanh(_Input);
		_Output = _StateOutput * _StateOutputWeight + _Bias;
	}
	void CRecurrentNeuron::FeedBackward(double Error_) noexcept
	{
		_Bias += Error_;
		_Gradient = DerivativeTanH(_Input) * _StateOutputWeight * Error_;
		_StateOutputWeight += _StateOutput * Error_;
		_StateInputWeight += _StateInput * _Gradient;
		_StateBias += _Gradient;
		_StateInput = _StateOutput;
	}
}
