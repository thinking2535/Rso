#pragma once


#include <mutex>
#include <vector>
#include <Rso/Core/Core.h>


namespace rso
{
    namespace data
    {
        using namespace std;
        using namespace core;

        template<typename _TData>
        class CDoubleBuffer sealed
        {
        private:
            using _TDatas = vector<_TData>;

            mutex _IndexMutex;
            bool _Loaded = false; // false : 최근 Get 이후 새로운 데이터가 Set되지 않음
            size_t _AccessIndex = 0; // Get Thread 에서만 접근
            size_t _LoadedIndex = 1; // Get, Set Thread 에서 접근
            size_t _FreeIndex = 2; // Set Thread 에서만 접근
            _TDatas _Datas = _TDatas(3);

            void _Set(void)
            {
                _IndexMutex.lock();
                Swap(_LoadedIndex, _FreeIndex);
                _Loaded = true;
                _IndexMutex.unlock();
            }

        public:
            void Set(const _TData& Data_)
            {
                _Datas[_FreeIndex] = Data_;
                _Set();
            }
            void Set(_TData&& Data_)
            {
                _Datas[_FreeIndex] = std::move(Data_);
                _Set();
            }
            const _TData* Get(void)
            {
                if (_IndexMutex.try_lock())
                {
                    if (_Loaded)
                    {
                        Swap(_AccessIndex, _LoadedIndex);
                        _Loaded = false;
                    }

                    _IndexMutex.unlock();
                }

                return &_Datas[_AccessIndex];
            }
        };
    }
}
