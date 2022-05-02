#pragma once

#include "Base.h"
#include <random>

namespace rso::util
{
	using namespace std;

	template<typename _TEngine, typename _TType>
	struct SUniformDistribution
	{
		_TType operator()(_TEngine& Engine_, _TType Min_, _TType Max_)
		{
			return uniform_int_distribution<_TType>(Min_, Max_)(Engine_);
		}
	};
	template<typename _TEngine>
	struct SUniformDistribution<_TEngine, float>
	{
		float operator()(_TEngine& Engine_, float Min_, float Max_)
		{
			return uniform_real_distribution<float>(Min_, Max_)(Engine_);
		}
	};
	template<typename _TEngine>
	struct SUniformDistribution<_TEngine, double>
	{
		double	operator()(_TEngine& Engine_, double Min_, double Max_)
		{
			return uniform_real_distribution<double>(Min_, Max_)(Engine_);
		}
	};

	template<typename _TEngine, typename _TType>
	class CRandom
	{
		std::mt19937_64 _Engine;
		_TType _Min{};
		_TType _Max{};

	public:
		CRandom(_TType Min_, _TType Max_) :
			_Min(Min_), _Max(Max_)
		{
		}
		_TType Get(void)
		{
			return SUniformDistribution<_TEngine, _TType>()(_Engine, _Min, _Max);
		}
	};
}