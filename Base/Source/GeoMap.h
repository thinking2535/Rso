// TODO : Key 들간의 거리는 전체 Range에서 Key들 간의 밀도에 따라 거리가 달라지도록 할것.

#pragma once


#include "Base.h"
#include <array>
#include <map>
#include <Rso/Core/Range.h>


namespace rso
{
    namespace base
    {
        using namespace std;

        template<typename TKey, typename TValue, size_t Dimension_, bool UseRange_ = false>
        class CGeoMap
        {
        public:
            using TKeys = array<TKey, Dimension_>;
        private:
            using _TCounter = uint64;
            struct _SValueCounter
            {
                TValue Value;
                _TCounter Counter;

                _SValueCounter(const TValue& Value_, _TCounter Counter_) :
                    Value(Value_), Counter(Counter_) {}
            };
            using _TDatas = multimap<TKeys, _SValueCounter>;
            using _TDatasIt = typename _TDatas::iterator;
            using _TSubData = multimap<TKey, _TDatasIt>;
            using _TSubDataIt = typename _TSubData::iterator;
            using _TSubMaps = array<_TSubData, Dimension_>;
            using _TOutDatas = multimap<TKey, _TDatasIt>;

            class iterator
            {
                CGeoMap* _Ptr = nullptr;

            public:
                iterator()
                {
                }
                iterator(CGeoMap* Ptr_) :
                    _Ptr(Ptr_)
                {
                }
				inline typename _TOutDatas::reference operator * ()
                {
                    return *(_Ptr->_OutDatas.begin());
                }
				inline typename _TOutDatas::iterator operator -> ()
                {
                    return _Ptr->_OutDatas.begin();
                }
				inline typename _TOutDatas::iterator end(void) const
                {
                    return _Ptr->_OutDatas.end();
                }
				inline bool operator == (typename _TOutDatas::const_iterator& It_) const
                {
                    return (_Ptr->_OutDatas.begin() == It_);
                }
				inline bool operator != (typename _TOutDatas::const_iterator& It_) const
                {
                    return !(operator == (It_));
                }
				inline void operator ++ ()
                {
                    _Ptr->_Iterate();
                }
            };

        private:
            template<bool>
            class _CDistance
            {
                using _TRanges = array<CRange<TKey>, Dimension_>;

                _TRanges _Ranges;
                TKey _KeyDistance(size_t Num_, const TKey& Key0_, const TKey& Key1_)
                {
                    if (_Ranges[Num_].Range() == TKey{})
                        return TKey{};

                    return (Key0_ - Key1_) / _Ranges[Num_].Range();
                }

            public:
                inline void Set(const TKeys& Keys_)
                {
                    for (size_t i = 0; i < Dimension_; ++i)
                        _Ranges[i].Set(Keys_[i]);
                }
                inline TKey KeyDistance(size_t Num_, const TKey& Key0_, const TKey& Key1_)
                {
                    auto Distance = _KeyDistance(Num_, Key0_, Key1_);
                    return (Distance < TKey{} ? -Distance : Distance);
                }
                bool KeysDistance(size_t Num_, const TKeys& Keys0_, const TKeys& Keys1_, TKey& KeysDistance_)
                {
                    bool IsValidDistance = true;
                    KeysDistance_ = TKey{};
                    auto Dist = KeyDistance(Num_, Keys0_[Num_], Keys1_[Num_]);
                    for (size_t i = 0; i < Dimension_; ++i)
                    {
                        auto KeyDist = KeyDistance(i, Keys0_[i], Keys1_[i]);
                        KeysDistance_ += (KeyDist * KeyDist);

                        if (KeyDist > Dist)
                            IsValidDistance = false;
                    }

                    KeysDistance_ = sqrt(KeysDistance_ / Dimension_);

                    return IsValidDistance;
                }
                void clear(void)
                {
                    for (auto& Range : _Ranges)
                        Range.clear();
                }
            };
            template<>
            class _CDistance<false>
            {
                inline TKey _KeyDistance(size_t Num_, const TKey& Key0_, const TKey& Key1_)
                {
                    return (Key0_ - Key1_);
                }

            public:
                inline void Set(const TKeys& Keys_)
                {
                }
                TKey KeyDistance(size_t Num_, const TKey& Key0_, const TKey& Key1_)
                {
                    auto Distance = _KeyDistance(Num_, Key0_, Key1_);
                    return (Distance < TKey{} ? -Distance : Distance);
                }
                bool KeysDistance(size_t Num_, const TKeys& Keys0_, const TKeys& Keys1_, TKey& KeysDistance_)
                {
                    bool IsValidDistance = true;
                    KeysDistance_ = TKey{};
                    auto Dist = KeyDistance(Num_, Keys0_[Num_], Keys1_[Num_]);
                    for (size_t i = 0; i < Dimension_; ++i)
                    {
                        auto KeyDist = KeyDistance(i, Keys0_[i], Keys1_[i]);
                        KeysDistance_ += (KeyDist * KeyDist);

                        if (KeyDist > Dist)
                            IsValidDistance = false;
                    }

                    KeysDistance_ = sqrt(KeysDistance_ / Dimension_);

                    return IsValidDistance;
                }
                void clear(void)
                {
                }
            };
        public: // 디버그 모드에서만 _SIterator 가 private이라 액세스 못하다고 에러나는데... 왜?
            struct _SIterator // iterator info about each key
            {
                typename _TSubDataIt Upper;
                typename _TSubDataIt Lower;
                TKey ValidDistance{}; // 다음 근접노드를 찾기위해 _Iterators 순회시 속도 향상을 위해 순회 Stop 하기위함.
                // 이 값 보다 가까운 거리의 노드는 현 _SIterator 에 없음.
                // 순회중인 Keys 에서 현재 Num의 Key가 가장 큰 값(들중 하나)이면 그때 구해진 ValidDistance 는 유효
                // _SIterator 를 순회 할 때 다음 조건이면 순회 Stop
                // - 이 값이 _ValidDistance 보다 크면
                // - 순회할 노드가 없는 경우
            };
        private:
            using _TIterators = array<_SIterator, Dimension_>;

