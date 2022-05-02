#pragma once

#include <windows.h>
#include <sqlext.h>
#define SQL_ODBC_VERSION 110
#if SQL_ODBC_VERSION == 100
#include <Sql/100/sqlncli.h>
#pragma comment(lib, "Sql/100/sqlncli10.lib" )
#elif SQL_ODBC_VERSION == 110
#include <Sql/110/sqlncli.h>
#pragma comment(lib, "Sql/110/sqlncli11.lib" )
#else
#error invalid SQL_ODBC_VERSION
#endif
