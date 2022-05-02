#pragma once

// Add(TCode PackageCode_, TCode Code_) 의 PackageCode_ 와 Code_ 가 같을 경우 단일항목 Package 로 간주

#include "Base.h"


namespace rso
{
	namespace gameutil
	{
		class CPackage
		{
		public:
			using TPackage = set<TCode>;

		private:
			using _TDatas = map<TCode, TPackage>;

			_TDatas _Datas;

		public:
			bool Add(TCode PackageCode_, TCode Code_);
			const TPackage* Get(TCode PackageCode_) const;
		};
	}
}