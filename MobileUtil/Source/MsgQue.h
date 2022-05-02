#pragma once

#include "Base.h"

namespace rso
{
	namespace mobileutil
	{
		struct SMsg
		{
			string RID;
			string Title;
			string Msg;
		};

		class CMsgQue
		{
			using _TMsgs = list<SMsg>;

			_TMsgs _Msgs;
			size_t _MsgQueSize;
			bool _New = true;

		public:
			CMsgQue(size_t MsgQueSize_);
			void Push(const SMsg& Msg_);
			void Pop(void);
			const SMsg* Get(bool& New_); // New_ : 최초 Get된 메시지 인가? (PushAll 메시지 중에 _Users가 많아서 한번에 보내지 못하고 나눠서 보내는 경우 Json의 Title, Msg 를 또다시 할당하지 않기 위함)
		};
	}
}
