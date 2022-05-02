#pragma once

#include <Rso/Core/Core.h>
#include <map>

namespace rso
{
    namespace base
    {
        using namespace std;

        template<typename TKey, typename TValue>
        class CCycleMap : public map<TKey, TValue>
        {
		public:
			using typename map<TKey, TValue>::iterator;
			using typename map<TKey, TValue>::const_iterator;
			using typename map<TKey, TValue>::size_type;
			using typename map<TKey, TValue>::key_type;
			using typename map<TKey, TValue>::value_type;
			using map<TKey, TValue>::begin;
			using map<TKey, TValue>::end;

		private:
            iterator _cycle_it = end();

		public:
            iterator erase(const_iterator _Where)
            {
                if (_cycle_it != end() && _Where == _cycle_it)
                    ++_cycle_it;

                return map<TKey, TValue>::erase(_Where);
            }
            iterator erase(const_iterator _First, const_iterator _Last)
            {
                for (auto it = _First; it != _Last; ++it)
                {
                    if (it == _cycle_it)
                    {
                        _cycle_it = _Last;
                        break;
                    }
                }

                return map<TKey, TValue>::erase(_First, _Last);
            }
            size_type erase(const key_type& _Keyval)
            {
                if (_cycle_it != end() && _Keyval == _cycle_it->first)
                    ++_cycle_it;

                return map<TKey, TValue>::erase(_Keyval);
            }
            void clear()
            {
                _cycle_it = end();

                map<TKey, TValue>::clear();
            }
            iterator& cycle(void)
            {
				if (_cycle_it == end())
					return (_cycle_it = map<TKey, TValue>::begin());
				else
					return ++_cycle_it;
			}
		};
    }
}