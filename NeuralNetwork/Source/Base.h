#pragma once

#include <Rso/Core/Core.h>

#pragma comment( lib, "Rso/NeuralNetwork_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <Rso/Math/Base.h>
#include <random>
#include <functional>

namespace rso::neuralnetwork
{
	using namespace std;
	using namespace core;
	using namespace math;

	inline double GetRandom(size_t PrevLayerSize_, size_t NextLayerSize_)
	{
		// debug ////////
		//auto r = (rand() % 200) / 100.0 - 1.0;
		//return r;
		/////////////////


		// Sigmoid �� Tanh �� ����� ��쿡�� Xavier �ʱ�ȭ 
		// ReLU �� ����� ��쿡�� He �ʱ�ȭ

		//static std::random_device rd;
		//static std::mt19937_64 gen(rd());
		static std::mt19937_64 gen;

		// My He
		const auto Random = sqrt(6.0 / PrevLayerSize_);

//		return std::uniform_real_distribution<>(-Random, Random)(gen);
		//return std::uniform_real_distribution<>(double(min(-int(PrevLayerSize_), -int(NextLayerSize_))), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_) * 2.0);

//			return std::uniform_real_distribution<>(double(min(-int(PrevLayerSize_), -int(NextLayerSize_))), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_) * 2.0);

			// Xavier /////////////////////////////////////////////////////////
			return std::uniform_real_distribution<>(double(min(-int(PrevLayerSize_), -int(NextLayerSize_))), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_));

			// He /////////////////////////////////////////////////////////
			//return std::uniform_real_distribution<>(double(min(-int(PrevLayerSize_), -int(NextLayerSize_))), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_) * 2.0);
			//���ͳ� �׽�Ʈ������ ������ �����µ� ����� ����� ����.
			//return std::uniform_real_distribution<>(double(min(PrevLayerSize_, NextLayerSize_)), double(max(PrevLayerSize_, NextLayerSize_)))(gen) / sqrt(double(PrevLayerSize_) * 2.0);
	}
}
