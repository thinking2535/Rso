#pragma once


#include "Base.h"
#include <new>
#include <vector>


namespace rso
{
    namespace base
    {
        using namespace std;

        template< typename _TData, typename _TSize >
        class CMemory
        {
        public:
            typedef _TData TData;
            typedef _TSize TSize;

        private:
            typedef vector< _TData* > TDatas;

        public:
            static const _TSize c_IDX_Null;

            struct SLink
            {
                _TSize ObjectIDX;
                _TSize Prev;
                _TSize Next;

                SLink()
                {
                    ObjectIDX = c_IDX_Null;
                    Prev = c_IDX_Null;
                    Next = c_IDX_Null;
                }
            };

        private:
            typedef vector< SLink > TLinks;

            TDatas m_Datas;
            TLinks m_Links;

            _TSize m_ObjectCnt;

            _TSize m_EmptyListHead;
            _TSize m_EmptyListTail;
            _TSize m_FilledListHead;
            _TSize m_FilledListTail;
            _TSize m_MaxSize;

            _TSize GetAvailIDX(_TSize Index_);
            const SLink* GetLink(_TSize Index_) const;
            inline void _New(_TSize Index_);
            inline void Detach(_TSize Index_);
            inline void AttachFilled(_TSize Index_);
            inline void AttachEmpty(_TSize Index_);
            inline bool _Reserve(_TSize Size_);

        public:
            CMemory();
            virtual ~CMemory();

			inline _TData* GetRawData(_TSize ObjectIDX_) { return m_Datas.at(ObjectIDX_); }
            _TData* GetObj(_TSize ObjectIDX_) const;
			inline _TSize GetObjCnt(void) const { return m_ObjectCnt; }

            _TData*  New(_TSize* pIndex_o = 0);
            _TData* NewIndex(_TSize Index_); // Index_ 의 크기에 비례해서 메모리를 잡게 되므로 주의

            void  Del(_TSize Index_);
            void Clear(void);
            bool Reserve(_TSize Size_);
			inline typename TDatas::iterator Begin(void) { return m_Datas.begin(); }
			inline const SLink* Head(void) const { return GetLink(m_FilledListHead); }
			inline const SLink* Tail(void) const { return GetLink(m_FilledListTail); }
			inline const SLink* Next(const SLink* pLink_) const { return GetLink(pLink_->Next); }
			inline const SLink* Prev(const SLink* pLink_) const { return GetLink(pLink_->Prev); }
			inline void SetMaxSize(const _TSize Size_) { m_MaxSize = Size_; }
			inline _TSize MaxSize(void) const { return m_MaxSize; }
        };
        template< typename _TData, typename _TSize >
        const _TSize CMemory<_TData, _TSize>::c_IDX_Null = numeric_limits<_TSize>::max();


        /************************************************************************/
        /* PRIVATE FUNCTIONS                                                    */
        /************************************************************************/
        template< typename _TData, typename _TSize >
        _TSize CMemory<_TData, _TSize>::GetAvailIDX(_TSize Index_)
        {
            if (Index_ != c_IDX_Null  &&  Index_ < 0) return c_IDX_Null;
            if (Index_ >= 0 && Index_ < (_TSize)m_Datas.size())
            {
                if (m_Links[Index_].ObjectIDX == c_IDX_Null)
                {
                    return Index_;
                }

                return c_IDX_Null;
            }

            // 인덱스를 지정하지 않은 경우
            if (Index_ == c_IDX_Null)
            {
                if (m_EmptyListHead == c_IDX_Null)
                {
                    if ((_TSize)m_Datas.size() >= m_MaxSize) return c_IDX_Null;

                    _TData* pData = 0;
                    try
                    {
                        pData = (_TData*) operator new (sizeof(_TData));
                    }
                    catch (bad_alloc)
                    {
                        return c_IDX_Null;
                    }
                    if (pData == 0)
                        return c_IDX_Null;

                    try
                    {
                        m_Datas.push_back(pData);
                    }
                    catch (...)
                    {
                        operator delete (pData);
                        return c_IDX_Null;
                    }

                    try
                    {
                        m_Links.push_back(SLink());
                    }
                    catch (...)
                    {
                        operator delete (pData);
                        m_Datas.pop_back();
                        return c_IDX_Null;
                    }

                    m_EmptyListHead = m_EmptyListTail = (_TSize)m_Datas.size() - 1;
                }

                return m_EmptyListHead;
            }


            // 박밖의 Index를 지정한 경우 /////////////
            if (_Reserve(Index_ + 1) == false)
                return c_IDX_Null;

            return Index_;
        }

