#pragma once

#include <Rso/Base/List.h>

namespace rso
{
    namespace base
    {
        using namespace std;

        template<typename TData>
        class CCycleCList : public CList<TData>
        {
		public:
			using typename CList<TData>::iterator;
			using typename CList<TData>::const_iterator;
			using typename CList<TData>::value_type;
			using CList<TData>::begin;
			using CList<TData>::end;
			using CList<TData>::last;
			using CList<TData>::get;

		private:
			iterator _cycle_it = end();

        public:
			void pop_front(void)
			{
				if (_cycle_it != end() && begin() == _cycle_it)
					++_cycle_it;

				CList<TData>::pop_front();
			}
			void pop_back(void)
			{
				if (_cycle_it != end() && last() == _cycle_it)
					++_cycle_it;

				CList<TData>::pop_back();
			}
			bool erase(const_iterator _Where)
            {
                if (_cycle_it != end() && _Where == _cycle_it)
                    ++_cycle_it;

                return CList<TData>::erase(_Where);
            }
			bool erase(size_t Index_)
			{
				auto it = get(Index_);
				if (it == end())
					return false;

				if (_cycle_it != end() && it == _cycle_it)
					++_cycle_it;

				return CList<TData>::erase(it);
			}
            void clear()
            {
                _cycle_it = end();

				CList<TData>::clear();
            }
            iterator& cycle(void)
            {
				if (_cycle_it == end())
					return (_cycle_it = CList<TData>::begin());
				else
					return ++_cycle_it;
			}
		};
    }
}