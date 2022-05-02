#include "ConsoleCtrlHandler.h"

namespace rso
{
	namespace win
	{
		volatile bool CConsoleCtrlHandler::_Exit = false;

		BOOL CConsoleCtrlHandler::_Handler(DWORD /*CtrlType_*/)
		{
			//switch (CtrlType_)
			//{
			//case CTRL_C_EVENT:
			//case CTRL_BREAK_EVENT:
			//	break;

			//case CTRL_CLOSE_EVENT:
			//case CTRL_LOGOFF_EVENT:
			//case CTRL_SHUTDOWN_EVENT:
			//	_Exit = true;

			//	return TRUE;
			//}

			_Exit = true;
			return TRUE;
		}
		CConsoleCtrlHandler::CConsoleCtrlHandler()
		{
			EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED); // Disable Close Button

			if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)_Handler, TRUE) == FALSE)
				THROWEX();
		}
	}
}