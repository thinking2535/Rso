#pragma once


#include "Loader.h"


namespace rso
{
    namespace data
    {
        class CLoaderFile : public ILoader
        {
        public:
            CLoaderFile(const wstring& FileName_);
            void Get(CStream& Stream_o);
            void Get(SProto& Proto_);

        private:
            wstring _FileName;
        };
    }
}
