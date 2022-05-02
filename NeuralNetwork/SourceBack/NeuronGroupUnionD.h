#pragma once

#include "NeuronGroupUnion.h"

namespace rso::neuralnetwork
{
	class CNeuronGroupUnionD : public CNeuronGroupUnion
	{
	public:
		CNeuronGroupUnionD(const TNeuronGroups& NeuronGroups_);
		inline const TLayer& GetInputLayer(void) const
		{
			return _NeuronGroups.front()->GetInputLayer();
		}
		inline const TLayer& GetOutputLayer(void) const
		{
			return _NeuronGroups.back()->GetOutputLayer();
		}
		unique_ptr<INeuronGroup> Clone(void);
	};
}
