#include "CoInitializer.h"

#pragma comment( lib, "Ole32.lib" )
#include <objbase.h>

namespace rso
{
	namespace win
	{
		CCoInitializer::CCoInitializer()
		{
			if ((_Result = CoInitialize(0)) != S_OK)
				THROWEX();
		}
		CCoInitializer::CCoInitializer(CCoInitializer&& Var_) :
			_Result(Var_._Result)
		{
			Var_._Result = S_FALSE;
		}
		CCoInitializer::~CCoInitializer()
		{
			if (_Result != S_FALSE)
				CoUninitialize();
		}
		CCoInitializer& CCoInitializer::operator = (CCoInitializer&& Var_)
		{
			this->~CCoInitializer();
			new (this) CCoInitializer(std::move(Var_));
			return *this;
		}
	}
}
