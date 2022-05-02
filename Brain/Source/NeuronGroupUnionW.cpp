#include "NeuronGroupUnionW.h"

namespace rso::brain
{
	CNeuronGroupUnionW::CNeuronGroupUnionW(const TNeuronGroups& NeuronGroups_) :
		CNeuronGroupUnion(NeuronGroups_)
	{
		for (auto& i : NeuronGroups_)
		{
			_InputLayer.insert(_InputLayer.end(), i->GetInputLayer().begin(), i->GetInputLayer().end());
			_OutputLayer.insert(_OutputLayer.end(), i->GetOutputLayer().begin(), i->GetOutputLayer().end());
		}
	}
}
