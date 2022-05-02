#pragma once

#include <Rso/Core/Core.h>
#pragma comment( lib, "Rso/Brain_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <Rso/Math/Base.h>
#include <vector>
#include <list>
#include <algorithm>
#include <random>

namespace rso::brain
{
	using namespace std;
	using namespace core;
	using namespace math;

	class CSynapse;
	class CNeuron;
	class CNeuronRecurrent;
	using TNeuron = unique_ptr<CNeuron>;
	using TLayer = vector<TNeuron>;
	using TLayers = vector<TLayer>;
	using TEndLayer = vector<CNeuron*>;

	__interface INeuronGroup
	{
		virtual void FeedForward(double Alpha_);
		virtual void FeedBackward(double Alpha_);
		virtual const TEndLayer& GetInputLayer(void) const;
		virtual const TEndLayer& GetOutputLayer(void) const;
	};
	using TNeuronGroups = vector<INeuronGroup*>;

	inline double GetRandom(size_t PrevLayerSize_, size_t /*NextLayerSize_*/)
	{
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

	inline double Activate(double Alpha_, double Value_)
	{
		//return Sigmoid(Value_); // Sigmoid
		//return tanh(Value_); // TanH
		//return (Value_ > 0.0 ? Value_ : 0.0); // ReLU
		//return (Value_ > 0.0 ? Value_ : 0.01 * Value_); // Leaky ReLU
		return (Value_ > 0.0 ? Value_ : Alpha_ * (exp(Value_) - 1)); // ELU
	}
	inline double DerivativeActivate(double Alpha_, double Value_)
	{
		//return DerivativeSigmoid(Value_); // Sigmoid
		//return [](double TanH_) { return (1.0 - TanH_) * (1.0 + TanH_); }(tanh(Value_)); // TanH
		//return (Value_ > 0.0 ? 1.0 : 0.0); // ReLU
		//return (Value_ > 0.0 ? 1.0 : 0.01); // Leaky ReLU
		return (Value_ > 0.0 ? 1.0 : Alpha_ * exp(Value_)); // ELU
	}
}
