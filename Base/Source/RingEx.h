#pragma once


#include "Base.h"


namespace rso
{
    namespace base
    {
        using namespace std;

        template<typename TType>
        class CRingEx : private vector<TType>
        {
            using _TBase = vector<TType>;
            size_type  _FrontIndex = 0;

        public:
            class const_iterator
            {
                CRingEx* _Ring = nullptr;
                size_t _CurIndex = 0;

            public:
                const_iterator()
                {
                }
                const_iterator(CRingEx* Ring_) :
                    _Ring(Ring_), _CurIndex(Ring_->_FrontIndex)
                {
                }
				inline reference operator*() const
                {
                    return ((_TBase*)_Ring)->operator[](_CurIndex);
                }
				inline pointer operator->() const
                {
                    return &((_TBase*)_Ring)->operator[](_CurIndex);
                }
                const_iterator& operator++()
                {
                    _CurIndex = (_CurIndex + 1) % _Ring->size();
                    if (_CurIndex == _Ring->_FrontIndex)
                    {
                        _Ring = nullptr;
                        _CurIndex = 0;
                    }

                    return (*this);
                }
                const_iterator operator++(int)
                {
                    const_iterator Tmp = *this;
                    ++*this;
                    return (Tmp);
                }
				inline bool operator==(const const_iterator& Right_) const
                {
                    return ((_Ring == Right_._Ring && _Ring == nullptr) || (_Ring == Right_._Ring && _CurIndex == Right_._CurIndex));
                }
				inline bool operator!=(const const_iterator& Right_) const
                {
                    return (!(*this == Right_));
                }
            };
            class iterator : public const_iterator
            {
            public:
                iterator()
                {
                }
                iterator(CRingEx* Ring_) :
                    const_iterator(Ring_)
                {
                }
				inline reference operator*() const
                {
                    return ((reference)**(const_iterator*)this);
                }
				inline pointer operator->() const
                {
                    return ((const_iterator*)this)->operator->();
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

            CRingEx(size_t Size_)
            {
                reserve(Size_);
            }
            template<typename... TArgs>
            void emplace_back(TArgs&&... Args_)
            {
                if (size() == capacity())
                {
                    vector<TType>::operator[](_FrontIndex) = { forward<TArgs>(Args_)... };
                    _FrontIndex = ((_FrontIndex + 1) % capacity());
                }
                else
                {
                    vector<TType>::emplace_back(forward<TArgs>(Args_)...);
                }
            }
            reference operator[](size_type _Pos)
            {
                if (_FrontIndex + _Pos < size())
                    return vector<TType>::operator[](_FrontIndex + _Pos);
                else
                    return vector<TType>::operator[]((_FrontIndex + _Pos) % size());
            }
            iterator begin()
            {
                if (empty())
                    return iterator();

                return iterator(this);
            }
            const_iterator begin() const
            {
                if (empty())
                    return const_iterator();

                return const_iterator(this);
            }
			inline iterator end()
            {
                return iterator();
            }
			inline const_iterator end() const
            {
                return const_iterator();
            }
        };
    }
}
