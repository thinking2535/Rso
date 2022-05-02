#include "Neuron.h"
#include "Synapse.h"

namespace rso::brain
{
	void CNeuron::Link(size_t PrevLayerSize_, size_t NextLayerSize_, CNeuron* Neuron_)
	{
		_NextSynapses.emplace_back(new CSynapse(PrevLayerSize_, NextLayerSize_, this, Neuron_));
		Neuron_->_PrevSynapses.emplace_back(_NextSynapses.back().get());
		Neuron_->_LearningRate = 1.0 / (Neuron_->_PrevSynapses.size() + 1); // 1 : For Bias
	}
	void CNeuron::SetBiasAndWeights(double Bias_, const initializer_list<double>& Weights_)
	{
		_Bias = Bias_;

		size_t i = 0;
		for (auto& w : Weights_)
			_NextSynapses[i++]->SetWeight(w);
	}
	SNeuron CNeuron::GetSNeuron(void) const
	{
		list<double> Synapses;
		for (auto& s : _NextSynapses)
			Synapses.emplace_back(s->GetWeight());
		return SNeuron(_Input, _Output, _Bias, std::move(Synapses));
	}
	void CNeuron::PrintBiasAndWeights(ostream& o_) const
	{
		o_ << _Bias << ", {";

		auto s = _NextSynapses.begin();
		if (s != _NextSynapses.end())
		{
			o_ << (*s)->GetWeight();

			for (++s; s != _NextSynapses.end(); ++s)
				o_ << ", " << (*s)->GetWeight();
		}
		o_ << "}" << endl;
	}
	void CNeuron::FeedForward(double Alpha_)
	{
		if (!_PrevSynapses.empty())
		{
			_Sum = _Input + _Bias;
			_Output = Activate(Alpha_, _Sum);
		}

		_Input = 0.0;

		for (auto& s : _NextSynapses)
			s->FeedForward(_Output);
	}
	void CNeuron::FeedBackward(double Alpha_)
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
			if (!_PrevSynapses.empty())
				_Bias += Gradient_;

			for (auto& s : _PrevSynapses)
				s->FeedBackward(Gradient_, _LearningRate);
		}
		(_Error * DerivativeActivate(Alpha_, _Sum));

		_Error = 0.0;
	}
}
