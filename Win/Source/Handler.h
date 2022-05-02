#pragma once

#include "Base.h"

namespace rso
{
	namespace win
	{
		class CHandler
		{
			HANDLE _Handle = NULL;

		public:
			CHandler();
			CHandler(CHandler&& Var_);
			virtual ~CHandler();
			CHandler& operator = (CHandler&& Var_);
			HANDLE& Handle(void)
			{
				return _Handle;
			}
			const HANDLE& Handle(void) const
			{
				return _Handle;
			}
		};
	}
}