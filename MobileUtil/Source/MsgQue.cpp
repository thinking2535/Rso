#include "MsgQue.h"

namespace rso
{
	namespace mobileutil
	{
		CMsgQue::CMsgQue(size_t MsgQueSize_) :
			_MsgQueSize(MsgQueSize_ <= 0 ? MaxValue<decltype(MsgQueSize_)>() : MsgQueSize_)
		{
		}
		void CMsgQue::Push(const SMsg& Msg_)
		{
			if (_Msgs.size() >= _MsgQueSize)
				return;

			_Msgs.emplace_back(Msg_);
		}
		void CMsgQue::Pop(void)
		{
			_Msgs.pop_front();
			_New = true;
		}
		const SMsg* CMsgQue::Get(bool& New_)
		{
			if (_Msgs.empty())
				return nullptr;

			New_ = _New;
			_New = false;

			return &_Msgs.front();
		}
	}
}