#include "Net.h"

namespace rso::mobile
{
	CNet::CNet(TLinkFunc LinkFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_, const milliseconds& KeepConnectDuration_) :
		_KeepConnectDuration(KeepConnectDuration_),
		_LinkFunc(LinkFunc_),
		_UnLinkFunc(UnLinkFunc_),
		_RecvFunc(RecvFunc_)
	{
	}
}
