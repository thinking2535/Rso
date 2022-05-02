#pragma once


#include <Rso/Core/Core.h>
#include <Rso/Base/List.h>


namespace rso
{
    namespace base
    {
        template<typename TData>
        class CSpace final
        {
            using _TDatas = CList<TData>;
            using _TFreeDatas = CList<TData*>;
            static const size_t c_Index_Null = (~0);

            _TDatas _Datas;
            _TFreeDatas _FreeDatas;

        public:
            using iterator = typename _TDatas::iterator;
            using const_iterator = typename _TDatas::const_iterator;

			inline const_iterator get(size_t Index_) const
            {
                return _Datas.get(Index_);
            }
			inline iterator get(size_t Index_)
            {
                return _Datas.get(Index_);
            }
            template<typename... TArgs>
            iterator get_free(TArgs&&... Args_)
            {
                auto itFree = _FreeDatas.begin();
                if (itFree)
                    return _Datas.get(itFree.Index());

                auto it = _Datas.emplace(forward<TArgs>(Args_)...);
                if (!it)
                    return iterator();

                _FreeDatas.emplace_at(it.Index(), it.operator->());

                return it;
            }
			inline void set_full(iterator It_)
            {
                _FreeDatas.erase(It_.Index());
            }
            void unset_full(iterator It_)
            {
                auto it = _Datas.get(It_.Index());
                if (!it)
                    return;

                auto itFree = _FreeDatas.get(It_.Index());
                if (itFree)
                    return;

                _FreeDatas.emplace_at(It_.Index(), it.operator->()); // 과거에 메모리가 확보되었으므로 무조건 성공이라 간주
            }
            void erase(iterator It_)
            {
                _FreeDatas.erase(It_.Index());
                _Datas.erase(It_.Index());
            }
            void erase(size_t Index_)
            {
                _FreeDatas.erase(Index_);
                _Datas.erase(Index_);
            }
            void clear(void)
            {
                _FreeDatas.clear();
                _Datas.clear();
            }
			inline TData& operator[](size_t Index_)
            {
                return _Datas[Index_];
            }
			inline iterator begin(void) const
            {
                return _Datas.begin();
            }
			inline iterator end(void) const
            {
                return _Datas.end();
            }
			inline iterator begin(void)
            {
                return _Datas.begin();
            }
			inline iterator end(void)
            {
                return _Datas.end();
            }
        };
    }
}