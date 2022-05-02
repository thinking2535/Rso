#pragma once

#include "Base.h"

namespace rso::gameutil
{
	inline bool IsInBoost(const STimeBoost& Boost_, const TTime& Now_)
	{
		return (Now_ < Boost_.EndTime);
	}
	inline t_duration GetRealDuration(const t_duration& BoostedDuration_, double Speed_)
	{
		return t_duration(int64(BoostedDuration_.count() / Speed_));
	}
	inline t_duration GetBoostedDuration(const STimeBoost& Boost_, const TTime& Now_)
	{
		return t_duration(int64((Boost_.EndTime - Now_).count() * Boost_.Speed));
	}
	t_duration GetBoostedDuration(const STimeBoost& Boost_, const TTime& BeginTime_, const TTime& EndTime_);
	TTime GetBoostedEndTime(const STimeBoost& Boost_, const TTime& BeginTime_, const t_duration& BoostedDuration_);
	bool ChangeBeginTime(const STimeBoost& Boost_, const TTime& Now_, double Speed_, TTime& BeginTime_); // return true : º¯°æµÊ
}
