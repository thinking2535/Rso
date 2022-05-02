#pragma once

#include "Base.h"
#include <filesystem>
#include <Rso/Base/Option.h>
#include <Rso/Json/Json.h>

namespace rso
{
	namespace gameutil
	{
		using namespace std;
		using namespace filesystem;
		using namespace base;
		using namespace json;

		template<typename TData>
		class COptionJson : public COption<TData>
		{
		public:
			using COption<TData>::operator*;
			using COption<TData>::operator->;
			using COption<TData>::operator=;

			COptionJson(const wstring& FileName_, bool NoException_) :
				COption<TData>(FileName_)
			{
				CStream Stream;

				try
				{
					Stream.LoadFile(COption<TData>::_FileName);
				}
				catch (...)
				{
					if (!NoException_)
						throw;

					Save();
					return;
				}

				try
				{
					CJson(Stream.to_string()) >> COption<TData>::_Data;
				}
				catch (...)
				{
					if (!NoException_)
						throw;

					Reset();
					Save();
				}
			}
			COptionJson(const wstring& FileName_, const TData& Data_) :
				COption<TData>(FileName_, Data_)
			{
				Save();
			}
			void Save(void) override
			{
				auto FullPath = absolute(COption<TData>::_FileName);
				create_directories(FullPath.parent_path());
				std::ofstream(FullPath) << CJson(COption<TData>::_Data).toStyledString();
			}
		};
	}
}
