#pragma once


#include <Rso/Core/Core.h>
#include <new>
#include <vector>


namespace rso
{
    namespace base
    {
        using namespace core;
        using namespace std;

        class CListVoid
        {
        private:
            struct _SNode
            {
                size_t Index;
                _SNode* pNext;
                _SNode* pPrev;
                bool Newed; // for emplace_at
            };

            using _TNodes = vector<_SNode*>;

            _TNodes _Nodes;
            _SNode* _pDeletedHead = nullptr;
            _SNode* _pDeletedTail = nullptr;
            _SNode* _pNewedHead = nullptr;
            _SNode* _pNewedTail = nullptr;
            size_t _Size = 0;

            void _AttachToNewed(_SNode* pNode_)
            {
                pNode_->pNext = nullptr;
                pNode_->Newed = true;

                if (_pNewedTail == 0)
                {
                    pNode_->pPrev = nullptr;
                    _pNewedHead = _pNewedTail = pNode_;
                }
                else
                {
                    pNode_->pPrev = _pNewedTail;
                    _pNewedTail->pNext = pNode_;
                    _pNewedTail = pNode_;
                }
            }
            void _AttachToDeleted(_SNode* pNode_)
            {
                pNode_->pNext = nullptr;
                pNode_->Newed = false;

                if (_pDeletedTail == 0)
                {
                    pNode_->pPrev = nullptr;
                    _pDeletedHead = _pDeletedTail = pNode_;
                }
                else
                {
                    pNode_->pPrev = _pDeletedTail;
                    _pDeletedTail->pNext = pNode_;
                    _pDeletedTail = pNode_;
                }
            }
            void _Detach(_SNode* pNode_)
            {
                if (pNode_->pPrev)
                    pNode_->pPrev->pNext = pNode_->pNext;
                if (pNode_->pNext)
                    pNode_->pNext->pPrev = pNode_->pPrev;
            }
            void _DetachFromDeleted(_SNode* pNode_)
            {
                _Detach(pNode_);

                if (!pNode_->pPrev)
                    _pDeletedHead = pNode_->pNext;
                if (!pNode_->pNext)
                    _pDeletedTail = pNode_->pPrev;
            }

        public:
            struct const_iterator
            {
            public:
                _SNode* pNode{ nullptr };

                const_iterator() {}
                const_iterator(_SNode* pNode_) :
                    pNode(pNode_)
                {
                }
				inline explicit operator bool() const
                {
                    return (pNode != 0 && pNode->Newed);
                }
                const_iterator& operator++()
                {
                    pNode = pNode->pNext;
                    return *this;
                }
                const_iterator operator++(int)
                {
                    auto Ret = *this;
                    ++(*this);
                    return Ret;
                }
				inline size_t Index(void) const
                {
                    return pNode->Index;
                }
				inline bool operator == (const const_iterator& Iterator_) const
                {
                    return (pNode == Iterator_.pNode);
                }
				inline bool operator != (const const_iterator& Iterator_) const
                {
                    return !(*this == Iterator_);
                }
				inline bool operator < (const const_iterator& Iterator_) const
                {
                    return (pNode->Index < Iterator_.pNode->Index);
                }
            };
            struct iterator : public const_iterator
            {
                iterator()
                {
				}
                iterator(_SNode* pNode_) :
                    const_iterator(pNode_)
                {
				}
				iterator& operator++()
				{
					const_iterator::operator++();
					return *this;
				}
				iterator operator++(int)
				{
					auto Ret = *this;
					++(*this);
					return Ret;
				}
			};
            CListVoid()
            {
            }
            CListVoid(size_t Size_)
            {
                resize(Size_);
            }
            CListVoid(const CListVoid& Var_)
            {
                for (auto it = Var_.begin(); it != Var_.end(); ++it)
                    emplace_at(it.Index());
            }
            CListVoid(CListVoid&& Var_) :
                _Nodes(std::move(Var_._Nodes)),
                _pDeletedHead(Var_._pDeletedHead),
                _pDeletedTail(Var_._pDeletedTail),
                _pNewedHead(Var_._pNewedHead),
                _pNewedTail(Var_._pNewedTail),
                _Size(Var_._Size)
            {
                Var_._pDeletedHead = nullptr;
                Var_._pDeletedTail = nullptr;
                Var_._pNewedHead = nullptr;
                Var_._pNewedTail = nullptr;
                Var_._Size = 0;
            }
            virtual ~CListVoid()
            {
                clear();

                for (auto& pNode : _Nodes)
                    operator delete(pNode);
            }
            void reserve(size_t Size_)
            {
                auto OldSize = _Nodes.size();

                if (Size_ <= OldSize)
                    return;

                _Nodes.resize(Size_);

                for (size_t i = OldSize; i < Size_; ++i)
                {
                    _Nodes[i] = (_SNode*) operator new(sizeof(_SNode));
                    _Nodes[i]->Index = i;
                    _AttachToDeleted(_Nodes[i]);
                }
            }
            void resize(size_t Size_)
            {
                if (_Size > 0)
                    return;

                for (size_t i = 0; i < Size_; ++i)
                    emplace();
            }
            CListVoid& operator = (const CListVoid& Var_)
            {
                this->~CListVoid();
                new (this) CListVoid(Var_);
                return *this;
            }
            CListVoid& operator = (CListVoid&& Var_)
            {
                this->~CListVoid();
                new (this) CListVoid(std::move(Var_));
                return *this;
            }

