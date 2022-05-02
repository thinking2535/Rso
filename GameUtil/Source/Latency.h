#pragma once

// 일정하지 않은 Latency 를 일정하도록

#include "Base.h"

namespace rso::gameutil
{
	using namespace core;

	class CLatency
	{
		const t_duration _MaxLatency;
		const t_duration _SubDuration = t_duration(100000);
		t_duration _Offset{ MinValue<int64>() };
		t_duration _Latency{};

	public:
		CLatency(const milliseconds& MaxLateancy_);
		void Recv(const TTime& Time_, const TTime& RemoteTime_);
		inline bool Proc(const TTime& Time_, const TTime& RemoteTime_) const { return Time_ + _Offset - _Latency >= RemoteTime_; }
		inline t_duration GetOffset(void) const { return _Offset; }
		inline t_duration GetLatency(void) const { return _Latency; }
	};
}