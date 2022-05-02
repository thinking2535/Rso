#pragma once


#include "Loader.h"
#include <Rso/Http/Http.h>


namespace rso
{
    namespace data
    {
        using namespace http;

        class CLoaderHttp : public ILoader
        {
			string _Server;
			string _Obj;
			string _Param;
			THttpPort _Port;
			CHttp _Http;
			CStream* _Stream = nullptr;

			void _OutFunc(size_t CmdIndex_, EHttpRet Ret_, CStream& Stream_);

		public:
            CLoaderHttp(const string& Server_, const string& Obj_, const string& Param_, THttpPort Port_);
			void Get(CStream& Stream_);
            void Get(SProto& Proto_);
        };
    }
}
