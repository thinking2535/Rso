#include "Base.h"

namespace rso
{
	namespace win
	{
		string WStringToUTF8(const wstring& Src_)
		{
			string Dst;
			Dst.resize(WideCharToMultiByte(CP_UTF8, 0, Src_.c_str(), (int)Src_.size(), NULL, 0, NULL, NULL));
			WideCharToMultiByte(CP_UTF8, 0, Src_.c_str(), (int)Src_.size(), (LPSTR)Dst.data(), (int)Dst.size(), NULL, NULL);
			return Dst;
		}
	}
}
