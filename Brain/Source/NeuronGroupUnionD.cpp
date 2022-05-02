#include "NeuronGroupUnionD.h"

namespace rso::brain
{
	CNeuronGroupUnionD::CNeuronGroupUnionD(const TNeuronGroups& NeuronGroups_) :
		CNeuronGroupUnion(NeuronGroups_)
	{
		for (size_t i = 0; i < NeuronGroups_.size() - 1; ++i)
		{
			for (auto& f : NeuronGroups_[i]->GetOutputLayer())
				for (auto& t : NeuronGroups_[i + 1]->GetInputLayer())
					f->Link(NeuronGroups_[i]->GetOutputLayer().size(), NeuronGroups_[i + 1]->GetInputLayer().size(), t);
		}
	}
}
