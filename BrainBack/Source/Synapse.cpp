#include "Synapse.h"
#include "Neuron.h"

namespace rso
{
	namespace brain
	{
		CSynapse::CSynapse(size_t PrevLayerSize_, size_t NextLayerSize_, CNeuron* PrevNeuron_, CNeuron* NextNeuron_) :
			_Weight(GetRandom(PrevLayerSize_, NextLayerSize_)),
			_PrevNeuron(PrevNeuron_),
			_NextNeuron(NextNeuron_)
		{
		}
		void CSynapse::FeedForward(double Input_)
		{
			_NextNeuron->AddInput(Input_ * _Weight);
		}
		void CSynapse::FeedBackward(double Gradient_, double LearningRate_)
		{
			_PrevNeuron->AddError(Gradient_ * _Weight);
			_Weight += Gradient_ * _PrevNeuron->GetOutput() * LearningRate_;
		}
	}
}