#pragma once


// operator new 를 쓰는 이유는 ConsDesCall==false 모드에서 reserve 시에는 생성자가 호출되어선 안되기 때문
// 또한 ~CList 에서 객체의 소멸자가 두번 호출되지 않도록


#include <Rso/Core/Core.h>
#include <new>
#include <vector>


namespace rso
{
    namespace base
    {
        using namespace core;
        using namespace std;

        template<typename TData>
        class CList
        {
		public:
			using value_type = TData;

        protected:
            struct _SNode
            {
                size_t Index = 0;
                _SNode* Next = nullptr;
                _SNode* Prev = nullptr;
                bool Newed = false; // for emplace_at
                TData Data{};
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
				inline const TData& operator*() const
                {
                    return _Node->Data;
                }
				inline const TData* operator->() const
                {
                    return &_Node->Data;
                }
				inline const TData* operator&() const
				{
					return &_Node->Data;
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
				using const_iterator::operator*;
				using const_iterator::operator->;
				using const_iterator::_Node;

                iterator()
                {}
                iterator(_SNode* Node_) :
                    const_iterator(Node_)
                {}
				inline TData& operator*()
                {
                    return _Node->Data;
                }
				inline TData* operator->()
                {
                    return &_Node->Data;
                }
				inline TData* operator&()
				{
					return &_Node->Data;
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
            CList()
            {
            }
            template<typename... TArgsEmplace>
            CList(size_t Size_, TArgsEmplace&&... Args_)
            {
                resize(Size_, forward<TArgsEmplace>(Args_)...);
            }
            CList(const CList& Var_)
            {
                for (auto it = Var_.begin(); it != Var_.end(); ++it)
                    emplace_at(it.Index(), (*it));
            }
            CList(CList&& Var_) :
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
            virtual ~CList()
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
            template<typename... TArgsEmplace>
            void resize(size_t Size_, TArgsEmplace&&... Args_)
            {
                if (_Size > 0)
                    return;

                for (size_t i = 0; i < Size_; ++i)
                    emplace(forward<TArgsEmplace>(Args_)...);
            }
            CList& operator = (const CList& Var_)
            {
                this->~CList();
                new (this) CList(Var_);
                return *this;
            }
            CList& operator = (CList&& Var_)
            {
                this->~CList();
                new (this) CList(std::move(Var_));
                return *this;
            }

            inline TData& operator[](size_t Index_)
            {
                return _Nodes[Index_]->Data;
            }
			inline const TData& operator[](size_t Index_) const
            {
                return _Nodes[Index_]->Data;
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
			inline const TData& front(void) const
            {
                return _NewedHead->Data;
            }
			inline TData& front(void)
            {
                return _NewedHead->Data;
            }
			inline const TData& back(void) const
            {
                return _NewedTail->Data;
            }
			inline TData& back(void)
            {
                return _NewedTail->Data;
            }
			inline size_t capacity(void) const { return _Nodes.size(); }
			inline size_t size(void) const { return _Size; }
			inline bool empty(void) const { return (_Size <= 0); }
            size_t new_index(void) const
            {
                if (_DeletedHead)
                    return _DeletedHead->Index;
                else
                    return _Nodes.size();
            }
            template<typename... TArgsEmplace>
			iterator emplace(TArgsEmplace&&... Args_)
            {
                _SNode* Node = nullptr;

                // DetachFromDeleted ////////////////////////////////////
                if (_DeletedHead)
                {
                    new(&_DeletedHead->Data)TData(forward<TArgsEmplace>(Args_)...);
                    Node = _DeletedHead;
                    _DetachFromDeleted(Node);
                }
                else
                {
                    _Nodes.emplace_back(nullptr);

                    try
                    {
                        _Nodes.back() = (_SNode*) operator new(sizeof(_SNode));

                        try
                        {
                            new(&_Nodes.back()->Data)TData(forward<TArgsEmplace>(Args_)...);
                        }
                        catch (...)
                        {
                            operator delete(_Nodes.back());
							throw;
                        }
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

            template<typename... TArgsEmplace>
			iterator emplace_at(size_t Index_, TArgsEmplace&&... Args_)
            {
                _SNode* Node = nullptr;

                // _Nodes 범위 이내 이면
                if (Index_ < _Nodes.size())
                {
					if (_Nodes[Index_]->Newed)
						THROWEX();

                    new(&_Nodes[Index_]->Data)TData(forward<TArgsEmplace>(Args_)...);
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

                    new(&_Nodes.back()->Data)TData(forward<TArgsEmplace>(Args_)...);

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

                Node_->Data.~TData();

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