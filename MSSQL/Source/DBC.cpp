#include "DBC.h"

namespace rso
{
	namespace mssql
	{
		CDBC::CDBC(SQLHENV ENV_, DWORD LoginTimeOut_, DWORD ConnTimeOut_)
		{
			if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, ENV_, &_DBC)))
				THROWEX();

			try
			{
				if (!SQL_SUCCEEDED(SQLSetConnectAttrW(_DBC, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER)LoginTimeOut_, 0)))
					THROWEX();

				if (!SQL_SUCCEEDED(SQLSetConnectAttrW(_DBC, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)ConnTimeOut_, 0)))
					THROWEX();
			}
			catch (...)
			{
				SQLFreeHandle(SQL_HANDLE_DBC, _DBC);
				throw;
			}
		}
		CDBC::CDBC(CDBC&& Var_) :
			_DBC(Var_._DBC)
		{
			Var_._DBC = SQL_NULL_HDBC;
		}
		CDBC::~CDBC()
		{
			SQLFreeHandle(SQL_HANDLE_DBC, _DBC);
		}
		CDBC& CDBC::operator = (CDBC&& Var_)
		{
			this->~CDBC();
			new (this) CDBC(std::move(Var_));
			return *this;
		}
	}
}