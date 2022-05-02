#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
        using namespace base;

		class CGroupData : public vector<TValue>
		{
        public:
            using TDatas = vector<TValue>;

        private:
            TDatas _Cnts;
            TDatas _AllDatas;
            vector<size_t> _Indices;
            size_t _EndIndex = 0;

		public:
            CGroupData(const TDatas& Cnts_) :
                TDatas(Cnts_.size()),
                _Cnts(Cnts_),
                _AllDatas([](const TDatas& Cnts_) { size_t sz = 0; for (auto& i : Cnts_) sz += i; return sz; }(Cnts_)),
                _Indices([](const TDatas& Cnts_) { vector<size_t> Indices; size_t Index = 0; for (auto& i : Cnts_) { Indices.emplace_back(Index); Index += i; } return Indices; }(Cnts_))
            {
            }
            template<class... _Valty>
            void emplace_back(_Valty&&... _Val) = delete;
            template<class... _Valty>
            iterator emplace(const_iterator _Where, _Valty&&... _Val) = delete;
            iterator erase(const_iterator _Where) = delete;
            iterator erase(const_iterator _First_arg, const_iterator _Last_arg) = delete;
            size_t all_size(void) const
            {
                return _AllDatas.size();
            }
            void push_back(TValue Val_)
            {
                for (size_t i = 0; i < _Indices.size(); ++i)
                    __super::operator[](i) -= _AllDatas[(_EndIndex + _Indices[i]) % _AllDatas.size()];

                for (size_t i = 0; i < _Indices.size() - 1; ++i)
                    __super::operator[](i) += _AllDatas[(_EndIndex + _Indices[i + 1]) % _AllDatas.size()];

                _AllDatas[_EndIndex] = Val_;
                __super::back() += Val_;
                _EndIndex = (_EndIndex + 1) % _AllDatas.size();
            }
            double avg(size_t Index_) const
            {
                return (double(__super::operator[](Index_)) / double(_Cnts[Index_]));
            }
        };
    }
}
