#pragma once

#include <Rso/Core/Base.h>

#pragma comment( lib, "Rso/GameUtil_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include "Protocol.h"

namespace rso::gameutil
{
	template<typename _TType>
	_TType GetCeilValue(_TType OutRange_, _TType InRange_, _TType InCur_) // return : OutCur
	{
		return OutRange_ - (OutRange_ * (InRange_ - InCur_) / InRange_);
	}
	template<typename _TType>
	_TType GetFromStream(CStream& Stream_) // not pop
	{
		_TType Type;

		Stream_.SaveState();
		Stream_ >> Type;
		Stream_.LoadState();

		return Type;
	}
	template<typename _TProto>
	void LoadProto(const wstring& FileName_, _TProto& Proto_)
	{
		CStream Stream(FileName_);
		Stream >> Proto_;
	}
	template<typename _TProto>
	void SaveProto(const _TProto& Proto_, const wstring& FileName_)
	{
		CStream Stream;
		Stream << Proto_;
		Stream.SaveFile(FileName_);
	}
	wstring GetRandomName(size_t MinLength_, size_t MaxLength_);
}
