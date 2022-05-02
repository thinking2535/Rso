#pragma once


#include <Rso/Core/Core.h>
#include <vector>
#include <queue>


namespace rso
{
    namespace base
    {
        using namespace std;

        template< typename _T >
        class CIndex
        {
        public:
            static const _T c_Null = (_T)(~0);

        private:
            typedef vector< bool > _TEmptyTable;
            typedef queue< _T > _TEmptyIDXs;

            _TEmptyTable m_EmptyTable;
            _TEmptyIDXs m_EmptyIDXs;
            _T m_MaxCnt;

        public:
            CIndex() : m_MaxCnt(0) {}
            ~CIndex()
            {
                m_EmptyTable.clear();
            }

            _T New(void);
            void Del(const _T Index_);
        };

        template< typename _T >
        _T CIndex<_T>::New(void)
        {
            if (m_MaxCnt + 1 == c_Null) return c_Null;

            if (m_EmptyIDXs.size() == 0)
            {
                m_EmptyTable.push_back(true);
                return m_MaxCnt++;
            }

            _T Index = m_EmptyIDXs.front();

            m_EmptyIDXs.pop();
            m_EmptyTable[Index] = true;

            return Index;
        }

        template< typename _T >
        void CIndex<_T>::Del(const _T Index_)
        {
            if (Index_ >= m_MaxCnt) return;
            if (Index_ < 0) return;

            if (m_EmptyTable[Index_] == true)
            {
                try
                {
                    m_EmptyIDXs.push(Index_);
                    m_EmptyTable[Index_] = false;
                }
                catch (...)
                {
                }
            }
        }
    }
}