        template< typename _TData, typename _TSize >
        typename const CMemory<_TData, _TSize>::SLink* CMemory<_TData, _TSize>::GetLink(_TSize Index_) const
        {
            if (Index_ >= (_TSize)m_Datas.size() || Index_ < 0)
				return nullptr;

            return &m_Links[Index_];
        }

        template< typename _TData, typename _TSize >
        void CMemory<_TData, _TSize>::_New(_TSize Index_)
        {
            // rso  which is better? iterator 나 기타 메모리 관리성 클래스에서 오동작  raw_storage_iterator 로 써야 함.?

            // Call Constructor
            new(m_Datas[Index_]) _TData;

            Detach(Index_);

            AttachFilled(Index_);

            ++m_ObjectCnt;
        }

        template< typename _TData, typename _TSize >
        void CMemory<_TData, _TSize>::Detach(_TSize Index_)
        {
            if (m_Links[Index_].Prev != c_IDX_Null)
            {
                m_Links[m_Links[Index_].Prev].Next = m_Links[Index_].Next;
            }

            if (m_Links[Index_].Next != c_IDX_Null)
            {
                m_Links[m_Links[Index_].Next].Prev = m_Links[Index_].Prev;
            }


            if (m_Links[Index_].ObjectIDX == c_IDX_Null) // Empty
            {
                if (m_EmptyListHead == Index_)
                    m_EmptyListHead = m_Links[Index_].Next;

                if (m_EmptyListTail == Index_)
                    m_EmptyListTail = m_Links[Index_].Prev;

                m_Links[Index_].ObjectIDX = Index_;
            }
            else // Filled
            {
                if (m_FilledListHead == Index_)
                    m_FilledListHead = m_Links[Index_].Next;

                if (m_FilledListTail == Index_)
                    m_FilledListTail = m_Links[Index_].Prev;

                m_Links[Index_].ObjectIDX = c_IDX_Null;
            }

            if (m_Links[Index_].Prev != c_IDX_Null) m_Links[Index_].Prev = c_IDX_Null;
            if (m_Links[Index_].Next != c_IDX_Null) m_Links[Index_].Next = c_IDX_Null;
        }

        template< typename _TData, typename _TSize >
        void CMemory<_TData, _TSize>::AttachFilled(_TSize Index_)
        {
            if (m_FilledListTail == c_IDX_Null)
            {
                m_FilledListHead = m_FilledListTail = Index_;
            }
            else
            {
                m_Links[m_FilledListTail].Next = Index_;
                m_Links[Index_].Prev = m_FilledListTail;
                m_FilledListTail = Index_;
            }
        }

        template< typename _TData, typename _TSize >
        void CMemory<_TData, _TSize>::AttachEmpty(_TSize Index_)
        {
            if (m_EmptyListHead == c_IDX_Null)
            {
                m_EmptyListHead = m_EmptyListTail = Index_;
            }
            else
            {
                m_Links[m_EmptyListHead].Prev = Index_;
                m_Links[Index_].Next = m_EmptyListHead;
                m_EmptyListHead = Index_;
            }
        }

        template< typename _TData, typename _TSize >
        bool CMemory<_TData, _TSize>::_Reserve(_TSize Size_)
        {
            if (Size_ > m_MaxSize) return false;

            _TSize OldSize = (_TSize)m_Datas.size();

            try
            {
                m_Datas.resize(Size_);
            }
            catch (...)
            {
                return false;
            }

            try
            {
                m_Links.resize(Size_);
            }
            catch (...)
            {
                m_Datas.resize(OldSize);
                return false;
            }


            bool bNewError = false;
            _TSize Cnt = OldSize;
            for (; Cnt < (_TSize)m_Datas.size(); ++Cnt)
            {
                try
                {
                    m_Datas[Cnt] = (_TData*) operator new(sizeof(_TData));
                }
                catch (bad_alloc)
                {
                    bNewError = true;
                    break;
                }
                if (m_Datas[Cnt] == 0)
                {
                    bNewError = true;
                }
            }

            if (bNewError)
            {
                // delete
                for (_TSize j = OldSize; j < Cnt; ++j)
                    operator delete (m_Datas[j]);

                m_Datas.resize(OldSize);
                m_Links.resize(OldSize);

                m_Datas.shrink_to_fit();
                m_Links.shrink_to_fit();

                return false;
            }


            m_Links[OldSize].Prev = m_EmptyListTail;

            for (_TSize i = OldSize; i < (_TSize)m_Links.size() - 1; ++i)
            {
                m_Links[i].Next = i + 1;
                m_Links[i + 1].Prev = i;
            }

            if (m_EmptyListHead == c_IDX_Null)
            {
                m_EmptyListHead = OldSize;
            }
            m_EmptyListTail = (_TSize)(m_Datas.size() - 1);

            return true;
        }


