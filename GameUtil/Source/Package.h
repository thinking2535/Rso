#pragma once

// Add(TCode PackageCode_, TCode Code_) �� PackageCode_ �� Code_ �� ���� ��� �����׸� Package �� ����

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