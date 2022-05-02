#include "TimeSync.h"

namespace rso::gameutil
{
	CTimeSync::CTimeSync(const milliseconds& Error_) :
		_Error(Error_)
	{
	}
	bool CTimeSync::Sync(const TTime& RemoteTime_)
	{
		if (RemoteTime_ > (steady_now() + _Error) || RemoteTime_ < _RemoteTime)
			return false;

		_RemoteTime = RemoteTime_;

		return true;
	}
}