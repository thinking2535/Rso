#pragma once

#include "Base.h"
#include "NeuronNormal.h"
#include "NeuronRecurrent.h"
#include "Synapse.h"

namespace rso
{
	namespace brain
	{
		template <typename TNeuron>
		class CBrain
		{
		public:
			using TLayer = vector<CNeuron*>;
			using TLayers = vector<TLayer>;

		private:
			double _Alpha = 1.0;
			double _LearningRate = 1.0;
			TLayers _Layers;

		public:
			CBrain(double Alpha_, double LearningRate_, const initializer_list<size_t>& LayerSizes_, TLayer*& Input_, TLayer*& Output_) :
				_Alpha(Alpha_),
				_LearningRate(LearningRate_),
				_Layers(LayerSizes_.size() > 1 ? LayerSizes_.size() : THROWEX())
			{
				vector<size_t> LayerSizes = LayerSizes_;

				for (size_t i = 0; i < LayerSizes.size(); ++i)
				{
					for (size_t j = 0; j < LayerSizes[i]; ++j)
						_Layers[i].emplace_back(new TNeuron());
				}

				for (size_t l = 0; l < _Layers.size() - 1; ++l)
					for (auto& f : _Layers[l])
						for (auto& t : _Layers[l + 1])
							f->Link(_Layers[l].size(), _Layers[l + 1].size(), t);

				Input_ = &_Layers.front();
				Output_ = &_Layers.back();
			}
			CBrain(const CBrain&) = delete;
			void FeedForward(void)
			{
				for (size_t l = 0; l < _Layers.size() - 1; ++l)
				{
					for (auto& n : _Layers[l + 1])
						n->ClearInput();

					for (auto& n : _Layers[l])
						n->FeedForward(_Alpha);
				}

				for (auto& n : _Layers.back())
					n->FeedForward(_Alpha);
			}
			void FeedBackward(void)
			{
				for (auto& l : reverse(_Layers))
					for (auto& n : l)
						n->FeedBackward(_Alpha, _LearningRate);
			}
			CBrain& operator = (const CBrain&) = delete;
			inline const TLayers& GetLayers(void) const
			{
				return _Layers;
			}
			void PrintBiasAndWeights(ostream& o_) const
			{
				for (auto& l : _Layers)
				{
					for (auto& n : l)
						n->PrintBiasAndWeights(o_);
					o_ << endl;
				}
			}
			void SetBiasAndWeights(size_t Layer_, size_t Node_, double Bias_, const initializer_list<double>& Weights_)
			{
				_Layers[Layer_][Node_]->SetBiasAndWeights(Bias_, Weights_);
			}
			SLayers ToLayers(void) const
			{
				SLayers Layers;

				for (auto& l : _Layers)
				{
					Layers.Layers.emplace_back(SLayer());
					for (auto& n : l)
						Layers.Layers.back().Neurons.emplace_back(n->GetSNeuron());
				}

				return Layers;
			}
		};
	}
}
