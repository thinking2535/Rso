#pragma once

#include <Rso/Core/Core.h>
#include "Sql.h"

namespace rso
{
	namespace mssql
	{
		using namespace core;

		class CSTMT
		{
			SQLHSTMT _STMT = SQL_NULL_HSTMT;

		public:
			CSTMT(SQLHDBC DBC_, DWORD QueryTimeOut_);
			CSTMT(CSTMT&& Var_);
			virtual ~CSTMT();
			CSTMT& operator = (CSTMT&& Var_);
			SQLHSTMT STMT(void) const { return _STMT; }
		};
	}
}