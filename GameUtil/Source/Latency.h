#pragma once

// 일정하지 않은 Latency 를 일정하도록

#include "Base.h"

namespace rso::gameutil
{
	using namespace core;

	class CLatency
	{
		const t_duration _maxLatency;
		const t_duration _subDuration = t_duration(100000);
		t_duration _offset{ MinValue<int64>() };
		t_duration _latency{};

	public:
		CLatency(const milliseconds& maxLateancy);
		void recv(const TTime& time, const TTime& remoteTime);
		inline bool proc(const TTime& time, const TTime& remoteTime) const { return time + _offset - _latency >= remoteTime; }
		inline t_duration getOffset(void) const { return _offset; }
		inline t_duration getLatency(void) const { return _latency; }
	};
}