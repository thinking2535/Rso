#pragma once


#include "Base.h"
#include <Rso/Win/Base.h>
#include <functional>


namespace rso
{
	namespace util
	{
		using namespace base;
		using namespace win;

		class CInput
		{
		private:
			HANDLE		_hIn;
			INPUT_RECORD _InRecord;

		public:
			CInput();
			INPUT_RECORD*	Get(void);
		};
	}
}
