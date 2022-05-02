#pragma once


#include <Rso/Core/Core.h>
#include <vector>
#include <new>


namespace rso
{
    namespace base
    {
        using namespace core;
        using namespace std;

        template<typename TType>
        class CRing final
        {
        private:
            using _TDatas = vector<TType*>;

            _TDatas _Datas;
            size_t _Size = 0;
            size_t _Head = 0;

            size_t _Tail(void) const
            {
                if (_Head + _Size < _Datas.size())
                    return (_Head + _Size);
                else
                    return (_Head + _Size - _Datas.size());
            }

        public:
            class iterator
            {
                CRing* _Ptr = nullptr;
                size_t _Cur = 0;

            public:
                iterator(CRing* Ptr_, size_t Cur_) :
                    _Ptr(Ptr_), _Cur(Cur_)
                {
                }
				inline TType* operator->()
                {
                    return _Ptr->_Datas[_Cur];
                }
				inline TType& operator*()
                {
                    return *_Ptr->_Datas[_Cur];
                }
                iterator operator++()
                {
                    if (_Cur == _Ptr->_Tail())
                    {
                        _Ptr = nullptr;
                        _Cur = 0;
                    }
                    else
                    {
                        ++_Cur;
                        _Cur %= _Ptr->_Datas.size();
                    }

                    return *this;
                }
                iterator operator++(int)
                {
                    auto Ret = *this;
                    ++(*this);
                    return Ret;
                }
				inline bool operator == (const iterator& it_) const
                {
                    return (_Ptr == it_._Ptr && _Cur == it_._Cur);
                }
				inline bool operator != (const iterator& it_) const
                {
                    return !(*this == it_);
                }
            };
            CRing()
            {
            }
            CRing(size_t Size_)
            {
                reserve(Size_);
            }
            CRing(const CRing& Var_)
            {
                _Datas = Var_._Datas;
                _Size = Var_._Size;
                _Head = Var_._Head;
            }
            CRing(CRing&& Var_) :
                _Datas(std::move(Var_._Datas)),
                _Size(Var_._Size),
                _Head(Var_._Head)
            {
                Var_._Size = 0;
                Var_._Head = 0;
            }
            virtual ~CRing()
            {
                clear();
            }
            CRing operator = (const CRing& Var_)
            {
                this->~CRing();
                new (this) CRing(Var_);
            }
            CRing operator = (CRing&& Var_)
            {
                this->~CRing();
                new (this) CRing(std::move(Var_));
            }
			inline iterator begin(void)
            {
                return iterator(this);
            }
			inline iterator end(void)
            {
                return iterator(nullptr, 0);
            }
			inline TType& front(void)
            {
                return operator[](0);
            }
			inline TType& back(void)
            {
                return operator[](_Size - 1);
            }
			inline const TType& front(void) const
            {
                return operator[](0);
            }
			inline const TType& back(void) const
            {
                return operator[](_Size - 1);
            }
            inline size_t size(void) const
            {
                return _Size;
            }
            inline size_t capacity(void) const
            {
                return _Datas.size();
            }
            inline bool empty(void) const
            {
                return (_Size == 0);
            }
            inline bool full(void) const
            {
                return (_Size == _Datas.size());
            }
            void reserve(size_t Size_)
            {
                if (_Size > 0)
                    THROWEX();

                _Datas.resize(Size_);

                try
                {
                    for (auto& i : _Datas)
                        i = (TType*)operator new(sizeof(TType));
                }
                catch (...)
                {
                    for (auto& i : _Datas)
                    {
                        if (!i)
                            break;

                        operator delete(i);
                    }
                    _Datas.clear();

					throw;
                }
            }
            template<typename... TArgs>
            void emplace(TArgs&&... Args_)
            {
                if (full())
                {
                    _Datas[_Head]->~TType();
                    new(_Datas[_Head])TType{ forward<TArgs>(Args_)... };

                    ++_Head;
                    if (_Head == _Datas.size())
                        _Head = 0;
                }
                else
                {
                    new(_Datas[_Tail()])TType{ forward<TArgs>(Args_)... };
                    ++_Size;
                }
            }
            void pop(void)
            {
                if (empty())
                    return;

                _Datas[_Head]->~TType();

                ++_Head;
                if (_Head == _Datas.size())
                    _Head = 0;

                --_Size;
            }
            size_t _Index(size_t Index_) const
            {
                if (_Head + Index_ < _Datas.size())
                    return _Head + Index_;
                else
                    return _Head + Index_ - _Datas.size();
            }
            size_t _ReverseIndex(size_t Index_) const
            {
                if (_Head + _Size - 1 - Index_ < 0)
                    return _Head + _Size - 1 - Index_ + _Datas.size();
                else if (_Head + _Size - 1 - Index_ >= _Datas.size())
                    return _Head + _Size - 1 - Index_ - _Datas.size();
                else
                    return _Head + _Size - 1 - Index_;
            }
			inline TType& operator[](size_t Index_)
            {
                return *_Datas[_Index(Index_)];
            }
			inline const TType& operator[](size_t Index_) const
            {
                return *_Datas[_Index(Index_)];
            }
			inline TType& reverse_at(size_t Index_)
            {
                return *_Datas[_ReverseIndex(Index_)];
            }
			inline const TType& reverse_at(size_t Index_) const
            {
                return *_Datas[_ReverseIndex(Index_)];
            }
			inline void clear(void)
            {
                for (; !empty(); pop());
            }
        };
    }
}