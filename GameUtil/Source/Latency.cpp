#include "Latency.h"

namespace rso::gameutil
{
	CLatency::CLatency(const milliseconds& MaxLatency_) :
		_MaxLatency(MaxLatency_)
	{
	}
	void CLatency::Recv(const TTime& Time_, const TTime& RemoteTime_)
	{
		auto Duration = RemoteTime_ - Time_;
		if (_Offset < Duration)
			_Offset = Duration;

		Duration = (Time_ + _Offset) - RemoteTime_;
		if (Duration > _MaxLatency)
			_Latency = _MaxLatency;
		else if (_Latency - Duration > _SubDuration)
			_Latency -= _SubDuration;
		else
			_Latency = Duration;
	}
}