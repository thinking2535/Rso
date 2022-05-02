#include "Neuron.h"
#include "Synapse.h"

namespace rso::neuralnetwork
{
	void CNeuron::Link(const TLayer& PrevLayer_)
	{
		_PrevSynapses.reserve(PrevLayer_.size());

		for (auto& n : PrevLayer_)
			_PrevSynapses.emplace_back(new CSynapse(PrevLayer_.size(), n.get()));

		_LearningRate = 1.0 / (PrevLayer_.size() + 1); // 1 for bias
	}
	void CNeuron::FeedForward(void)
	{
		if (_PrevSynapses.empty())
			return;

		_Input = 0.0;

		for (auto& s : _PrevSynapses)
			_Input += s->FeedForward();

		_Input = _Input + _Bias;
		_Output = Activate(_Input);
	}
	void CNeuron::FeedBackward(void)
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

		if (_PrevSynapses.empty())
			return;

		const auto Gradient = _Error * DerivativeActivate(_Input) * _LearningRate;

		_Bias += Gradient;

		for (auto& s : _PrevSynapses)
			s->FeedBackward(Gradient);

		_Error = 0.0;
	}
	void CNeuron::CopyTo(CNeuron* Neuron_)
	{
		Neuron_->_LearningRate = _LearningRate;
		Neuron_->_Bias = _Bias;
		Neuron_->_Input = _Input;
		Neuron_->_Output = _Output;
		Neuron_->_Error = _Error;

		for (size_t i = 0; i < _PrevSynapses.size(); ++i)
			_PrevSynapses[i]->CopyTo(Neuron_->_PrevSynapses[i].get());
	}
}