        /************************************************************************/
        /* PUBLIC FUNCTIONS                                                     */
        /************************************************************************/
        template< typename _TData, typename _TSize >
        CMemory<_TData, _TSize>::CMemory() :
            m_ObjectCnt(0),
            m_EmptyListHead(c_IDX_Null),
            m_EmptyListTail(c_IDX_Null),
            m_FilledListHead(c_IDX_Null),
            m_FilledListTail(c_IDX_Null),
            m_MaxSize(numeric_limits<_TSize>::max())
        {
        }

        template< typename _TData, typename _TSize >
        CMemory<_TData, _TSize>::~CMemory()
        {
            Clear();

            for (TDatas::iterator it = m_Datas.begin();
                it != m_Datas.end();
                ++it)
            {
                if ((*it) != 0)
                {
                    operator delete (*it);
                    (*it) = 0;
                }
            }

            m_Datas.clear();
            m_Links.clear();

            m_FilledListTail = c_IDX_Null;
            m_FilledListHead = c_IDX_Null;
            m_EmptyListTail = c_IDX_Null;
            m_EmptyListHead = c_IDX_Null;
            m_ObjectCnt = 0;
        }

        template< typename _TData, typename _TSize >
        _TData* CMemory<_TData, _TSize>::GetObj(_TSize ObjectIDX_) const
        {
            if (ObjectIDX_ >= (_TSize)m_Datas.size() || ObjectIDX_ < 0)
                return nullptr;

            if (m_Links[ObjectIDX_].ObjectIDX == c_IDX_Null)
                return nullptr;

            return m_Datas[ObjectIDX_];
        }

        template< typename _TData, typename _TSize >
        _TData* CMemory<_TData, _TSize>::New(_TSize* pIndex_o)
        {
            _TSize CurIndex = GetAvailIDX(c_IDX_Null);
            if (CurIndex == c_IDX_Null)
				return nullptr;

            _New(CurIndex);

            if (pIndex_o != 0)
                *pIndex_o = CurIndex;

            return m_Datas[CurIndex];
        }

        template< typename _TData, typename _TSize >
        _TData* CMemory<_TData, _TSize>::NewIndex(_TSize Index_)
        {
            if (Index_ == c_IDX_Null)
				return nullptr;

            _TSize CurIndex = GetAvailIDX(Index_);
            if (CurIndex == c_IDX_Null)
				return nullptr;

            _New(CurIndex);

            return m_Datas[CurIndex];
        }

        template< typename _TData, typename _TSize >
        void CMemory<_TData, _TSize>::Del(_TSize Index_)
        {
            if (Index_ >= (_TSize)m_Datas.size() ||
                Index_ < 0 ||
                m_Links[Index_].ObjectIDX == c_IDX_Null)
                return;

            // Call Destructor
            m_Datas[Index_]->~_TData();

            Detach(Index_);

            AttachEmpty(Index_);

            --m_ObjectCnt;

            return;
        }

        template< typename _TData, typename _TSize >
        void CMemory<_TData, _TSize>::Clear(void)
        {
            // Call Destructor
            for (const SLink* pLink = Tail();
                pLink != 0;
                pLink = Prev(pLink))
            {
                m_Datas[pLink->ObjectIDX]->~_TData();
            }

            m_FilledListHead = c_IDX_Null;
            m_FilledListTail = c_IDX_Null;
            m_ObjectCnt = 0;

            if (m_Links.size() > 0)
            {
                m_Links[0].Prev = c_IDX_Null;
                m_Links[m_Links.size() - 1].Next = c_IDX_Null;
                m_Links[m_Links.size() - 1].ObjectIDX = c_IDX_Null;

                for (_TSize i = 0; i < (_TSize)m_Links.size() - 1; ++i)
                {
                    m_Links[i].Next = i + 1;
                    m_Links[i + 1].Prev = i;
                    m_Links[i].ObjectIDX = c_IDX_Null;
                }


                m_EmptyListHead = 0;
                m_EmptyListTail = (_TSize)(m_Datas.size() - 1);
            }
            else
            {
                m_EmptyListHead = c_IDX_Null;
                m_EmptyListTail = c_IDX_Null;
            }
        }

        template< typename _TData, typename _TSize >
        bool CMemory<_TData, _TSize>::Reserve(_TSize Size_)
        {
            if (Size_ <= (_TSize)m_Datas.size())
                return true;

            return _Reserve(Size_);
        }
    }
}
