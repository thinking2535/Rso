#pragma once

#include <Rso/Net/Base.h>

#pragma comment( lib, "Rso/Game_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include "Protocol.h"

namespace rso::game
{
	using namespace net;

	class exception_game : public exception
	{
		EGameRet _Ret = EGameRet::Null;

	public:
		exception_game() noexcept
		{
		}
		explicit exception_game(EGameRet Ret_) noexcept
			: exception(StringFormat("exception_game[%d]", (int)Ret_).c_str())
		{
		}
		exception_game(EGameRet Ret_, int _Int) noexcept
			: exception(StringFormat("exception_game[%d]", (int)Ret_).c_str(), _Int)
		{
		}
		exception_game(exception const& _Other) noexcept
			: exception(_Other)
		{
		}
		inline EGameRet GetRet(void) const noexcept { return _Ret; }
	};

	class CUIDPair : public SUIDPair
	{
	public:
		CUIDPair()
		{
		}
		CUIDPair(const SUIDPair& UIDPair_) :
			SUIDPair(UIDPair_)
		{
		}
		CUIDPair(TUID UID_, TUID SubUID_) :
			SUIDPair(UID_, SubUID_)
		{
		}
		explicit operator bool() const
		{
			return (UID > 0 && SubUID > 0);
		}
		bool operator == (const CUIDPair& UIDPair_) const
		{
			return (UID == UIDPair_.UID && SubUID == UIDPair_.SubUID);
		}
		bool operator != (const CUIDPair& UIDPair_) const
		{
			return !(*this == UIDPair_);
		}
		bool operator < (const CUIDPair& UIDPair_) const
		{
			if (UID < UIDPair_.UID)
				return true;
			else if (UID > UIDPair_.UID)
				return false;
			else if (SubUID < UIDPair_.SubUID)
				return true;
			else
				return false;
		}
		void Clear(void)
		{
			UID = 0;
			SubUID = 0;
		}
	};
	inline bool operator < (const SServerNet& lhs_, const SServerNet& rhs_)
	{
		return lhs_.ServerBindNamePort < rhs_.ServerBindNamePort;
	}
}
