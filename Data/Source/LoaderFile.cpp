#include "LoaderFile.h"


namespace rso
{
    namespace data
    {
        CLoaderFile::CLoaderFile(const wstring& FileName_) :
            _FileName(FileName_)
        {
        }
        void CLoaderFile::Get(CStream& Stream_o)
        {
            Stream_o.LoadFile(_FileName);
        }
        void CLoaderFile::Get(SProto& Proto_)
        {
            CStream Stream;

            Get(Stream);
            Stream >> Proto_;

            if (Stream.size() > 0)
                THROWEX();
        }
    }
}
