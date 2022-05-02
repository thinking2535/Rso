#include "Lock.h"

namespace rso
{
	namespace win
	{
		void	CLock2::Lock(void)
		{
			while (++_Counter != 1)
			{
				--_Counter;
				this_thread::sleep_for(_SpinTime);
			}
		}

		bool	CLock2::LockTry(void)
		{
			if (++_Counter == 1)
				return true;

			--_Counter;

			return false;
		}

		void	CLock2::UnLock(void)
		{
			--_Counter;
		}
	}
}
