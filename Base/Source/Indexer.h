#pragma once


#include <map>


namespace rso
{
    namespace base
    {
        using namespace std;

        template<typename _TKeyType, typename _TValueType>
        class CIndexer
        {
        private:
            using _TIndice = map<_TKeyType, _TValueType>;

            _TIndice _Indice;

        public:
            inline typename _TIndice::iterator begin(void) { return _Indice.begin(); }
            inline typename _TIndice::iterator end(void) { return _Indice.end(); }
            inline typename _TIndice::const_iterator begin(void) const { return _Indice.begin(); }
            inline typename _TIndice::const_iterator end(void) const { return _Indice.end(); }
            bool emplace(const _TKeyType& Key_, const _TValueType& Value_)
            {
                try
                {
                    return _Indice.emplace(Key_, Value_).second;
                }
                catch (...)
                {
                    return false;
                }
            }
            typename _TIndice::const_iterator find(const _TKeyType& Key_) const
            {
                auto it = _Indice.upper_bound(Key_);
                if ((it == _Indice.end() && _Indice.empty()) ||
                    it == _Indice.begin())
                    return _Indice.end();

                return --it;
            }
        };
    }
}