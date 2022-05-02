#include "Neuron.h"
#include "Synapse.h"

namespace rso
{
	namespace brain
	{
		void CNeuron::Link(size_t PrevLayerSize_, size_t NextLayerSize_, CNeuron* Neuron_)
		{
			_NextSynapses.emplace_back(new CSynapse(PrevLayerSize_, NextLayerSize_, this, Neuron_));
			Neuron_->_PrevSynapses.emplace_back(_NextSynapses.back().get());
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
	}
}