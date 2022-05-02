#pragma once


#include <Rso/Core/Core.h>
#pragma comment( lib, "Rso/Brain_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <Rso/Math/Base.h>
#include <vector>
#include <list>
#include <algorithm>
#include <random>

namespace rso
{
	namespace brain
	{
		using namespace std;
		using namespace core;
		using namespace math;

		inline double GetRandom(size_t /*PrevLayerSize_*/, size_t /*NextLayerSize_*/)
		{
			// Sigmoid �� Tanh �� ����� ��쿡�� Xavier �ʱ�ȭ 
			// ReLU �� ����� ��쿡�� He �ʱ�ȭ

			std::random_device rd;
			std::mt19937 gen(rd());

			// My Random
			return std::uniform_real_distribution<>(-0.5, 0.5)(gen);

			// Xavier /////////////////////////////////////////////////////////
			//return std::uniform_real_distribution<>(double(min(-int(PrevLayerSize_), -int(NextLayerSize_))), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_));

			// He /////////////////////////////////////////////////////////
			//return std::uniform_real_distribution<>(double(min(-int(PrevLayerSize_), -int(NextLayerSize_))), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_) * 0.5);
			//���ͳ� �׽�Ʈ������ ������ �����µ� ����� ����� ����.
			//return std::uniform_real_distribution<>(double(min(PrevLayerSize_, NextLayerSize_)), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_) * 0.5);
		}
		
		inline double Activate(double Alpha_, double Value_)
		{
			//return Sigmoid(Value_); // Sigmoid
			//return (Value_ > 0.0 ? Value_ : 0.0); // ReLU
			//return (Value_ > 0.0 ? Value_ : 0.01 * Value_); // Leaky ReLU
			return (Value_ > 0.0 ? Value_ : Alpha_ * (exp(Value_) - 1)); // ELU
		}
		inline double DerivativeActivate(double Alpha_, double Value_)
		{
			//return DerivativeSigmoid(Value_); // Sigmoid
			//return (Value_ > 0.0 ? 1.0 : 0.0); // ReLU
			//return (Value_ > 0.0 ? 1.0 : 0.01); // Leaky ReLU
			return (Value_ > 0.0 ? 1.0 : Alpha_ * exp(Value_)); // ELU
		}
	}
}
