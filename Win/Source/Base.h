#pragma once

#include <Rso/Core/Core.h>

#pragma comment( lib, "Rso/Win_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace rso
{
	namespace win
	{
		using namespace core;

		inline string WStringToUTF8(const wstring& Src_)
		{
			string Dst;
			Dst.resize(WideCharToMultiByte(CP_UTF8, 0, Src_.c_str(), (int32)Src_.size(), NULL, 0, NULL, NULL));
			WideCharToMultiByte(CP_UTF8, 0, Src_.c_str(), (int32)Src_.size(), (LPSTR)Dst.data(), (int32)Dst.size(), NULL, NULL);
			return Dst;
		}
	}
}