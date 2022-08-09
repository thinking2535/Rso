#pragma once

#include "Base.h"

namespace rso::gameutil
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
			list<wstring> Examples;
			TCommand Command;

			SCommandInfo(const wstring& Description_, const list<wstring>& Examples_, TCommand Command_) :
				Description(Description_), Examples(Examples_), Command(Command_)
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
				Manual += L"]\n";
				Manual += L" " + i.second.Description + L"\n";

				for (auto& ex : i.second.Examples)
				{
					Manual += L"  " + i.first;

					if (!ex.empty())
						Manual += L" " + ex;

					Manual += L"\n";
				}

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