#pragma once

#include "Base.h"
#include "Neuron.h"
#include "Synapse.h"

namespace rso::neuralnetwork
{
	class CNeuronGroup : public INeuronGroup
	{
		bool _Freeze = false;
		TLayers _Layers;

	public:
		CNeuronGroup(const initializer_list<size_t>& LayerSizes_); // ��ü NeuronGroup�� �� Layer ���� 3 �̻��� �ǵ���.
		void FeedForward(void);
		void FeedBackward(void);
		inline const TLayer& GetInputLayer(void) const
		{
			return _Layers.front();
		}
		inline const TLayer& GetOutputLayer(void) const
		{
			return _Layers.back();
		}
		inline void SetFreeze(bool Freeze_) noexcept
		{
			_Freeze = Freeze_;
		}
		unique_ptr<INeuronGroup> Clone(void);
	};
}
