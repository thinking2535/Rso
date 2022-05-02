#include "Messenger.h"


namespace rso
{
	namespace messenger
	{
		void		CMessenger::_Worker(void)
		{

		}

		CMessenger::CMessenger(TCallbackAdd CallbackAdd_, TCallbackDel CallbackDel_, TCallbackMsg CallbackMsg_) :
			_CallbackAdd(CallbackAdd_), _CallbackDel(CallbackDel_), _CallbackMsg(CallbackMsg_)
		{
			_WorkerThread = new CThread(std::bind(&CMessenger::_Worker, this));
		}

		CMessenger::~CMessenger()
		{
			if (!_WorkerThread)
			{
				delete _WorkerThread;
				_WorkerThread = 0;
			}
		}

		void	CMessenger::Proc(void)
		{
		}
	}
}
