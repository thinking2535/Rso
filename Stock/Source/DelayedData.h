#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
        using namespace base;

        template<typename TData>
		class CDelayedData : public vector<TData>
		{
            size_t _LastIndex = 0;

        public:
            using TDatas = vector<TData>;

            CDelayedData(size_t Size_) :
                TDatas(Size_ + 1)
            {
            }
            template<class... _Valty>
            void emplace_back(_Valty&&... _Val) = delete;
            template<class... _Valty>
            iterator emplace(const_iterator _Where, _Valty&&... _Val) = delete;
            iterator erase(const_iterator _Where) = delete;
            iterator erase(const_iterator _First_arg, const_iterator _Last_arg) = delete;
            TData push_back(const TData& Val_)
            {
                (*this)[_LastIndex] = Val_;

                _LastIndex = (_LastIndex + 1) % size();

                return (*this)[_LastIndex];
            }
            TData push_back(TData&& Val_)
            {
                (*this)[_LastIndex] = forward<TData>(Val_);

                _LastIndex = (_LastIndex + 1) % size();

                return (*this)[_LastIndex];
            }
        };
    }
}
