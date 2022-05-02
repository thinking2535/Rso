#pragma once

#include <map>
#include <chrono>
#include <filesystem>
#include "Protocol.h"
#include <Rso/Base/Thread.h>
#include <Rso/Base/Period.h>
#include <Rso/Net/Base.h>

#pragma comment( lib, "Rso/Monitor_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

namespace rso::monitor
{
	using namespace std;
	using namespace stdext;
	using namespace chrono;
	using namespace rso;
	using namespace base;
	using namespace net;

	using TCallbackUserProto = function<void(const CKey&, CStream&)>;

	struct SAgentValue
	{
		wstring Name;
		SAgentOption Option;
		Value Stat;

		SAgentValue(const SAgent& Agent_) :
			Name(Agent_.Name),
			Option(Agent_.Option)
		{
			if (!Reader().parse(Agent_.Stat, Stat))
				THROWEX();
		}
		SAgent GetSAgent(void) const
		{
			return SAgent(Name, Option, Stat.toStyledWString());
		}
	};
	struct SProcValue
	{
		wstring Name;
		Value Stat;

		SProcValue()
		{
		}
		SProcValue(const SProc& Proc_) :
			Name(Proc_.Name)
		{
			if (!Reader().parse(Proc_.Stat, Stat))
				THROWEX();
		}
		SProc GetSProc(void) const
		{
			return SProc(Name, Stat.toStyledWString());
		}
		void clear(void)
		{
			Name.clear();
			Stat.clear();
		}
	};
}
