#include "ClientCore.h"
#include <filesystem>

namespace rso
{
	namespace patch
	{
		using namespace std;

		void CClientCore::_PatchCore(const SPatchData& Data_)
		{
			if (_Data->Version.Main != Data_.Version.Main ||
				_Data->Version.Sub > Data_.Version.Sub)
			{
				_Data = Data_;
			}
			else
			{
				_Data->Version.Sub = Data_.Version.Sub;

				for (auto& i : Data_.Files)
				{
					if (i.second.IsAdded)
						_Data->Files.emplace(i);
					else
						_Data->Files.erase(i.first);
				}
			}

			_Data.Save();
		}
		CClientCore::CClientCore(const wstring& FileName_, const string& DataPath_) :
			_Data(FileName_, true),
			_DataPathFull(current_path() /= DataPath_)
		{
			create_directories(_DataPathFull);
		}
	}
}
