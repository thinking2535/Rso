#pragma once

#include "MultiIndex.h"

namespace rso::base
{
	template<typename... TMaps>
	struct set_value_type
	{
		using const_iterators = tuple<typename TMaps::const_iterator...>;
		using data_type = const_iterators;
	};

	template<typename... TMaps>
	class CMultiIndexSet : public CList<typename set_value_type<TMaps...>::data_type>
	{
	public:
		using maps = typename tuple<TMaps...>;
		using iterator = typename CList<typename set_value_type<TMaps...>::data_type>::iterator;
		using const_iterator = typename CList<typename set_value_type<TMaps...>::data_type>::const_iterator;
		using const_iterators = typename set_value_type<TMaps...>::const_iterators;
		using CList<typename set_value_type<TMaps...>::data_type>::new_index;
		using CList<typename set_value_type<TMaps...>::data_type>::reserve;
		using CList<typename set_value_type<TMaps...>::data_type>::get;
		using CList<typename set_value_type<TMaps...>::data_type>::begin;
		using CList<typename set_value_type<TMaps...>::data_type>::end;
		using CList<typename set_value_type<TMaps...>::data_type>::last;
		using CList<typename set_value_type<TMaps...>::data_type>::front;
		using CList<typename set_value_type<TMaps...>::data_type>::back;
		template<typename... TArgsEmplace>
		void resize(size_t Size_, TArgsEmplace&&... Args_) = delete;

	protected:
		maps _Maps;

		template<std::size_t... Indices_>
		void _clear(index_sequence<Indices_...>)
		{
			(std::get<Indices_>(_Maps).clear(), ...);
		}
		template<std::size_t Index_, typename TKey, typename... TKeys>
		void _emplace(std::size_t DataIndex_, const_iterators& Iterators_, const TKey& Key_, const TKeys&... Keys_)
		{
			std::get<Index_>(Iterators_) = std::get<Index_>(_Maps).emplace(Key_, DataIndex_);

			try
			{
				if constexpr (sizeof...(TKeys) > 0)
					_emplace<Index_ + 1>(DataIndex_, Iterators_, Keys_...);
			}
			catch (...)
			{
				std::get<Index_>(_Maps).erase(std::get<Index_>(Iterators_));
				throw;
			}
		}
		template<std::size_t... Indices_>
		void _erase_tuple(index_sequence<Indices_...>, const const_iterators& Iterators_)
		{
			(_erase(std::get<Indices_>(Iterators_), std::get<Indices_>(_Maps)), ...);
		}
		template<std::size_t... Indices_>
		void _erase(index_sequence<Indices_...>, const typename TMaps::key_type&... Keys_)
		{
			(std::get<Indices_>(_Maps).erase(Keys_), ...);
		}
		template<typename TIterator, typename TMap>
		void _erase(TIterator Iterator_, TMap&& Map_)
		{
			Map_.erase(Iterator_);
		}

	public:
		template<std::size_t Index_>
		inline auto& operator()() { return std::get<Index_>(_Maps); }
		template<std::size_t Index_>
		inline auto begin(void) const { return std::get<Index_>(_Maps).begin(); }
		template<std::size_t Index_>
		inline auto end(void) const { return std::get<Index_>(_Maps).end(); }
		template<std::size_t Index_>
		inline auto begin(void) { return std::get<Index_>(_Maps).begin(); }
		template<std::size_t Index_>
		inline auto end(void) { return std::get<Index_>(_Maps).end(); }
		template<std::size_t Index_>
		inline auto rbegin(void) const { return std::get<Index_>(_Maps).rbegin(); }
		template<std::size_t Index_>
		inline auto rend(void) const { return std::get<Index_>(_Maps).rend(); }
		template<std::size_t Index_>
		inline auto rbegin(void) { return std::get<Index_>(_Maps).rbegin(); }
		template<std::size_t Index_>
		inline auto rend(void) { return std::get<Index_>(_Maps).rend(); }
		template<std::size_t Index_, typename TKey>
		auto find(const TKey& Key_) const { return std::get<Index_>(_Maps).find(Key_); }
		template<std::size_t Index_, typename TKey>
		auto find(const TKey& Key_) { return std::get<Index_>(_Maps).find(Key_); }
		iterator emplace_at(std::size_t DataIndex_, const typename TMaps::key_type&... Keys_)
		{
			const_iterators Iterators;
			_emplace<0>(DataIndex_, Iterators, Keys_...);
			return CList<typename set_value_type<TMaps...>::data_type>::emplace_at(DataIndex_, Iterators);
		}
		iterator emplace(const typename TMaps::key_type&... Keys_)
		{
			return emplace_at(new_index(), Keys_...);
		}
		bool erase(iterator it_)
		{
			_erase_tuple(index_sequence_for<typename TMaps::key_type...>{}, * it_);
			return CList<typename set_value_type<TMaps...>::data_type>::erase(it_);
		}
		bool erase(std::size_t Index_)
		{
			auto it = get(Index_);
			if (!it)
				return false;

			return erase(it);
		}
		template<std::size_t Index_, typename TKey>
		size_t erase(const TKey& Key_)
		{
			size_t ret = 0;
			auto range = std::get<Index_>(_Maps).equal_range(Key_);
			for (auto it = range.first; it != range.second;)
			{
				auto itCheck = it++;
				erase(get(itCheck->second));
				++ret;
			}

			return ret;
		}
		template<std::size_t Index_>
		bool erase(typename tuple_element<Index_, maps>::type::iterator it_)
		{
			return erase(get(it_->second));
		}
		template<std::size_t Index_>
		bool erase(typename tuple_element<Index_, maps>::type::const_iterator it_)
		{
			return erase(get(it_->second));
		}
		void pop_front(void)
		{
			auto it = begin();
			if (!it)
				return;

			erase(it);
		}
		void pop_back(void)
		{
			auto it = last();
			if (!it)
				return;

			erase(it);
		}
		void clear(void)
		{
			_clear(index_sequence_for<typename TMaps::key_type...>{});
			CList<typename set_value_type<TMaps...>::data_type>::clear();
		}
	};
}
