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

        template<typename TData, bool Call_ = true>
        class CVector final
        {
        private:
            struct _SNode
            {
                bool Constructed;
                TData Data;
            };

            using _TDatas = vector<_SNode*>;

            _TDatas _Nodes;
            size_t _Size = 0;

            template<bool>
            void _destruct(_SNode* Node_)
            {
                Node_->Constructed = false;
                Node_->Data.~TData();
            }
            template<>
            void _destruct<false>(_SNode* Node_)
            {
            }

        public:
            struct iterator
            {
            private:
                CVector* _Vector = nullptr;
                size_t _Index = 0;

            public:
                iterator(CVector* Queue_, size_t Index_) :
                    _Vector(Queue_), _Index(Index_)
                {
                }
				inline TData& operator*()
                {
                    return _Vector->_Nodes[_Index]->Data;
                }
				inline TData* operator->()
                {
                    return &_Vector->_Nodes[_Index]->Data;
                }
                iterator operator++()
                {
                    ++_Index;
                    return *this;
                }
                iterator operator++(int)
                {
                    auto Ret = *this;
                    ++(*this);
                    return Ret;
                }
				inline bool operator == (const iterator& Iterator_)
                {
                    return (_Vector == Iterator_._Vector && _Index == Iterator_._Index);
                }
				inline bool operator != (const iterator& Iterator_)
                {
                    return !(operator == (Iterator_));
                }
            };

            CVector()
            {
            }
            template<typename... TArgsEmplace>
            CVector(size_t Size_, TArgsEmplace&&... Args_)
            {
                resize(Size_, forward<TArgsEmplace>(Args_)...);
            }
            CVector(const CVector& Var_) = delete;
            CVector(CVector&& Var_) = delete;
            virtual ~CVector()
            {
                for (auto& Node : _Nodes)
                {
                    if (Node->Constructed)
                        Node->Data.~TData();
                }

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
                    _Nodes[i]->Constructed = false;
                }
            }
            template<typename... TArgsEmplace>
            void resize(size_t Size_, TArgsEmplace&&... Args_)
            {
                auto OldSize = _Size;
                if (Size_ <= OldSize)
                    return;

                reserve(Size_);

                for (size_t i = OldSize; i < Size_; ++i)
                {
                    if (!emplace(forward<TArgsEmplace>(Args_)...))
                        THROWEX();
                }
            }
            CVector operator = (const CVector& Var_) = delete;
            CVector operator = (CVector&& Var_) = delete;
			inline size_t size(void) const
            {
                return _Size;
            }
			inline bool empty(void) const
            {
                return (_Size == 0);
            }
            template<typename... TArgsEmplace>
            bool emplace_back(TArgsEmplace&&... Args_)
            {
                if (_Size >= _Nodes.size())
                {
                    try
                    {
                        _Nodes.emplace_back(nullptr);

                        try
                        {
                            _Nodes.back() = (_SNode*)operator new(sizeof(_SNode));

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
                            return false;
                        }
                    }
                    catch (...)
                    {
                        return false;
                    }

                    _Nodes.back()->Constructed = true;
                }
                else
                {
                    if (!_Nodes[_Size]->Constructed)
                    {
                        try
                        {
                            new(&_Nodes[_Size]->Data)TData(forward<TArgsEmplace>(Args_)...);
                        }
                        catch (...)
                        {
                            return false;
                        }

                        _Nodes[_Size]->Constructed = true;
                    }
                }

                ++_Size;

                return true;
            }
            void pop_back(void)
            {
                if (_Size == 0)
                    return;

                _destruct<Call_>(_Nodes[--_Size]);
            }
            inline TData& front(void)
            {
                return _Nodes[0]->Data;
            }
            inline TData& back(void)
            {
                return _Nodes[_Size - 1]->Data;
            }
            inline const TData& front(void) const
            {
                return _Nodes[0]->Data;
            }
            inline const TData& back(void) const
            {
                return _Nodes[_Size - 1]->Data;
            }
            inline iterator begin(void)
            {
                return iterator(this, 0);
            }
            inline iterator end(void)
            {
                return iterator(this, _Size);
            }
            inline TData& operator[](size_t Index_)
            {
                return _Nodes[Index_]->Data;
            }
            void clear(void)
            {
                for (size_t i = 0; i < _Size; ++i)
                    _destruct<Call_>(_Nodes[i]);

                _Size = 0;
            }
        };
    }
}