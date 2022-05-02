#pragma once

#include "Base.h"
#include <Rso/Base/MultiIndexMap.h>
#include <Rso/Net/Protocol.h>

namespace rso::ranking
{
	using namespace base;
	using namespace net;

	template<typename _TValue>
	class CRanking : public CMultiIndexMap<_TValue, unique_index<TUID>, non_unique_index<__int32, greater<__int32>>>
	{
	public:
		using TValue = _TValue;

	private:
		wstring _SPName;
		__int32 _MaxCount = 0;

	public:
		CRanking(const wstring& Name_) :
			_SPName(L"dbo.sys_sp_" + Name_)
		{
		}
		inline const wstring& GetSPName(void) const { return _SPName; }
	};
}