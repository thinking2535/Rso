#include "STMT.h"

namespace rso
{
	namespace mssql
	{
		CSTMT::CSTMT(SQLHDBC DBC_, DWORD QueryTimeOut_)
		{
			if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, DBC_, &_STMT)))
				THROWEX();

			if (!SQL_SUCCEEDED(SQLSetStmtAttrW(_STMT, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)&QueryTimeOut_, SQL_IS_UINTEGER)))
			{
				SQLFreeHandle(SQL_HANDLE_STMT, _STMT);
				THROWEX();
			}
		}
		CSTMT::CSTMT(CSTMT&& Var_) :
			_STMT(Var_._STMT)
		{
			Var_._STMT = SQL_NULL_HSTMT;
		}
		CSTMT::~CSTMT()
		{
			SQLFreeHandle(SQL_HANDLE_STMT, _STMT);
		}
		CSTMT& CSTMT::operator = (CSTMT&& Var_)
		{
			this->~CSTMT();
			new (this) CSTMT(std::move(Var_));
			return *this;
		}
	}
}