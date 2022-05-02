#include "IOCP.h"

namespace rso::net
{
	CIOCP::CIOCP(size_t ThreadCnt_) :
		_ThreadCnt(ThreadCnt_ > 0 ? ThreadCnt_ : THROWEX())
	{
		_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if (_IOCP == NULL)
			THROWEX();
	}
	CIOCP::CIOCP(CIOCP&& Var_) :
		_ThreadCnt(Var_._ThreadCnt),
		_IOCP(Var_._IOCP)
	{
		Var_._ThreadCnt = 0;
		Var_._IOCP = NULL;
	}
	CIOCP::~CIOCP()
	{
		for (size_t i = 0; i < _ThreadCnt; ++i)
			PostQueuedCompletionStatus(_IOCP, 0, NULL, NULL);
	}
	CIOCP& CIOCP::operator = (CIOCP&& Var_)
	{
		this->~CIOCP();
		new (this) CIOCP(std::move(Var_));
		return *this;
	}
}