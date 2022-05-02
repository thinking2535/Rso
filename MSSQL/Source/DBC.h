#pragma once

#include <Rso/Core/Core.h>
#include "Sql.h"

namespace rso
{
	namespace mssql
	{
		using namespace core;

		class CDBC
		{
			SQLHDBC _DBC = SQL_NULL_HDBC;

		public:
			CDBC(SQLHENV ENV_, DWORD LoginTimeOut_, DWORD ConnTimeOut_);
			CDBC(CDBC&& Var_);
			virtual ~CDBC();
			CDBC& operator = (CDBC&& Var_);
			SQLHDBC DBC(void) const { return _DBC; }
		};
	}
}