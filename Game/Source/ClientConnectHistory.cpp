#include "ClientConnectHistory.h"

namespace rso::game
{
	bool CClientConnectHistory::Connect(const CNamePort& NamePort_)
	{
		try
		{
			return _Datas.emplace(NamePort_).second;
		}
		catch (...)
		{
			return false;
		}
	}
	void CClientConnectHistory::Clear(void)
	{
		_Datas.clear();
	}
}