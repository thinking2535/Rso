#pragma once


#include "Base.h"
#include <map>
#include <string>
#include <chrono>
#include <functional>
#include "Protocol.h"


namespace rso
{
	namespace util
	{
		using namespace std;
		using namespace chrono;
		using namespace rso;
		using namespace base;

		class CReminder
		{
		public:
			using TIntervals = vector<minutes>;

		private:
			using TMemories = multimap<TTime, SData>;
			using TMastered = vector<SData>;
			using TCallback = function<bool(const SData&)>;

			TIntervals _Intervals;
			TMemories _Memories;
			TMastered _Mastered;
			TCallback _Callback;

		public:
			CReminder(TIntervals&& Intervals_, TCallback Callback_, CStream& Memories_, CStream& Mastered_);

			void Push(const wstring& Key_, const wstring& Value_);
			bool Proc(void); // true : Callback function was Called
			CStream MemoriesStream(void) const;
			CStream MasteredStream(void) const;
		};
	}
}