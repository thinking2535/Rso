#include "LoaderHttp.h"


namespace rso
{
    namespace data
    {
		void CLoaderHttp::_OutFunc(size_t /*CmdIndex_*/, EHttpRet Ret_, CStream& Stream_)
		{
			if (Ret_ != EHttpRet::Ok)
			{
				_Stream = nullptr;
				THROWEX();
			}

			*_Stream = std::move(Stream_);
			_Stream = nullptr;

			if (Stream_.size() > 0)
				THROWEX();
		}
		CLoaderHttp::CLoaderHttp(const string& Server_, const string& Obj_, const string& Param_, THttpPort Port_) :
            _Server(Server_),
            _Obj(Obj_),
            _Param(Param_),
            _Port(Port_),
            _Http(1, std::bind(&CLoaderHttp::_OutFunc, this, _1, _2, _3))
        {
        }
		void CLoaderHttp::Get(CStream& Stream_)
		{
			_Stream = &Stream_;
			_Http.Push(0, SInObj("", _Server, _Port, "", "", EMethod::Get, _Obj, _Param, false));

			while (_Stream)
			{
				_Http.Proc();
				this_thread::sleep_for(milliseconds(10));
			}
		}
		void CLoaderHttp::Get(SProto& Proto_)
        {
			CStream Stream;
			Get(Stream);
			Stream >> Proto_;
			if (Stream.size() > 0)
				THROWEX();
        }
    }
}
