#pragma once

#include <Rso/Net/Base.h>

namespace rso::game
{
	using namespace net;

	class CClientConnectHistory
	{
		set<CNamePort> _Datas;

	public:
		bool Connect(const CNamePort& NamePort_);
		void Clear(void);
	};
}