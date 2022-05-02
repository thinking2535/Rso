#include "Package.h"


namespace rso
{
	namespace gameutil
	{
		bool CPackage::Add(TCode PackageCode_, TCode Code_)
		{
			if (PackageCode_ == Code_)
			{
				return _Datas.emplace(PackageCode_, TPackage()).second;
			}
			else
			{
				if (_Datas.find(Code_) == _Datas.end())
					return false;

				auto it = _Datas.find(PackageCode_);
				if (it == _Datas.end())
				{
					return _Datas.emplace(PackageCode_, TPackage()).first->second.emplace(Code_).second;
				}
				else
				{
					if (it->second.empty())
						return false;

					return it->second.emplace(Code_).second;
				}
			}
		}
		const CPackage::TPackage* CPackage::Get(TCode PackageCode_) const
		{
			auto it = _Datas.find(PackageCode_);
			if (it == _Datas.end())
				return nullptr;

			return &it->second;
		}
	}
}