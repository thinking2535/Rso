#pragma once

#include "NeuronGroupUnion.h"

namespace rso::brain
{
	class CNeuronGroupUnionD : public CNeuronGroupUnion
	{
	public:
		CNeuronGroupUnionD(const TNeuronGroups& NeuronGroups_);
		inline const TEndLayer& GetInputLayer(void) const override
		{
			return _NeuronGroups.front()->GetInputLayer();
		}
		inline const TEndLayer& GetOutputLayer(void) const override
		{
			return _NeuronGroups.back()->GetOutputLayer();
		}
	};
}
