#pragma once

#include "Base.h"
#include "Neuron.h"
#include "Synapse.h"

namespace rso::brain
{
	class CNeuronGroup : public INeuronGroup
	{
	protected:
		TLayers _Layers;
		TEndLayer _InputLayer;
		TEndLayer _OutputLayer;

	public:
		CNeuronGroup(const initializer_list<size_t>& LayerSizes_);
		void FeedForward(double Alpha_) override;
		void FeedBackward(double Alpha_) override;
		inline const TEndLayer& GetInputLayer(void) const noexcept override
		{
			return _InputLayer;
		}
		inline const TEndLayer& GetOutputLayer(void) const noexcept override
		{
			return _OutputLayer;
		}
		inline const TLayers& GetLayers(void) const noexcept
		{
			return _Layers;
		}
		void PrintBiasAndWeights(ostream& o_) const;
		void SetBiasAndWeights(size_t Layer_, size_t Node_, double Bias_, const initializer_list<double>& Weights_);
		SLayers ToLayers(void) const;
	};
}
