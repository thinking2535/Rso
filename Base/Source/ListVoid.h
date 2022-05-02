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
                _SNode* Next;
                _SNode* Prev;
                bool Newed; // for emplace_at
            };

            using _TNodes = vector<_SNode*>;

            _TNodes _Nodes;
            _SNode* _DeletedHead = nullptr;
            _SNode* _DeletedTail = nullptr;
            _SNode* _NewedHead = nullptr;
            _SNode* _NewedTail = nullptr;
            size_t _Size = 0;

            void _AttachToNewed(_SNode* Node_)
            {
                Node_->Next = 0;
                Node_->Newed = true;

                if (_NewedTail == 0)
                {
                    Node_->Prev = 0;
                    _NewedHead = _NewedTail = Node_;
                }
                else
                {
                    Node_->Prev = _NewedTail;
                    _NewedTail->Next = Node_;
                    _NewedTail = Node_;
                }
            }
            void _AttachToDeleted(_SNode* Node_)
            {
                Node_->Next = 0;
                Node_->Newed = false;

                if (_DeletedTail == 0)
                {
                    Node_->Prev = 0;
                    _DeletedHead = _DeletedTail = Node_;
                }
                else
                {
                    Node_->Prev = _DeletedTail;
                    _DeletedTail->Next = Node_;
                    _DeletedTail = Node_;
                }
            }
            void _Detach(_SNode* Node_)
            {
                if (Node_->Prev)
                    Node_->Prev->Next = Node_->Next;
                if (Node_->Next)
                    Node_->Next->Prev = Node_->Prev;
            }
            void _DetachFromDeleted(_SNode* Node_)
            {
                _Detach(Node_);

                if (!Node_->Prev)
                    _DeletedHead = Node_->Next;
                if (!Node_->Next)
                    _DeletedTail = Node_->Prev;
            }

        public:
            struct const_iterator
            {
            protected:
                _SNode* _Node{ nullptr };

            public:
                const_iterator() {}
                const_iterator(_SNode* Node_) :
                    _Node(Node_)
                {
                }
				inline explicit operator bool() const
                {
                    return (_Node != 0 && _Node->Newed);
                }
                const_iterator& operator++()
                {
                    _Node = _Node->Next;
                    return *this;
                }
                const_iterator operator++(int)
                {
                    auto Ret = *this;
                    ++(*this);
                    return Ret;
                }
				inline _SNode* Node(void)
                {
                    return _Node;
                }
				inline size_t Index(void) const
                {
                    return _Node->Index;
                }
				inline bool operator == (const const_iterator& Iterator_) const
                {
                    return (_Node == Iterator_._Node);
                }
				inline bool operator != (const const_iterator& Iterator_) const
                {
                    return !(*this == Iterator_);
                }
				inline bool operator < (const const_iterator& Iterator_) const
                {
                    return (_Node->Index < Iterator_._Node->Index);
                }
            };
            struct iterator : public const_iterator
            {
                iterator()
                {
				}
                iterator(_SNode* Node_) :
                    const_iterator(Node_)
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
                _DeletedHead(Var_._DeletedHead),
                _DeletedTail(Var_._DeletedTail),
                _NewedHead(Var_._NewedHead),
                _NewedTail(Var_._NewedTail),
                _Size(Var_._Size)
            {
                Var_._DeletedHead = nullptr;
                Var_._DeletedTail = nullptr;
                Var_._NewedHead = nullptr;
                Var_._NewedTail = nullptr;
                Var_._Size = 0;
            }
            virtual ~CListVoid()
            {
                clear();

                for (auto& Node : _Nodes)
                    operator delete(Node);
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
                return const_iterator(_NewedHead);
            }
			inline const_iterator end(void) const // for just iterate
            {
                return const_iterator();
            }
			inline const_iterator last(void) const
			{
				return const_iterator(_NewedTail);
			}
			inline iterator begin(void)
            {
                return iterator(_NewedHead);
            }
			inline iterator end(void)
            {
                return iterator();
            }
			inline iterator last(void)
			{
				return iterator(_NewedTail);
			}
			inline size_t capacity(void) const { return _Nodes.size(); }
			inline size_t size(void) const { return _Size; }
            size_t new_index(void) const
            {
                if (_DeletedHead)
                    return _DeletedHead->Index;
                else
                    return _Nodes.size();
            }
			iterator emplace(void)
            {
                _SNode* Node = nullptr;

                // DetachFromDeleted ////////////////////////////////////
                if (_DeletedHead)
                {
                    Node = _DeletedHead;
                    _DetachFromDeleted(Node);
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

                    Node = _Nodes.back();
                    Node->Index = _Nodes.size() - 1;
                }

                _AttachToNewed(Node);

                ++_Size;

                return iterator(Node);
            }
			iterator emplace_at(size_t Index_)
            {
                _SNode* Node = nullptr;

                // _Nodes 범위 이내 이면
                if (Index_ < _Nodes.size())
                {
					if (_Nodes[Index_]->Newed)
						THROWEX();

                    Node = _Nodes[Index_];
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

                    Node = _Nodes.back();
                }

                _DetachFromDeleted(Node);
                _AttachToNewed(Node);

                ++_Size;

                return iterator(Node);
            }

        private:
            bool _erase(_SNode* Node_)
            {
                if (!Node_->Newed)
                    return false;

                _Detach(Node_);

                if (!Node_->Prev)
                    _NewedHead = Node_->Next;
                if (!Node_->Next)
                    _NewedTail = Node_->Prev;


                // AttachToDeleted ////////////////////
                _AttachToDeleted(Node_);

                --_Size;

                return true;
            }

        public:
			inline bool erase(const_iterator Iterator_)
            {
                return _erase(Iterator_.Node());
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
				_erase(_NewedHead);
			}
			inline void pop_back(void)
			{
				_erase(_NewedTail);
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