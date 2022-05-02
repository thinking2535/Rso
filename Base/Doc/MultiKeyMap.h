#pragma once

#include <Rso/Base/List.h>
#include <map>

namespace rso
{
	namespace base
	{
		using namespace std;

		template<typename _TKey, typename... _TKeys>
		class _CMultiKeyMap : public _CMultiKeyMap<_TKey>, public _CMultiKeyMap<_TKeys...>
		{
		public:
			using _CMultiKeyMap<_TKey>::emplace;
			using _CMultiKeyMap<_TKeys...>::emplace;
			using _CMultiKeyMap<_TKey>::erase;
			using _CMultiKeyMap<_TKeys...>::erase;
			using _CMultiKeyMap<_TKey>::find;
			using _CMultiKeyMap<_TKeys...>::find;
			using _CMultiKeyMap<_TKey>::end;
			using _CMultiKeyMap<_TKeys...>::end;
		};
		template<typename _TKey>
		class _CMultiKeyMap<_TKey>
		{
			using _TMap = map<_TKey, size_t>;
			using _TMapIt = typename _TMap::iterator;

			_TMap _Map;

		public:
			inline auto emplace(_TKey&& Key_, size_t Index_)
			{
				return _Map.emplace(Key_, Index_);
			}
			inline void erase(_TMapIt It_)
			{
				_Map.erase(It_);
			}
			inline auto find(const _TKey& Key_) const
			{
				return _Map.find(Key_);
			}
			inline auto end(void) const
			{
				return _Map.end();
			}
		};
		template<typename _TValue, typename... _TKeys>
		class CMultiKeyMap : private CList<pair<_TValue, tuple<typename map<_TKeys, size_t>::iterator...>>>
		{
		public:
			static const size_t key_size = sizeof...(_TKeys);
			using typename CList<pair<_TValue, tuple<typename map<_TKeys, size_t>::iterator...>>>::value_type;
			using iterator = typename CList<pair<_TValue, tuple<typename map<_TKeys, size_t>::iterator...>>>::iterator;

		private:
			using _TMapIts = typename value_type::second_type;

			_CMultiKeyMap<_TKeys...> _Maps;

			template<typename _TKey, typename... _TKeys>
			bool _emplace(iterator It_, _TKey&& Key_, _TKeys&&... Keys_)
			{
				auto ib = _Maps.emplace(std::move(Key_), It_.Index());
				if (!ib.second)
					return false;

				std::get<key_size - sizeof...(Keys_)-1>(It_->second) = ib.first;

				try
				{
					if (!_emplace(It_, Keys_...))
					{
						_Maps.erase(ib.first);
						return false;
					}
				}
				catch (...)
				{
					_Maps.erase(ib.first);
					return false;
				}

				return true;
			}
			template<typename _TKey>
			bool _emplace(iterator It_, _TKey&& Key_)
			{
				auto ib = _Maps.emplace(std::move(Key_), It_.Index());
				if (!ib.second)
					return false;

				std::get<key_size - 1>(It_->second) = ib.first;
				return true;
			}
			template<size_t MapNum_>
			void _erase_maps(const _TMapIts& MapIts_)
			{
				_Maps.erase(std::get<MapNum_>(MapIts_));
				_erase_maps<MapNum_ - 1>(MapIts_);
			}
			template<>
			void _erase_maps<0>(const _TMapIts& MapIts_)
			{
				_Maps.erase(std::get<0>(MapIts_));
			}

		public:
			using CList<value_type>::get;
			using CList<value_type>::begin;
			using CList<value_type>::end;
			using CList<value_type>::front;
			using CList<value_type>::back;
			using CList<value_type>::size;
			using CList<value_type>::operator[];

			template<typename _TValue, typename... _TKeys>
			auto emplace(_TValue&& Value_, _TKeys&&... Keys_)
			{
				auto it = __super::emplace(Value_, value_type::second_type());
				if (!it)
					return iterator();

				try
				{
					if (!_emplace(it, Keys_...))
					{
						__super::erase(it);
						return iterator();
					}
				}
				catch (...)
				{
					__super::erase(it);
					return iterator();
				}

				return it;
			}
			template<typename _TValue, typename... _TKeys>
			auto emplace_at(size_t Index_, _TValue&& Value_, _TKeys&&... Keys_)
			{
				auto it = __super::emplace_at(Index_, Value_, value_type::second_type());
				if (!it)
					return iterator();

				try
				{
					if (!_emplace(it, Keys_...))
					{
						__super::erase(it);
						return iterator();
					}
				}
				catch (...)
				{
					__super::erase(it);
					return iterator();
				}

				return it;
			}
			template<typename _TKey>
			void erase_with_key(const _TKey& Key_)
			{
				auto it = _Maps.find(Key_);
				if (it == _Maps._CMultiKeyMap<_TKey>::end())
					return;

				erase(it->second);
			}
			void erase(iterator It_)
			{
				_erase_maps<key_size - 1>(It_->second);
				__super::erase(It_);
			}
			void erase(size_t Index_)
			{
				auto it = __super::get(Index_);
				if (!it)
					return;

				erase(it);
			}
			template<typename _TKey>
			auto find(const _TKey& Key_)
			{
				auto it = _Maps.find(Key_);
				if (it == _Maps._CMultiKeyMap<_TKey>::end())
					return iterator();

				return __super::get(it->second);
			}
			template<typename _TKey>
			auto find(const _TKey& Key_) const
			{
				auto it = _Maps.find(Key_);
				if (it == _Maps._CMultiKeyMap<_TKey>::end())
					return const_iterator();

				return __super::get(it->second);
			}
		};
	}
}