            const_iterator get(size_t Index_) const
            {
                if (Index_ < 0 ||
                    Index_ >= _Nodes.size() ||
                    !_Nodes[Index_]->Newed)
                    return const_iterator();

                return const_iterator(_Nodes[Index_]);
            }
            iterator get(size_t Index_)
            {
                if (Index_ < 0 ||
                    Index_ >= _Nodes.size() ||
                    !_Nodes[Index_]->Newed)
                    return iterator();

                return iterator(_Nodes[Index_]);
            }
			inline const_iterator begin(void) const // for just iterate
            {
                return const_iterator(_pNewedHead);
            }
			inline const_iterator end(void) const // for just iterate
            {
                return const_iterator();
            }
			inline const_iterator last(void) const
			{
				return const_iterator(_pNewedTail);
			}
			inline iterator begin(void)
            {
                return iterator(_pNewedHead);
            }
			inline iterator end(void)
            {
                return iterator();
            }
			inline iterator last(void)
			{
				return iterator(_pNewedTail);
			}
			inline size_t capacity(void) const { return _Nodes.size(); }
			inline size_t size(void) const { return _Size; }
            size_t new_index(void) const
            {
                if (_pDeletedHead)
                    return _pDeletedHead->Index;
                else
                    return _Nodes.size();
            }
			iterator emplace(void)
            {
                _SNode* pNode;

                // DetachFromDeleted ////////////////////////////////////
                if (_pDeletedHead)
                {
                    pNode = _pDeletedHead;
                    _DetachFromDeleted(pNode);
                }
                else
                {
                    _Nodes.emplace_back(nullptr);

                    try
                    {
                        _Nodes.back() = (_SNode*) operator new(sizeof(_SNode));
                    }
                    catch (...)
                    {
                        _Nodes.pop_back();
						throw;
                    }

                    pNode = _Nodes.back();
                    pNode->Index = _Nodes.size() - 1;
                }

                _AttachToNewed(pNode);

                ++_Size;

                return iterator(pNode);
            }
			iterator emplace_at(size_t Index_)
            {
                _SNode* pNode;

                // _Nodes 범위 이내 이면
                if (Index_ < _Nodes.size())
                {
					if (_Nodes[Index_]->Newed)
						THROWEX();

                    pNode = _Nodes[Index_];
                }
                else
                {
                    auto LastSize = _Nodes.size();

                    for (auto i = LastSize; i < Index_ + 1; ++i)
                    {
                        _Nodes.emplace_back(nullptr);

                        try
                        {
                            _Nodes.back() = (_SNode*) operator new(sizeof(_SNode));
                        }
                        catch (...)
                        {
                            _Nodes.pop_back();
							throw;
                        }

                        _Nodes.back()->Index = _Nodes.size() - 1;
                        _AttachToDeleted(_Nodes.back());
                    }

                    pNode = _Nodes.back();
                }

                _DetachFromDeleted(pNode);
                _AttachToNewed(pNode);

                ++_Size;

                return iterator(pNode);
            }

        private:
            bool _erase(_SNode* pNode_)
            {
                if (!pNode_->Newed)
                    return false;

                _Detach(pNode_);

                if (!pNode_->pPrev)
                    _pNewedHead = pNode_->pNext;
                if (!pNode_->pNext)
                    _pNewedTail = pNode_->pPrev;


                // AttachToDeleted ////////////////////
                _AttachToDeleted(pNode_);

                --_Size;

                return true;
            }

        public:
			inline bool erase(const_iterator Iterator_)
            {
                return _erase(Iterator_.pNode);
            }
            bool erase(size_t Index_)
            {
                if (Index_ < 0 ||
                    Index_ >= _Nodes.size())
                    return false;

                return _erase(_Nodes[Index_]);
            }
			inline void pop_front(void)
			{
				_erase(_pNewedHead);
			}
			inline void pop_back(void)
			{
				_erase(_pNewedTail);
			}
			void clear(void) // no thread safe
            {
                for (auto it = begin();
                    it;)
                {
                    auto itDel = it;
                    ++it;

                    erase(itDel);
                }
            }
        };
    }
}