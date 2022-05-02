#include "NeuronGroupUnionW.h"

namespace rso::neuralnetwork
{
	CNeuronGroupUnionW::CNeuronGroupUnionW(const TNeuronGroups& NeuronGroups_) :
		CNeuronGroupUnion(NeuronGroups_)
	{
		for (auto& i : NeuronGroups_)
		{
			_InputLayer.insert(_InputLayer.end(), i->GetInputLayer().begin(), i->GetInputLayer().end());
			_OutputLayer.insert(_OutputLayer.end(), i->GetOutputLayer().begin(), i->GetOutputLayer().end());
		}

		_InputLayer.shrink_to_fit();
		_OutputLayer.shrink_to_fit();
	}
	unique_ptr<INeuronGroup> CNeuronGroupUnionW::Clone(void)
	{
		throw 0;

		return unique_ptr<INeuronGroup>();
	}
}
