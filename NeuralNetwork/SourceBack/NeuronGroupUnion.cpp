#include "NeuronGroupUnion.h"

namespace rso::neuralnetwork
{
	CNeuronGroupUnion::CNeuronGroupUnion(const TNeuronGroups& NeuronGroups_) :
		_NeuronGroups(NeuronGroups_.size() < 2 ? throw "NeuronGroups_.size() < 2" : NeuronGroups_)
	{
	}
	void CNeuronGroupUnion::FeedForward(void)
	{
		for (auto& i : _NeuronGroups)
			i->FeedForward();
	}
	void CNeuronGroupUnion::FeedBackward(void)
	{
		for (auto& i : reverse(_NeuronGroups))
			i->FeedBackward();
	}
	void CNeuronGroupUnion::SetFreeze(bool Freeze_)
	{
		for (auto& i : _NeuronGroups)
			i->SetFreeze(Freeze_);
	}
}
