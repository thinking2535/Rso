#include "ENV.h"

namespace rso
{
	namespace mssql
	{
		CENV::CENV()
		{
			if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_ENV)))
				THROWEX();

			if (!SQL_SUCCEEDED(SQLSetEnvAttr(_ENV, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_UINTEGER)))
			{
				SQLFreeHandle(SQL_HANDLE_ENV, _ENV);
				THROWEX();
			}
		}
		CENV::CENV(CENV&& Var_) :
			_ENV(Var_._ENV)
		{
			Var_._ENV = SQL_NULL_HENV;
		}
		CENV::~CENV()
		{
			SQLFreeHandle(SQL_HANDLE_ENV, _ENV);
		}
		CENV& CENV::operator = (CENV&& Var_)
		{
			this->~CENV();
			new (this) CENV(std::move(Var_));
			return *this;
		}
	}
}