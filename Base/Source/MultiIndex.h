#pragma once

#include <tuple>
#include <map>
#include <unordered_map>
#include <Rso/Base/List.h>

namespace rso::base
{
	using namespace std;

	template<typename TKey, class _TCompare = less<TKey>>
	struct unique_index : public std::map<TKey, std::size_t, _TCompare>
	{
		typename std::map<TKey, std::size_t, _TCompare>::iterator emplace(const TKey& Key_, std::size_t DataIndex_)
		{
			auto ib = std::map<TKey, std::size_t, _TCompare>::emplace(Key_, DataIndex_);
			if (!ib.second)
				throw 0;

			return ib.first;
		}
	};
	template<typename TKey, class _TCompare = less<TKey>>
	struct non_unique_index : public std::multimap<TKey, std::size_t, _TCompare>
	{
		typename std::multimap<TKey, std::size_t, _TCompare>::iterator emplace(const TKey& Key_, std::size_t DataIndex_)
		{
			return std::multimap<TKey, std::size_t, _TCompare>::emplace(Key_, DataIndex_);
		}
	};
	template<typename TKey, class _TCompare = less<TKey>>
	struct unordered_unique_index : public std::unordered_map<TKey, std::size_t, _TCompare>
	{
		typename std::unordered_map<TKey, std::size_t, _TCompare>::iterator emplace(const TKey& Key_, std::size_t DataIndex_)
		{
			auto ib = std::unordered_map<TKey, std::size_t, _TCompare>::emplace(Key_, DataIndex_);
			if (!ib.second)
				throw 0;

			return ib.first;
		}
	};
	template<typename TKey, class _TCompare = less<TKey>>
	struct unordered_non_unique_index : public std::unordered_multimap<TKey, std::size_t, _TCompare>
	{
		typename std::unordered_multimap<TKey, std::size_t, _TCompare>::iterator emplace(const TKey& Key_, std::size_t DataIndex_)
		{
			return std::unordered_multimap<TKey, std::size_t, _TCompare>::emplace(Key_, DataIndex_);
		}
	};
}
