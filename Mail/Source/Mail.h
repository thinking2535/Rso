#pragma once

#include <Rso/Core/Core.h>
#pragma comment( lib, "Rso/Mail_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#import "Rso/msado15.dll" rename("EOF", "AdoEOF"), no_namespace
#import "Rso/cdoex.dll" no_namespace

#include "Rso/Base/Base.h"

namespace rso
{
	namespace mail
	{
		using namespace core;

		class CMail
		{
		public:
			typedef		string	TAddress;
			typedef		string	TTitle;
			typedef		string	TBody;

		private:

		public:
			CMail();
			~CMail();

			void	Send(TAddress From_, TAddress To_, TTitle Title_, TBody Body_);
		};
	}
}
