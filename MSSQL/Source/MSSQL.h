#pragma once

#include "STMT.h"
#include "DBC.h"
#include "ENV.h"
#include "Type.h"
#include <Rso/Base/List.h>
#include <Rso/Base/LFQueueB.h>
#include <Rso/Proto/Parser.h>

namespace rso::mssql
{
	using namespace std;
	using namespace base;
	using namespace proto;

	class CMSSQL : private CENV, private CDBC
	{
	public:
		static wstring GetFullName(const wstring& Name_);
		using TParams = vector<unique_ptr<CParam>>;
		struct SCmdInfo
		{
			wstring SPName;
			TParams Params;
			TResults Results;

		public:
			SCmdInfo() {}
			SCmdInfo(const SCmdInfo& Obj_) = delete;
			SCmdInfo(SCmdInfo&& Obj_) :
				SPName(std::move(Obj_.SPName)),
				Params(std::move(Obj_.Params)),
				Results(std::move(Obj_.Results))
			{
			}
			template< typename _TParamType, typename _TReturnType >
			void Bind(const wstring& SPName_, bool DoesHaveReturn_)
			{
				// Bind Return
				if (DoesHaveReturn_)
					Params.emplace_back(new CParamReturn());

				// Bind InputParameters
				bool IsRowOpened = false;
				CParser Parser(_TParamType::StdName());
				for (auto Token = Parser.Get();
					Token != L"";
					Token = Parser.Get())
				{
					if (IsRowOpened)
					{
						if (Token == L"}")
							IsRowOpened = false;
						else
							((CParamTVP*)Params.back().get())->AddField(GetType(Token));
					}
					else
					{
						if (Token == L"{")
						{
							Params.emplace_back(new CParamTVP());
							IsRowOpened = true;
						}
						else
						{
							// 코드만 복잡해지므로 현재는 CParamInputOutput 를 쓰지 않음.
							// Params.emplace_back((*itBindInfo == L'1' ? new CParamInputOutput(GetType(Token)) : new CParamInput(GetType(Token))));
							Params.emplace_back(new CParamInput(GetType(Token)));
						}
					}
				}


				// Bind Results
				IsRowOpened = false;
				Parser = CParser(_TReturnType::StdName());
				for (auto Token = Parser.Get();
					Token != L"";
					Token = Parser.Get())
				{
					if (IsRowOpened)
					{
						if (Token == L"}")
						{
							IsRowOpened = false;
							continue;
						}
						else
						{
							Results.back().emplace_back(new CResult(GetType(Token)));
						}
					}
					else
					{
						if (Token == L"{")
						{
							Results.emplace_back(TResult());
							IsRowOpened = true;
						}
						else
						{
							break;
						}
					}
				}

				SPName = SPName_;
			}
			wstring QueryString(void) const
			{
				wstring Str;

				Str += L"{";

				auto it = Params.begin();
				if (it != Params.end() &&
					(*it)->IsReturn())
				{
					Str += L"?=";
					++it;
				}


				Str += L"call ";
				Str += SPName;


				if (Params.size() > 0)
				{
					Str += L"(";

					while (it != Params.end())
					{
						Str += L"?";
						if (++it != Params.end())
							Str += L",";
					}

					Str += L")";
				}

				Str += L"}";

				return Str;
			}
			bool HasReturn(void) const
			{
				if (Params.empty())
					return false;

				return Params.front()->IsReturn();
			}
			void SetReturn(int32 Return_)
			{
				if (!HasReturn())
					return;

				((CParamReturn*)Params.front().get())->SetValue(Return_);
			}
			int32 GetReturn(void) const
			{
				if (!HasReturn())
					return 0;

				return ((CParamReturn*)Params.front().get())->GetValue();
			}
		};

	private:
		volatile bool _Connected = false;
		SConnectInfo _ConnectInfo;
		CList<SCmdInfo> _CmdInfos;
		CList<CSTMT> _STMTs;
		size_t _CmdNum = 0;
		unique_ptr<CSTMT> _DirectSTMT;
		CLog _Log{ Target_Console | Target_File, EPeriod::Day, L"SqlLog/", L"Normal", ELocale::Korean };

	protected:
		SCmdInfo& _SetCmd(size_t CmdNum_);

	public:
		CMSSQL(const SDBOption& Option_);
		void CreateSP(const wstring& Name_, const wstring& AlterQuery_);
		template< typename _TParamType, typename _TResultType >
		void AddCmd(size_t SPNum_, const wstring& SPName_, bool DoesHaveReturn_)
		{
			auto itCmdInfo = _CmdInfos.emplace_at(SPNum_, SCmdInfo());
			itCmdInfo->Bind<_TParamType, _TResultType>(SPName_, DoesHaveReturn_);
		}
		inline bool Connected(void) const { return _Connected; }
		bool Connect(void);
		void Disconnect(void);
		void Error(SQLHSTMT hSTMT_);
		void Cancel(void);
		SQLHSTMT STMT(void) const { return _STMTs[_CmdNum].STMT(); }
		TReturn ExecuteBegin(CStream& InStream_);
		TReturn ExecuteEnd(int32& SpRet_, CStream& OutStream_);
		TReturn Fetch(TResults::iterator itResult_, CStream& OutStream_);
		TReturn ExecuteDirect(const wstring& Query_);
		void Log(const wstring& Msg_);
	};
}
