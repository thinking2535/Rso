#include "NeuronGroup.h"

namespace rso::neuralnetwork
{
	CNeuronGroup::CNeuronGroup(const initializer_list<size_t>& LayerSizes_) :
		_Layers(LayerSizes_.size())
	{
		vector<size_t> LayerSizes = LayerSizes_;

		for (size_t i = 0; i < LayerSizes.size(); ++i)
		{
			if (LayerSizes[i] < 1)
				throw;

			_Layers[i].reserve(LayerSizes[i]);

			for (size_t j = 0; j < LayerSizes[i]; ++j)
				_Layers[i].emplace_back(new CNeuron());
		}

		for (size_t l = 0; l < _Layers.size() - 1; ++l)
			for (auto& n : _Layers[l + 1])
				n->Link(_Layers[l]);
	}
	void CNeuronGroup::FeedForward(void)
	{
		for (auto& l : _Layers)
			for (auto& n : l)
				n->FeedForward();
	}
	void CNeuronGroup::FeedBackward(void)
	{
		if (_Freeze)
			return;

		for (auto& l : reverse(_Layers))
			for (auto& n : l)
				n->FeedBackward();
	}
	unique_ptr<INeuronGroup> CNeuronGroup::Clone(void)
	{
		vector<size_t> il;
		transform(_Layers.begin(), _Layers.end(), back_inserter(il), [](const TLayer& Data_) { return Data_.size(); });

		auto Ret = make_unique<CNeuronGroup>(initializer_list<size_t>(il.data(), il.data() + il.size()));

		for (size_t l = 0; l < _Layers.size(); ++l)
		{
			for (size_t n = 0; n < _Layers[l].size(); ++n)
				_Layers[l][n]->CopyTo(Ret->_Layers[l][n].get());
		}

		Ret->_Freeze = _Freeze;

		return Ret;
	}
}
