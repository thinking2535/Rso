#pragma once

#include <Rso/Core/Core.h>
#include <set>

namespace rso
{
    namespace base
    {
        using namespace std;

        template<typename TKey>
        class CCycleSet : public set<TKey>
        {
		public:
			using typename set<TKey>::const_iterator;
			using typename set<TKey>::iterator;
			using typename set<TKey>::size_type;
			using typename set<TKey>::key_type;
			using set<TKey>::begin;
			using set<TKey>::end;

		private:
			const_iterator _cycle_it = end();

        public:
            iterator erase(const_iterator _Where)
            {
                if (_cycle_it != end() && _Where == _cycle_it)
                    ++_cycle_it;

                return set<TKey>::erase(_Where);
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

                return set<TKey>::erase(_First, _Last);
            }
            size_type erase(const key_type& _Keyval)
            {
                if (_cycle_it != end() && _Keyval == *_cycle_it)
                    ++_cycle_it;

                return set<TKey>::erase(_Keyval);
            }
            void clear()
            {
                _cycle_it = end();

                set<TKey>::clear();
            }
            const_iterator& cycle(void)
            {
                if (_cycle_it == end())
                    return (_cycle_it = set<TKey>::begin());
                else
                    return ++_cycle_it;
            }
		};
    }
}