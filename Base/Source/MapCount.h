#pragma once


#include "Base.h"


namespace rso
{
    namespace base
    {
        template<typename TKey, typename TValue>
        class CMapCount
        {
            using _TDatas = map<TKey, TValue>;
        public:
            using value_type = typename _TDatas::value_type;
        private:
            map<TKey, TValue> _Datas;

        public:
            CMapCount operator += (const value_type& rhs_)
            {
                auto ib = _Datas.emplace(rhs_);
                if (!ib.second)
                {
                    if (ib.first->second + rhs_.second != 0)
                        ib.first->second += rhs_.second;
                    else
                        _Datas.erase(ib.first);
                }

                return *this;
            }
            CMapCount operator -= (const value_type& rhs_)
            {
                auto ib = _Datas.emplace(rhs_);
                if (!ib.second)
                {
                    if (ib.first->second - rhs_.second != 0)
                        ib.first->second -= rhs_.second;
                    else
                        _Datas.erase(ib.first);
                }

                return *this;
            }
            CMapCount operator += (const CMapCount& rhs_)
            {
                for (auto& i : rhs_._Datas)
                    operator += (i);

                return *this;
            }
            void operator -= (const CMapCount& rhs_)
            {
                for (auto& i : rhs_._Datas)
                    operator -= (i);

                return *this;
            }
            inline bool empty(void) const
            {
                return _CodeCnts.empty();
            }
        };
    }
}