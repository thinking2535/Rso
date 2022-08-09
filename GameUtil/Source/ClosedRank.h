#pragma once

// 기본적으로 이름이 랭크 이므로 Key를 내림차순으로 정렬하였을 때 가장 처음값 이상이면 첫 값을 출력함.
// 오름차순으로 정렬하여 Key가 처음값 이하이면 첫 값을 출력하려면 연산자로 greater<> 을 써야함.

#include "Base.h"

namespace rso::gameutil
{
	using namespace std;

	template<typename _TKey, typename _TValue, typename _TOp = less<_TKey>>
	class CClosedRank : public map<_TKey, _TValue, _TOp>
	{
	public:
		using typename map<_TKey, _TValue, _TOp>::const_iterator;
		using typename map<_TKey, _TValue, _TOp>::iterator;

		const_iterator get(_TKey Key_) const
		{
			auto it = lower_bound(Key_);
			if (it == end() && !empty())
				--it;

			return it;
		}
		iterator get(_TKey Key_)
		{
			auto it = lower_bound(Key_);
			if (it == end() && !empty())
				--it;

			return it;
		}
	};
}