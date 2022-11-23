#include "Latency.h"

namespace rso::gameutil
{
	CLatency::CLatency(const milliseconds& maxLatency) :
		_maxLatency(maxLatency)
	{
	}
	void CLatency::recv(const TTime& time, const TTime& remoteTime)
	{
		auto Duration = remoteTime - time;
		if (_offset < Duration)
			_offset = Duration;

		Duration = (time + _offset) - remoteTime;
		if (Duration > _maxLatency)
			_latency = _maxLatency;
		else if (_latency - Duration > _subDuration)
			_latency -= _subDuration;
		else
			_latency = Duration;
	}
}