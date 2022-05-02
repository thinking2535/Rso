#include "NeuronRecurrent.h"

namespace rso::brain
{
	void CNeuronRecurrent::FeedForward(double Alpha_)
	{
		if (!_PrevSynapses.empty())
		{
			_Sum = _Input + _Bias + (_State * _StateWeight);
			_State = tanh(_Sum);
		}

		_Input = 0.0;

		for (auto& s : _NextSynapses)
			s->FeedForward(_Output);
	}
}