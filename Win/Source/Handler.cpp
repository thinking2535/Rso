#include "Handler.h"

namespace rso
{
	namespace win
	{
		CHandler::CHandler()
		{
		}
		CHandler::CHandler(CHandler&& Var_) :
			_Handle(Var_._Handle)
		{
			Var_._Handle = FALSE;
		}
		CHandler::~CHandler()
		{
			if (_Handle != NULL)
				CloseHandle(_Handle);
		}
		CHandler& CHandler::operator = (CHandler&& Var_)
		{
			this->~CHandler();
			new (this) CHandler(std::move(Var_));
			return *this;
		}
	}
}