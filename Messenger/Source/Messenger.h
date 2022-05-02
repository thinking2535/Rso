#pragma once


#include "Base.h"
#include <Rso/Base/Thread.h>


#pragma comment( lib, "Rso/Messenger_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )


namespace rso
{
	namespace messenger
	{
		using namespace base;

		class CMessenger sealed
		{
		private:
			TCallbackAdd	_CallbackAdd;
			TCallbackDel	_CallbackDel;
			TCallbackMsg	_CallbackMsg;
			CThread*	_WorkerThread{ 0 };

			void		_Worker(void);

		public:
			CMessenger(TCallbackAdd CallbackAdd_, TCallbackDel CallbackDel_, TCallbackMsg CallbackMsg_);
			~CMessenger();

			ERet		Add(const SFriend& Friend_);
			void		Del(const wstring& ID_);
			bool		Send(const wstring& Msg_);

			void	Proc(void);
		};
	}
}
