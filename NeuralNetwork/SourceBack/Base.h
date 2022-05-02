#pragma once

#include <Rso/Core/Core.h>
#pragma comment( lib, "Rso/NeuralNetwork_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <Rso/Math/Base.h>
#include <vector>
#include <list>
#include <algorithm>
#include <random>

namespace rso::neuralnetwork
{
	using namespace std;
	using namespace core;
	using namespace math;

	class CSynapse;
	class CNeuron;
	class CNeuronGroup;
	class CNeuronGroupUnionD;
	class CNeuronGroupUnionW;
	using TLayer = vector<shared_ptr<CNeuron>>;
	using TLayers = vector<TLayer>;

	__interface INeuronGroup
	{
		void FeedForward(void);
		void FeedBackward(void);
		const TLayer& GetInputLayer(void) const;
		const TLayer& GetOutputLayer(void) const;
		void SetFreeze(bool Freeze_);
		unique_ptr<INeuronGroup> Clone(void);
	};
	using TNeuronGroups = vector<INeuronGroup*>;

	constexpr double c_GlobalLearningRate = 0.5;

	inline double GetRandom(size_t PrevLayerSize_)
	{
		// debug ////////
		//auto r = (rand() % 200) / 100.0 - 1.0;
		//return r;
		/////////////////


		// Sigmoid 나 Tanh 를 사용할 경우에는 Xavier 초기화 
		// ReLU 를 사용할 경우에는 He 초기화

		//static std::random_device rd;
		//static std::mt19937_64 gen(rd());
		static std::mt19937_64 gen;

		// My He
		const auto Random = sqrt(6.0 / PrevLayerSize_);

		return std::uniform_real_distribution<>(-Random, Random)(gen);
		//return std::uniform_real_distribution<>(double(min(-int(PrevLayerSize_), -int(NextLayerSize_))), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_) * 2.0);

//			return std::uniform_real_distribution<>(double(min(-int(PrevLayerSize_), -int(NextLayerSize_))), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_) * 2.0);

			// Xavier /////////////////////////////////////////////////////////
			//return std::uniform_real_distribution<>(double(min(-int(PrevLayerSize_), -int(NextLayerSize_))), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_));

			// He /////////////////////////////////////////////////////////
			//return std::uniform_real_distribution<>(double(min(-int(PrevLayerSize_), -int(NextLayerSize_))), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_) * 2.0);
			//인터넷 테스트에서는 음수도 나오는데 여기는 양수만 나옴.
			//return std::uniform_real_distribution<>(double(min(PrevLayerSize_, NextLayerSize_)), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_) * 2.0);
	}

	inline double Activate(double Value_)
	{
		//return Sigmoid(Value_); // Sigmoid
		//return tanh(Value_); // TanH
		//return (Value_ > 0.0 ? Value_ : 0.0); // ReLU
		//return (Value_ > 0.0 ? Value_ : 0.01 * Value_); // Leaky ReLU
		return (Value_ > 0.0 ? Value_ : (exp(Value_) - 1)); // ELU
	}
	inline double DerivativeActivate(double Value_)
	{
		//return DerivativeSigmoid(Value_); // Sigmoid
		//return [](double TanH_) { return (1.0 - TanH_) * (1.0 + TanH_); }(tanh(Value_)); // TanH
		//return (Value_ > 0.0 ? 1.0 : 0.0); // ReLU
		//return (Value_ > 0.0 ? 1.0 : 0.01); // Leaky ReLU
		return (Value_ > 0.0 ? 1.0 : exp(Value_)); // ELU
	}
}
