#include "Synapse.h"
#include "Neuron.h"

namespace rso::neuralnetwork
{
	void CSynapse::CopyTo(CSynapse* Synapse_)
	{
		Synapse_->_Weight = _Weight;
	}
	CSynapse::CSynapse(size_t PrevLayerSize_, CNeuron* PrevNeuron_) :
		_Weight(GetRandom(PrevLayerSize_)),
		_PrevNeuron(PrevNeuron_)
	{
	}
	void CSynapse::UpdateWeight(double Gradient_)
	{
		_Weight += Gradient_ * _PrevNeuron->GetOutput();
	}
	double CSynapse::FeedForward(void)
	{
		return _PrevNeuron->GetOutput() * _Weight;
	}
	void CSynapse::FeedBackward(double Gradient_)
	{
		_PrevNeuron->AddError(Gradient_ * _Weight);
		UpdateWeight(Gradient_);
	}
}