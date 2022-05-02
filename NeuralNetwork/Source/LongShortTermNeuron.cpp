#include "LongShortTermNeuron.h"

namespace rso::neuralnetwork
{
	void CLongShortTermNeuron::FeedForward(double Input_) noexcept
	{
		_Input = Input_;
		_MemoryOutput =
			_MemoryInput * _FunctionF.FeedForward(Input_, _StateInput) +
			_FunctionI.FeedForward(Input_, _StateInput) * _FunctionG.FeedForward(Input_, _StateInput);

		_MemoryOutputActivate = tanh(_MemoryOutput);
		_StateOutput = _MemoryOutputActivate * _FunctionO.FeedForward(Input_, _StateInput);
		_Output = _StateOutput * _StateOutputWeight + _Bias;
	}
	void CLongShortTermNeuron::FeedBackward(double Error_) noexcept
	{
		_Bias += Error_;
		const auto StateOutputDerivative = _StateOutputWeight * Error_; // dHt
		_StateOutputWeight += _StateOutput * Error_;
		const auto MemoryOutputDerivative = DerivativeTanH(_MemoryOutput) * _FunctionO.GetOutput() * StateOutputDerivative; // dCt
		_FunctionO.FeedBackward(_Input, _StateInput, _MemoryOutputActivate * StateOutputDerivative);
		_FunctionF.FeedBackward(_Input, _StateInput, _MemoryInput * MemoryOutputDerivative);
		_MemoryInput = _MemoryOutput;
		_FunctionI.FeedBackward(_Input, _StateInput, _FunctionG.GetOutput() * MemoryOutputDerivative);
		_FunctionG.FeedBackward(_Input, _StateInput, _FunctionI.GetOutput() * MemoryOutputDerivative);

		_StateInput = _StateOutput;
		_Gradient = _FunctionF.GetGradient() + _FunctionI.GetGradient() + _FunctionG.GetGradient() + _FunctionO.GetGradient();
	}
}
