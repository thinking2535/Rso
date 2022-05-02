#pragma once


#include "Base.h"


namespace rso
{
    namespace data
    {
        __interface ILoader
        {
            void Get(CStream& Stream_o);
            void Get(SProto& Proto_);
        };
    }
}
