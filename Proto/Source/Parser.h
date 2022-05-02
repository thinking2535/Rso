#pragma once


#include "Base.h"
#include <string>


namespace rso
{
    namespace proto
    {
        using namespace std;

        class CParser
        {
        private:
            wstring _Str;
            wstring::size_type _Index;

        public:
            CParser(const wstring& Str_) :
                _Str(Str_), _Index(0) {}

            wstring Get(void);
        };
    }
}
