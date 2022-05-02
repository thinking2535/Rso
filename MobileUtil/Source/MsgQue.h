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
			const SMsg* Get(bool& New_); // New_ : ���� Get�� �޽��� �ΰ�? (PushAll �޽��� �߿� _Users�� ���Ƽ� �ѹ��� ������ ���ϰ� ������ ������ ��� Json�� Title, Msg �� �Ǵٽ� �Ҵ����� �ʱ� ����)
		};
	}
}
