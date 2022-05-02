#include "Input.h"


namespace rso
{
	namespace util
	{
		CInput::CInput() :
			_hIn(INVALID_HANDLE_VALUE)
		{
			ZeroMemory(&_InRecord, sizeof(_InRecord));

			_hIn = GetStdHandle(STD_INPUT_HANDLE);
			if (_hIn == INVALID_HANDLE_VALUE)
				THROWEX();
		}
		INPUT_RECORD*	CInput::Get(void)
		{
			DWORD		_EventCount = 0;
			DWORD		_NumRecordsRead = 0;
			GetNumberOfConsoleInputEvents(_hIn, &_EventCount);
			if(_EventCount > 0)
			{
				if (ReadConsoleInput(_hIn, &_InRecord, 1, &_NumRecordsRead) && _NumRecordsRead == 1)
				{
					if (_InRecord.EventType == KEY_EVENT && _InRecord.Event.KeyEvent.bKeyDown)
						return &_InRecord;
				}
			}

			return nullptr;
		}
	}
}
