#include "BinderServer.h"

namespace rso::net
{
	CBinderServer::CBinderServer(CBinder&& Binder_, int BackLog_) :
		CBinder(std::move(Binder_))
	{
		if (listen(Socket(), BackLog_) == SOCKET_ERROR)
			THROWEX();
	}
}
