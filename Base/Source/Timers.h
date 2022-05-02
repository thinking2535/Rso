#pragma once


#include "Base.h"
#include <chrono>
#include <functional>


namespace rso
{
    namespace base
    {
        using namespace std;
        using namespace chrono;
        using namespace placeholders;

        template<typename TData>
        class CTimers
        {
            using _TCallback = function<void(TData&)>;
            using _TDatas = multimap<TTime, TData>;

            _TCallback _Callback;
            _TDatas _Datas;

        public:
            CTimers(_TCallback Callback_) :
                _Callback(Callback_)
            {}
            template<typename TDuration>
			inline void emplace(const TDuration& Delay_, const TData& Data_)
            {
                _Datas.emplace(system_clock::now() + Delay_, Data_);
            }
            void Proc(void)
            {
                auto Now = system_clock::now();
                auto it = _Datas.begin();
                for (; it != _Datas.end(); ++it)
                {
                    if (it->first > Now)
                        break;

                    _Callback(it->second);
                }
                _Datas.erase(_Datas.begin(), it);
            }
        };
    }
}
