#include "NeuronGroup.h"

namespace rso::brain
{
	CNeuronGroup::CNeuronGroup(const initializer_list<size_t>& LayerSizes_) :
		_Layers(LayerSizes_.size())
	{
		vector<size_t> LayerSizes = LayerSizes_;

		for (size_t i = 0; i < LayerSizes.size(); ++i)
		{
			for (size_t j = 0; j < LayerSizes[i]; ++j)
				_Layers.at(i).emplace_back(make_unique<CNeuron>());
		}

		for (size_t l = 0; l < _Layers.size() - 1; ++l)
			for (auto& f : _Layers.at(l))
				for (auto& t : _Layers.at(l + 1))
					f->Link(_Layers.at(l).size(), _Layers.at(l + 1).size(), t.get());

		std::transform(_Layers.front().begin(), _Layers.front().end(), std::back_inserter(_InputLayer), [](const TNeuron& Data_) { return Data_.get(); });
		std::transform(_Layers.back().begin(), _Layers.back().end(), std::back_inserter(_OutputLayer), [](const TNeuron& Data_) { return Data_.get(); });
	}
	void CNeuronGroup::FeedForward(double Alpha_)
	{
		for (auto& l : _Layers)
			for (auto& n : l)
				n->FeedForward(Alpha_);
	}
	void CNeuronGroup::FeedBackward(double Alpha_)
	{
		for (auto& l : reverse(_Layers))
			for (auto& n : l)
				n->FeedBackward(Alpha_);
	}
	void CNeuronGroup::PrintBiasAndWeights(ostream& o_) const
	{
		for (auto& l : _Layers)
		{
			for (auto& n : l)
				n->PrintBiasAndWeights(o_);
			o_ << endl;
		}
	}
	void CNeuronGroup::SetBiasAndWeights(size_t Layer_, size_t Node_, double Bias_, const initializer_list<double>& Weights_)
	{
		_Layers[Layer_][Node_]->SetBiasAndWeights(Bias_, Weights_);
	}
	SLayers CNeuronGroup::ToLayers(void) const
	{
		SLayers Layers;

		for (auto& l : _Layers)
		{
			Layers.Layers.emplace_back(SLayer());
			for (auto& n : l)
				Layers.Layers.back().Neurons.emplace_back(n->GetSNeuron());
		}

		return Layers;
	}
}
