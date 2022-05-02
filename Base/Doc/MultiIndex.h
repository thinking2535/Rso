#pragma once

#include <tuple>
#include <map>
#include <optional>
#include <Rso/Base/List.h>

namespace rso
{
	namespace base
	{
		using namespace std;

		// need to remove ///////////////////
		template<typename TKey>
		struct multimap_index : public std::multimap<TKey, std::size_t>
		{
			typename std::multimap<TKey, std::size_t>::iterator emplace(const TKey& Key_, std::size_t DataIndex_)
			{
				return std::multimap<TKey, std::size_t>::emplace(Key_, DataIndex_);
			}
		};
		template<typename TKey>
		struct map_index : std::map<TKey, std::size_t>
		{
			typename std::map<TKey, std::size_t>::iterator emplace(const TKey& Key_, std::size_t DataIndex_)
			{
				auto ib = std::map<TKey, std::size_t>::emplace(Key_, DataIndex_);
				if (!ib.second)
					throw 0;

				return ib.first;
			}
		};
		//////////////////////////////////////

		template<typename TValue, typename... TMaps> // => template<typename pair<key, bool is_unique>...>
		class CMultiIndex
		{
			using _TMaps = tuple<TMaps...>;
			using _TIterators = tuple<typename TMaps::iterator...>;
			using _TDatas = CList<pair<_TIterators, TValue>>;

		public:
			using iterator = typename _TDatas::iterator;
			using const_iterator = typename _TDatas::const_iterator;

		private:
			_TMaps _Maps;
			_TDatas _Datas;

		public:
			CMultiIndex()
			{
			}
			CMultiIndex(const CMultiIndex& Var_) = delete;
			CMultiIndex& operator = (const CMultiIndex& Var_) = delete;
			inline TValue& operator[](std::size_t DataIndex_) { return _Datas[DataIndex_].second; }
			inline const TValue& operator[](std::size_t DataIndex_) const { return _Datas[DataIndex_].second; }
			inline const_iterator get(std::size_t DataIndex_) const { _Datas.get(DataIndex_); }
			inline iterator get(std::size_t DataIndex_) { _Datas.get(DataIndex_); }
			inline const_iterator begin(void) const { return _Datas.begin(); }
			inline const_iterator end(void) const { return _Datas.end(); }
			inline const_iterator last(void) const { return _Datas.last(); }
			inline iterator begin(void) { return _Datas.begin(); }
			inline iterator end(void) { return _Datas.end(); }
			inline iterator last(void) { return _Datas.last(); }
			inline const TValue& front(void) const { return _Datas.front().second; }
			inline TValue& front(void) { return _Datas.front().second; }
			inline const TValue& back(void) const { return _Datas.back().second; }
			inline TValue& back(void) { return _Datas.back().second; }
			inline std::size_t capacity(void) const { return _Datas.capacity(); }
			inline std::size_t size(void) const { return _Datas.size(); }
			inline bool empty(void) const { return _Datas.empty(); }
			inline std::size_t new_index(void) const { return _Datas.new_index(); }

		private:
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
			template<std::size_t... Indices_>
			void _erase_tuple(index_sequence<Indices_...>, const _TIterators& Iterators_)
			{
				(_erase(std::get<Indices_>(Iterators_), std::get<Indices_>(_Maps)), ...);
			}
			template<std::size_t... Indices_>
			void _clear(index_sequence<Indices_...>)
			{
				(std::get<Indices_>(_Maps).clear(), ...);
			}
		public:
			template<std::size_t Index_, typename TKey, typename... TKeys>
			void _emplace(std::size_t DataIndex_, _TIterators& Iterators_, const TKey& Key_, const TKeys&... Keys_)
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
			optional<iterator> emplace_at(std::size_t DataIndex_, const typename TMaps::key_type&... Keys_, const TValue& Value_)
			{
				try
				{
					_TIterators Iterators;
					_emplace<0>(DataIndex_, Iterators, Keys_...);
					return _Datas.emplace_at(DataIndex_, pair<_TIterators, TValue>(Iterators, Value_));
				}
				catch (...)
				{
					return {};
				}
			}
			optional<iterator> emplace(const typename TMaps::key_type&... Keys_, const TValue& Value_)
			{
				return emplace_at(_Datas.new_index(), Keys_..., Value_);
			}
			void erase(iterator it_)
			{
				_erase_tuple(index_sequence_for<typename TMaps::key_type...>{}, it_->first);
				_Datas.erase(it_);
			}
			size_t erase_at(std::size_t DataIndex_)
			{
				auto it = _Datas.get(DataIndex_);
				if (!it)
					return 0;

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
					erase(_Datas.get(itCheck->second));
					++ret;
				}

				return ret;
			}
			void pop_front(void)
			{
				auto it = _Datas.begin();
				if (!it)
					return;

				erase(it);
			}
			void pop_back(void)
			{
				auto it = _Datas.last();
				if (!it)
					return;

				erase(it);
			}
			template<std::size_t Index_, typename TKey>
			const_iterator find(const TKey& Key_) const
			{
				auto it = std::get<Index_>(_Maps).find(Key_);
				if (it == std::get<Index_>(_Maps).end())
					return const_iterator();

				return _Datas.get(it->second);
			}
			template<std::size_t Index_, typename TKey>
			iterator find(const TKey& Key_)
			{
				auto it = std::get<Index_>(_Maps).find(Key_);
				if (it == std::get<Index_>(_Maps).end())
					return iterator();

				return _Datas.get(it->second);
			}
			template<std::size_t Index_>
			void clear(void)
			{
				_clear(index_sequence_for<typename TMaps::key_type...>{});
				_Datas.clear();
			}
		};
	}
}
