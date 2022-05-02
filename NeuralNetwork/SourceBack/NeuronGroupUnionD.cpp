#include "NeuronGroupUnionD.h"

namespace rso::neuralnetwork
{
	CNeuronGroupUnionD::CNeuronGroupUnionD(const TNeuronGroups& NeuronGroups_) :
		CNeuronGroupUnion(NeuronGroups_)
	{
		for (size_t i = 0; i < NeuronGroups_.size() - 1; ++i)
		{
			for (auto& n : NeuronGroups_[i + 1]->GetInputLayer())
				n->Link(NeuronGroups_[i]->GetOutputLayer());
		}
	}
	unique_ptr<INeuronGroup> CNeuronGroupUnionD::Clone(void)
	{
		throw 0;

		return unique_ptr<INeuronGroup>();
	}
}
