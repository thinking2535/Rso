#include "Reminder.h"


namespace rso
{
	namespace util
	{
		CReminder::CReminder(TIntervals&& Intervals_, TCallback Callback_, CStream& Memories_, CStream& Mastered_) :
			_Intervals(std::move(Intervals_)), _Callback(Callback_)
		{
			if (_Intervals.size() == 0)
				throw("Invalid Interval Size");

			try
			{
				SDataTime DataTime;
				while (true)
				{
					Memories_ >> DataTime;
					_Memories.emplace(system_clock::time_point(system_clock::duration(DataTime.TimePoint)), DataTime);
				}
			}
			catch (...)
			{
			}

			try
			{
				SDataTime Data;
				while (true)
				{
					Mastered_ >> Data;
					_Mastered.emplace_back(Data);
				}
			}
			catch (...)
			{
			}
		}

		void CReminder::Push(const wstring& Key_, const wstring& Value_)
		{
			_Memories.emplace(system_clock::now() + _Intervals[0], SData(Key_, Value_, 0, 0));
		}
		bool CReminder::Proc(void)
		{
			auto Now = system_clock::now();
			auto itBegin = _Memories.begin();
			if (itBegin == _Memories.end())
				return false;

			if (itBegin->first > Now)
				return false;

			if (_Callback(itBegin->second))
			{
				++itBegin->second.RemindCount;
				++itBegin->second.Grade;

				if (itBegin->second.Grade >= (int32)_Intervals.size())
					_Mastered.emplace_back(std::move(itBegin->second));
				else
					_Memories.emplace(Now + _Intervals[itBegin->second.Grade], std::move(itBegin->second));
			}
			else
			{
				++itBegin->second.RemindCount;
				_Memories.emplace(Now + _Intervals[itBegin->second.Grade], std::move(itBegin->second));
			}

			_Memories.erase(itBegin);

			return true;
		}
		// delete me 한번에 모두 콜백 호출하는 방식 에서 매번 한건만 콜백하는 방식으로 수정(외부에서 중간에 exit 등 의 처리를 하기 위해)
		//void CReminder::Proc(void)
		//{
		// auto Now = system_clock::now();
		// auto itTo = _Memories.begin();
		// for (; itTo != _Memories.end(); ++itTo)
		// {
		// if (itTo->first > Now)
		// break;

		// if (_Callback(itTo->second))
		// {
		// ++itTo->second.Grade;

		// if (itTo->second.Grade >= _Intervals.size())
		// _Mastered.emplace_back(std::move(itTo->second));
		// else
		// _Memories.emplace(Now + _Intervals[itTo->second.Grade], std::move(itTo->second));
		// }
		// else
		// {
		// _Memories.emplace(Now + _Intervals[itTo->second.Grade], std::move(itTo->second));
		// }

		// ++itTo->second.RemindCount;
		// }

		// _Memories.erase(_Memories.begin(), itTo);
		//}
		CStream CReminder::MemoriesStream(void) const
		{
			CStream Stream;

			for (auto& Mem : _Memories)
				Stream << SDataTime(Mem.second, Mem.first.time_since_epoch().count());

			return Stream;
		}
		CStream CReminder::MasteredStream(void) const
		{
			CStream Stream;

			for (auto& Mem : _Mastered)
				Stream << Mem;

			return Stream;
		}
	}
}