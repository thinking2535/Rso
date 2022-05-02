#pragma once

#include "Base.h"

namespace rso
{
	namespace gameutil
	{
		using namespace std;

		template<typename... _TParams>
		class CCommand
		{
		public:
			using TCommand = function <void(wstringstream&, _TParams&...)>;
			struct SCommandInfo
			{
				wstring Description;
				wstring ParamInfo;
				TCommand Command;

				SCommandInfo(const wstring& Description_, const wstring& ParamInfo_, TCommand Command_) :
					Description(Description_), ParamInfo(ParamInfo_), Command(Command_)
				{
				}
			};

		private:
			map<wstring, SCommandInfo> _Commands;

		public:
			void Insert(const wstring& Command_, const SCommandInfo& CommandInfo_)
			{
				_Commands.emplace(Command_, CommandInfo_);
			}
			void Erase(const wstring& Command_)
			{
				_Commands.erase(Command_);
			}
			wstring GetManual(void) const
			{
				wstring Manual;

				for (auto& i : _Commands)
				{
					Manual += L"[";
					Manual += i.first;
					Manual += L"] : ";
					Manual += i.second.Description;
					Manual += L" ParamInfo : ";
					Manual += i.second.ParamInfo;
					Manual += L"\n";
				}

				return Manual;
			}
			bool Call(const wstring& CommandParameter_, _TParams&... Params_)
			{
				wstringstream ss(CommandParameter_);
				wstring Command;
				ss >> Command;

				wcslwr((wchar_t*)Command.c_str());

				auto itCommand = _Commands.find(Command);
				if (itCommand == _Commands.end())
					return false;

				itCommand->second.Command(ss, Params_...);

				return true;
			}
		};
	}
}