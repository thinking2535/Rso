#include "Brain.h"

namespace rso::brain
{
	CBrain::CBrain(double Alpha_, INeuronGroup* NeuronGroup_) :
		_Alpha(Alpha_),
		_NeuronGroup(NeuronGroup_)
	{
	}
}
