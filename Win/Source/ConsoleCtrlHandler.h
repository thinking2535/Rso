#pragma once

#include "Base.h"
#include <Windows.h>
#include <thread>
#include <chrono>
#include <functional>

namespace rso
{
	namespace win
	{
		using namespace std;
		using namespace chrono;
		using namespace placeholders;

		class CConsoleCtrlHandler
		{
			static volatile bool _Exit;

			static BOOL _Handler(DWORD CtrlType_);

		public:
			CConsoleCtrlHandler();
			inline volatile bool GetExit(void) const
			{
				return _Exit;
			}
			inline void SetExit(void) // for exiting loop
			{
				_Exit = true;
			}
		};
	}
}
