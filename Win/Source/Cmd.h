#pragma once

#include "Handler.h"
#include <Rso/Core/Core.h>
#include <Rso/Core/Stream.h>
#include <Rso/Base/LFQueue.h>
#include <functional>
#include <Rso/Base/Thread.h>

namespace rso
{
	namespace win
	{
		using namespace core;
		using namespace base;
		using namespace std;

		class CCmd
		{
		public:
			using TOutCallback = function<void(const string&)>;
			struct SInObject
			{
				wstring Cmd;
				wstring WorkDir;

				void clear(void)
				{
					Cmd.clear();
					WorkDir.clear();
				}
			};

		private:
			TOutCallback _OutCallback;
			CHandler _CmdStdOutRdHandle;
			CHandler _CmdStdOutWrHandle;
			CLFQueueB<SInObject> _InQueue;
			CLFQueueB<string> _OutQueue;
			CThread _WorkerReadThread;
			CThread _WorkerInputThread;

			void _WorkerRead(const volatile bool* Exit_);
			void _WorkerInput(const volatile bool* Exit_);

		public:
			CCmd(TOutCallback OutCallback_);
			void Input(const SInObject& Object_);
			void Proc(void);
		};
	}
}
