#pragma once

#include <Rso/Core/Core.h>
#include "Sql.h"

namespace rso
{
	namespace mssql
	{
		using namespace core;

		class CENV
		{
			SQLHENV _ENV = SQL_NULL_HENV;

		public:
			CENV();
			CENV(CENV&& Var_);
			virtual ~CENV();
			CENV& operator = (CENV&& Var_);
			SQLHENV ENV(void) const { return _ENV; }
		};
	}
}