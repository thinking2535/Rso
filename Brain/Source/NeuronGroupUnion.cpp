#include "NeuronGroupUnion.h"

namespace rso::brain
{
	CNeuronGroupUnion::CNeuronGroupUnion(const TNeuronGroups& NeuronGroups_) :
		_NeuronGroups(NeuronGroups_)
	{
	}
	void CNeuronGroupUnion::FeedForward(double Alpha_)
	{
		for (auto& i : _NeuronGroups)
			i->FeedForward(Alpha_);
	}
	void CNeuronGroupUnion::FeedBackward(double Alpha_)
	{
		for (auto& i : reverse(_NeuronGroups))
			i->FeedBackward(Alpha_);
	}
}
