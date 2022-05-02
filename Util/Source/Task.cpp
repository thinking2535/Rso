#include "Task.h"


namespace rso
{
	namespace util
	{
		bool CTask::PushTask(TTaskCnt PeerNum_)
		{
			try
			{
				auto ib = _Tasks.insert(_TTasks::value_type(PeerNum_, _STaskInfo()));
				if (!ib.second)
				{
					ib.first->second.LastSetTime = system_clock::now();
					if (ib.first->second.TaskCnt + 1 > ib.first->second.TaskCnt)
						++(ib.first->second.TaskCnt);
				}
			}
			catch (...)
			{
				return false;
			}

			return true;
		}
		void CTask::PopTask(TTaskCnt PeerNum_)
		{
			auto it = _Tasks.find(PeerNum_);
			if (it == _Tasks.end())
				return;

			if (it->second.TaskCnt > 0)
			{
				--(it->second.TaskCnt);
				it->second.LastSetTime = system_clock::now();
			}

			if (it->second.TaskCnt == 0)
			{
				_Tasks.erase(it);
			}
		}
		bool CTask::empty(void) const
		{
			return (_Tasks.size() == 0);
		}
	}
}