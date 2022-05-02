#include "Base.h"

namespace rso::gameutil
{
	t_duration GetBoostedDuration(const STimeBoost& Boost_, const TTime& BeginTime_, const TTime& EndTime_)
	{
		if (EndTime_ <= BeginTime_)
			return t_duration(0);

		if (Boost_.EndTime < BeginTime_)
		{
			return EndTime_ - BeginTime_;
		}
		else
		{
			if (Boost_.EndTime < EndTime_)
				return t_duration(__int64((Boost_.EndTime - BeginTime_).count() * Boost_.Speed) + (EndTime_ - Boost_.EndTime).count());
			else
				return t_duration(__int64((EndTime_ - BeginTime_).count() * Boost_.Speed));
		}
	}
	TTime GetBoostedEndTime(const STimeBoost& Boost_, const TTime& BeginTime_, const t_duration& BoostedDuration_)
	{
		if (Boost_.EndTime < BeginTime_)
		{
			return BeginTime_ + BoostedDuration_;
		}
		else
		{
			auto BoostDuration = t_duration(__int64((Boost_.EndTime - BeginTime_).count() * Boost_.Speed));
			if (BoostedDuration_ <= BoostDuration)
				return BeginTime_ + t_duration(__int64(BoostedDuration_.count() / Boost_.Speed));
			else
				return Boost_.EndTime + (BoostedDuration_ - BoostDuration);
		}
	}
	bool ChangeBeginTime(const STimeBoost& Boost_, const TTime& Now_, double Speed_, TTime& BeginTime_)
	{
		if (IsInBoost(Boost_, Now_))
			return false;

		if (BeginTime_ <= Boost_.EndTime)
			BeginTime_ = Now_ - (Boost_.EndTime - BeginTime_) - GetRealDuration(Now_ - Boost_.EndTime, Speed_);
		else
			BeginTime_ = Now_ - GetRealDuration(Now_ - BeginTime_, Speed_);

		return true;
	}
	wstring GetRandomName(size_t MinLength_, size_t MaxLength_)
	{
		static const wchar_t Str[] =
			L"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		auto Length = MinLength_ + (rand() % (MaxLength_ - MinLength_)) + 1;

		wstring Nick(Length, 0);

		for (int i = 0; i < Nick.size(); ++i)
			Nick[i] = Str[rand() % ((sizeof(Str) / sizeof(wchar_t)) - 1)];

		return Nick;
	}
}
