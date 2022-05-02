#pragma once

#include "Base.h"
#include <Rso/Base/MultiIndexSet.h>
#include <functional>

namespace rso::gameutil
{
	using namespace base;
	using namespace std;

	template<typename TKey>
	class CTimer : public CMultiIndexSet<unique_index<TKey>, non_unique_index<TTime>>
	{
	public:
		using TCallback = function<void(const TKey& Key_)>;
		using const_iterator = typename CMultiIndexSet<unique_index<TKey>, non_unique_index<TTime>>::const_iterator;
		using iterator = typename CMultiIndexSet<unique_index<TKey>, non_unique_index<TTime>>::iterator;
	private:
		TCallback _Callback;

	public:
		CTimer(TCallback Callback_) : _Callback(Callback_) {}

		template<std::size_t Index_, typename _TKey>
		const_iterator find(const _TKey& Key_) const = delete;
		template<std::size_t Index_, typename _TKey>
		iterator find(const _TKey& Key_) = delete;
		template<std::size_t Index_>
		inline const_iterator find(const TKey& Key_) const
		{
			auto it = std::get<0>(_Maps).find(Key_);
			if (it == std::get<0>(_Maps).end())
				return const_iterator();

			return _Datas.get(it->second);
		}
		inline iterator find(const TKey& Key_)
		{
			auto it = std::get<0>(_Maps).find(Key_);
			if (it == std::get<0>(_Maps).end())
				return iterator();

			return _Datas.get(it->second);
		}

		template<typename _TDuration>
		void move(iterator it_, const _TDuration& Duration_) // throw
		{
			auto itNew = std::get<1>(_Maps).emplace(std::get<1>(*it_)->first + Duration_, it_.Index());
			std::get<1>(_Maps).erase(std::get<1>(*it_));
			std::get<1>(*it_) = itNew;
		}
		template<typename _TDuration>
		void move(const TKey& Key_, const _TDuration& Duration_) // throw
		{
			auto it = std::get<0>(_Maps).find(Key_);
			if (it == std::get<0>(_Maps).end())
				return;

			move(_Datas.get(it->second), Duration_);
		}
		void move(iterator it_, const TTime& Time_) // throw
		{
			auto itNew = std::get<1>(_Maps).emplace(Time_, it_.Index());
			std::get<1>(_Maps).erase(std::get<1>(*it_));
			std::get<1>(*it_) = itNew;
		}
		inline void move(const TKey& Key_, const TTime& Time_) // throw
		{
			auto it = std::get<0>(_Maps).find(Key_);
			if (it == std::get<0>(_Maps).end())
				return;

			move(_Datas.get(it->second), Time_);
		}
		void Proc(void)
		{
			auto Now = system_clock::now();
			for (auto it = std::get<1>(_Maps).begin(); it != std::get<1>(_Maps).end(); )
			{
				auto itCheck = it;
				++it;

				if (itCheck->first > Now)
					break;

				auto itData = _Datas.get(itCheck->second);
				_Callback(std::get<0>(*itData)->first);
				erase(itData);
			}
		}
	};
}