            _TDatas _Datas;
            _TSubMaps _SubDatas;
            _CDistance<UseRange_> _Distance;
            _TOutDatas _OutDatas;
            _TCounter _Counter = 0;
            _TIterators _Iterators;
            TKey _ValidDistance{}; // _OutDatas의 최소 Key가 이 값 이하인 경우 유효
            TKeys _FindingKeys;

            bool _IncIterator(size_t Num_)
            {
                if (_Iterators[Num_].ValidDistance > _ValidDistance)
                    return false;

                auto Current = _SubDatas[Num_].end();

                for (;;)
                {
                    if (_Iterators[Num_].Upper == _SubDatas[Num_].end())
                    {
                        if (_Iterators[Num_].Lower == _SubDatas[Num_].end())
                        {
                            return false;
                        }
                        else
                        {
                            Current = _Iterators[Num_].Lower;

                            if (_Iterators[Num_].Lower == _SubDatas[Num_].begin())
                                _Iterators[Num_].Lower = _SubDatas[Num_].end();
                            else
                                --_Iterators[Num_].Lower;
                        }
                    }
                    else
                    {
                        if (_Iterators[Num_].Lower == _SubDatas[Num_].end())
                        {
                            Current = _Iterators[Num_].Upper;

                            if (_Iterators[Num_].Upper != _SubDatas[Num_].end())
                                ++_Iterators[Num_].Upper;
                        }
                        else
                        {
                            if (_Iterators[Num_].Upper->first - _FindingKeys[Num_] < _FindingKeys[Num_] - _Iterators[Num_].Lower->first)
                            {
                                Current = _Iterators[Num_].Upper;

                                ++_Iterators[Num_].Upper;
                            }
                            else
                            {
                                Current = _Iterators[Num_].Lower;

                                if (_Iterators[Num_].Lower == _SubDatas[Num_].begin())
                                    _Iterators[Num_].Lower = _SubDatas[Num_].end();
                                else
                                    --_Iterators[Num_].Lower;
                            }
                        }
                    }

                    if (Current->second->second.Counter == _Counter)
                        continue;

                    Current->second->second.Counter = _Counter;

                    auto IsValidDistance = _Distance.KeysDistance(Num_, _FindingKeys, Current->second->first, _Iterators[Num_].ValidDistance);
                    _OutDatas.emplace(_Iterators[Num_].ValidDistance, Current->second);

                    if (IsValidDistance)
                        return true;
                }
            }
            void _IncIterators(void)
            {
                auto NewValidDistance = (numeric_limits<TKey>::max)();

                for (size_t i = 0; i < Dimension_; ++i)
                {
                    if (!_IncIterator(i))
                        continue;

                    if (_Iterators[i].ValidDistance < NewValidDistance)
                        NewValidDistance = _Iterators[i].ValidDistance;
                }

                _ValidDistance = NewValidDistance;
            }
            void _Iterate(void)
            {
                _OutDatas.erase(_OutDatas.begin());

                auto itBegin = _OutDatas.begin();
                if (itBegin == _OutDatas.end() ||
                    itBegin->first > _ValidDistance)
                    _IncIterators();
            }

        public:
            inline size_t size(void) const
            {
                return _Datas.size();
            }
            void insert(const TKeys& Keys_, const TValue& Value_)
            {
                auto it = _Datas.emplace(Keys_, _SValueCounter(Value_, 0));
                for (size_t i = 0; i < Dimension_; ++i)
                    _SubDatas[i].emplace(Keys_[i], it);

                _Distance.Set(Keys_);
            }
            void clear(void)
            {
                _Counter = 0;
                _Distance.clear();

                for (size_t i = 0; i < Dimension_; ++i)
                    _SubDatas[i].clear();

                _Datas.clear();
            }
            iterator lower_bound(const TKeys& Keys_)
            {
                _ValidDistance = TKey{};
                for (auto& it : _Iterators)
                    it.ValidDistance = (TKey{}); // ()를 하지 않으면 에러 버그?

                if (!_OutDatas.empty())
                    _OutDatas.clear();

                _Distance.Set(Keys_);

                if (_Counter + 1 < _Counter)
                {
                    for (auto& it : _Datas)
                        it.second.Counter = 0;

                    _Counter = 0;
                }

                ++_Counter;
                _FindingKeys = Keys_;

                for (size_t i = 0; i < Dimension_; ++i)
                {
                    _Iterators[i].Upper = _SubDatas[i].upper_bound(_FindingKeys[i]);

                    if (_SubDatas[i].empty())
                    {
                        _Iterators[i].Lower = _SubDatas[i].end();
                    }
                    else
                    {
                        if (_Iterators[i].Upper == _SubDatas[i].begin())
                        {
                            _Iterators[i].Lower = _SubDatas[i].end();
                        }
                        else
                        {
                            _Iterators[i].Lower = _Iterators[i].Upper;
                            --_Iterators[i].Lower;
                        }
                    }
                }

                _IncIterators();

                return iterator(this);
            }
            inline typename _TOutDatas::const_iterator end(void) const
            {
                return _OutDatas.end();
            }
        };
    }
}
