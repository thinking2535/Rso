#pragma once

#include "Rank.h"
#include <Rso/Util/Random.h>

namespace rso
{
	namespace gameutil
	{
		using namespace std;
		using namespace util;

		template<typename _TKey, typename _TValue, typename _TOp = less<_TKey>>
		class CRandomBox : public CRank<_TKey, _TValue, _TOp>
		{
			std::mt19937_64 _RandomEngine;
			_TKey _AccProbability{};

			using CRank::emplace;
			using CRank::insert;
			using CRank::erase;
			using CRank::clear;
			using CRank::get;

		public:
			void Insert(const _TKey& Key_, const _TValue& Value_)
			{
				if (Key_ <= _TKey{})
					return;

				emplace(_AccProbability, Value_);
				_AccProbability += Key_;
			}
			_TValue& Get(void)
			{
				return get(SUniformDistribution<std::mt19937_64, double>()(_RandomEngine, 0.0, _AccProbability))->second;
			}
		};
	}
}