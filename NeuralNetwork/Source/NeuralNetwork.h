#pragma once

#include "Base.h"
#include "DeepNeuron.h"
#include "RecurrentNeuron.h"
#include "LongShortTermNeuron.h"
#include <ppl.h>

namespace rso::neuralnetwork
{
	using namespace concurrency;

	struct SSynapseLayer
	{
		vector<vector<double>> Synapses;

		SSynapseLayer(size_t PrevLayerSize_, size_t NextLayerSize_) :
			Synapses(PrevLayerSize_)
		{
			for (auto& s : Synapses)
			{
				s.reserve(NextLayerSize_);

				for (size_t i = 0; i < NextLayerSize_; ++i)
					s.push_back(GetRandom(PrevLayerSize_, NextLayerSize_));
			}
		}
	};

	template<typename TNeuron>
	class CNeuralNetwork
	{
	public:
		using TLayer = vector<TNeuron>;
		using TLayers = vector<TLayer>;

	private:
		double _LearningRate = 1.0;
		TLayers _Layers;
		vector<SSynapseLayer> _SynapseLayers;

	public:
		CNeuralNetwork(double LearningRate_, const initializer_list<size_t>& LayerSizes_) : // ��ü NeuronGroup�� �� Layer ���� 3 �̻��� �ǵ���.
			_LearningRate(LearningRate_),
			_Layers(LayerSizes_.size() < 3 ? throw : LayerSizes_.size())
		{
			vector<size_t> LayerSizes = LayerSizes_;

			for (size_t i = 0; i < LayerSizes.size(); ++i)
			{
				if (LayerSizes[i] < 1)
					throw;

				_Layers[i].resize(LayerSizes[i]);
			}

			_SynapseLayers.reserve(LayerSizes.size() - 1);
			for (size_t i = 0; i < LayerSizes.size() - 1; ++i)
				_SynapseLayers.emplace_back(LayerSizes[i], LayerSizes[i + 1]);
		}
		void FeedForward(void)
		{
			for (int l = 1; l < _Layers.size(); ++l)
				parallel_for(0, (const int)_Layers[l].size(), [&](int i) {
					double Input = 0.0;

					for (int p = 0; p < _Layers[l - 1].size(); ++p)
						Input += (_Layers[l - 1][p].GetOutput() * _SynapseLayers[l - 1].Synapses[p][i]);

					_Layers[l][i].FeedForward(Input);});
		}
		void FeedBackward(void)
		{
			// �� �������� �� ���̾�� ���� ������, parallel �� ó���Ϸ��� �� �������� �� ���̾�κ��� ���� �����ؾ���
			// ��´� �������� ó��
			for (auto l = _Layers.size() - 2; l > 0; --l)
				parallel_for(0, (const int)_Layers[l].size(), [&](int i) {
				double Error = 0.0;

				for (int n = 0; n < _Layers[l + 1].size(); ++n)
				{
					Error += _SynapseLayers[l].Synapses[i][n] * _Layers[l + 1][n].GetGradient();
					_SynapseLayers[l].Synapses[i][n] += _Layers[l][i].GetOutput() * _Layers[l + 1][n].GetGradient();
				}

				_Layers[l][i].FeedBackward(Error);});
		}
		inline CNeuralNetwork::TLayer& GetInputLayer(void)
		{
			return _Layers.front();
		}
		inline CNeuralNetwork::TLayer& GetOutputLayer(void)
		{
			return _Layers.back();
		}
		void SetTarget(size_t Index_, double Target_)
		{
			_Layers.back()[Index_].FeedBackward((Target_ - _Layers.back()[Index_].GetOutput()) * _LearningRate);
		}
	};

	using TNeuralNetworkDNN = CNeuralNetwork<CDeepNeuron>;
	using TNeuralNetworkRNN = CNeuralNetwork<CRecurrentNeuron>;
	using TNeuralNetworkLSTM = CNeuralNetwork<CLongShortTermNeuron>;
}
