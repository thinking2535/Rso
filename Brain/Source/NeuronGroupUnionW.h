#pragma once

#include "NeuronGroupUnion.h"

namespace rso::brain
{
	class CNeuronGroupUnionW : public CNeuronGroupUnion
	{
		TEndLayer _InputLayer;
		TEndLayer _OutputLayer;

	public:
		CNeuronGroupUnionW(const TNeuronGroups& NeuronGroups_);
		inline const TEndLayer& GetInputLayer(void) const override
		{
			return _InputLayer;
		}
		inline const TEndLayer& GetOutputLayer(void) const override
		{
			return _OutputLayer;
		}
	};
}
