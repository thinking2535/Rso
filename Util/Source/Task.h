#pragma once


#include <Rso/Base/Period.h>
#include <map>
#include <chrono>

namespace rso
{
	namespace util
	{
		using namespace std;
		using namespace chrono;
		using namespace rso;
		using namespace base;

		class CTask
		{
		public:
			using TTaskCnt = int64;

		private:
			struct _STaskInfo
			{
				int32 TaskCnt = 0;
				system_clock::time_point LastSetTime;

				_STaskInfo() :
					TaskCnt(1), LastSetTime(system_clock::now())
				{
				}
			};
			using _TTasks = map<TTaskCnt, _STaskInfo>;

			_TTasks _Tasks;
			CPeriod<milliseconds> _Period{ milliseconds(3000) };

		public:
			bool PushTask(TTaskCnt PeerNum_);
			void PopTask(TTaskCnt PeerNum_);
			bool empty(void) const;
		};
	}
}