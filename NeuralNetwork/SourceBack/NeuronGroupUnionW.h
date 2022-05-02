#pragma once

#include "NeuronGroupUnion.h"

namespace rso::neuralnetwork
{
	class CNeuronGroupUnionW : public CNeuronGroupUnion
	{
		TLayer _InputLayer;
		TLayer _OutputLayer;

	public:
		CNeuronGroupUnionW(const TNeuronGroups& NeuronGroups_);
		inline const TLayer& GetInputLayer(void) const
		{
			return _InputLayer;
		}
		inline const TLayer& GetOutputLayer(void) const
		{
			return _OutputLayer;
		}
		unique_ptr<INeuronGroup> Clone(void);
	};
}
