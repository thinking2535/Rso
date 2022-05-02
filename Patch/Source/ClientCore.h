#pragma once

#include "Base.h"
#include <Rso/Http/Http.h>
#include <queue>

namespace rso
{
	namespace patch
	{
		using namespace http;
		using namespace std;

		class CClientCore
		{
		protected:
			COptionStream<SPatchData> _Data;
			path _DataPathFull;

			void _PatchCore(const SPatchData& Data_);

		public:
			CClientCore(const wstring& FileName_, const string& DataPath_);
			inline const SPatchData& GetData(void) const
			{
				return (*_Data);
			}
			inline path GetDataPathFull(void) const
			{
				return _DataPathFull;
			}
		};
	}
}
