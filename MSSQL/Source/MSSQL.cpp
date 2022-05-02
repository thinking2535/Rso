#include "MSSQL.h"

namespace rso::mssql
{
	wstring CMSSQL::GetFullName(const wstring& Name_)
	{
		return L"[dbo].[" + Name_ + L"]";
	}

	CMSSQL::SCmdInfo& CMSSQL::_SetCmd(size_t CmdNum_)
	{
		_CmdNum = CmdNum_;
		return _CmdInfos[_CmdNum];
	}
	CMSSQL::CMSSQL(const SDBOption& Option_) :
		CDBC(ENV(), Option_.LoginTimeOut, Option_.ConnTimeOut), _ConnectInfo(Option_.ConnInfo)
	{
	}
	void CMSSQL::CreateSP(const wstring& Name_, const wstring& AlterQuery_)
	{
		auto PreQuery = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + Name_ + LR"(]') AND type in (N'P', N'PC'))
BEGIN
	EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE [dbo].[)" + Name_ + LR"(] AS' 
END)";
		if (!SQL_SUCCEEDED(ExecuteDirect(PreQuery)))
			THROWEX();

		if (!SQL_SUCCEEDED(ExecuteDirect(AlterQuery_)))
			THROWEX();
	}
	bool CMSSQL::Connect(void)
	{
		if (_Connected)
			return _Connected;

		try
		{
			if (!SQL_SUCCEEDED(SQLDriverConnectW(DBC(), NULL, (SQLWCHAR*)_ConnectInfo.ConnStr.c_str(), SQL_NTS, 0, (SQLSMALLINT)_ConnectInfo.ConnStr.size(), 0, SQL_DRIVER_NOPROMPT)))
				THROWEX();

			try
			{
				for (auto it = _CmdInfos.begin(); it != _CmdInfos.end(); ++it)
				{
					_STMTs.emplace_at(it.Index(), CSTMT(DBC(), _ConnectInfo.QueryTimeOut));

					if (!SQL_SUCCEEDED(SQLPrepareW(_STMTs.back().STMT(), (SQLWCHAR*)it->QueryString().c_str(), SQL_NTS)))
						THROWEX();

					for (TParamNum i = 0; i < it->Params.size(); ++i)
					{
						if (!SQL_SUCCEEDED(it->Params[i]->BindPrepare(_STMTs.back().STMT(), i + 1)))
							THROWEX();
					}
				}
			}
			catch (...)
			{
				_STMTs.clear();
				SQLDisconnect(DBC());
				throw;
			}
		}
		catch (...)
		{
			return false;
		}

		_Connected = true;

		return true; // 다른 스레드에서 _Connected를 false로 바꿀 수 있으므로 여기서는 _Connected 를 반환하지 않고 true를 반환
	}
	void CMSSQL::Disconnect(void)
	{
		_DirectSTMT.reset();
		_STMTs.clear();
		SQLDisconnect(DBC());
		_Connected = false;
	}
	void CMSSQL::Error(SQLHSTMT hSTMT_)
	{
		if (!SqlError(_Log, ENV(), DBC(), hSTMT_))
			Disconnect();
	}
	void CMSSQL::Cancel(void)
	{
		Error(_STMTs[_CmdNum].STMT());

		if (!_Connected)
			return;

		if (!SQL_SUCCEEDED(SQLCancel(_STMTs[_CmdNum].STMT())))
			Error(_STMTs[_CmdNum].STMT());
	}
	TReturn CMSSQL::ExecuteBegin(CStream& InStream_)
	{
		try
		{
			for (auto& it : _CmdInfos[_CmdNum].Params)
				it->Set(InStream_);

			TReturn Ret = SQL_SUCCESS;
			for (TParamNum i = 0; i < _CmdInfos[_CmdNum].Params.size(); ++i)
			{
				Ret = _CmdInfos[_CmdNum].Params[i]->BindExecute(_STMTs[_CmdNum].STMT(), i + 1);
				if (!SQL_SUCCEEDED(Ret))
					throw Ret;
			}

			// Sp가 바인딩된 row를 반환하지 않고 return 해버린 경우, 반환값이 들어오며,
			// 그때는 SQLFetch 하면 잘못된 커서가 되므로 이를 방지하기 위해 fetch 전에 반환값 체크하기 위해 사용하지 않는 -1을 저장
			_CmdInfos[_CmdNum].SetReturn(0);

			Ret = SQLExecute(_STMTs[_CmdNum].STMT());
			if (Ret == SQL_NEED_DATA)
			{
				Ret = SQLParamData(_STMTs[_CmdNum].STMT(), NULL);
				if (Ret == SQL_NEED_DATA)
				{
					for (TParamNum i = 0; i < _CmdInfos[_CmdNum].Params.size(); ++i)
					{
						Ret = _CmdInfos[_CmdNum].Params[i]->ParamData(_STMTs[_CmdNum].STMT());
						if (Ret != SQL_NEED_DATA)
						{
							if (!SQL_SUCCEEDED(Ret))
								throw Ret;
						}
					}
				}
				else if (!SQL_SUCCEEDED(Ret))
				{
					throw Ret;
				}
			}
			else if (!SQL_SUCCEEDED(Ret))
			{
				throw Ret;
			}

			return Ret;
		}
		catch (TReturn Ret_)
		{
			Cancel();
			return Ret_;
		}
		catch (...)
		{
			return SQL_ERROR;
		}
	}
	TReturn CMSSQL::ExecuteEnd(int32& SpRet_, CStream& OutStream_)
	{
		TReturn Ret = SQLMoreResults(_STMTs[_CmdNum].STMT()); // 바인딩된 것보다 실제 결과가 더 많은지 체크하기 위함.(For Getting Return)
		if (SQL_SUCCEEDED(Ret)) // Binding 된 RowSet보다 많게 들어왔을때
		{
			Ret = SQL_ERROR;
			_Log.Push(L"Over DB ResultSet");
			Cancel();
			return Ret;
		}
		else if (Ret != SQL_NO_DATA)
		{
			Cancel();
			return Ret;
		}

		SpRet_ = _CmdInfos[_CmdNum].GetReturn();
		if (SpRet_ == 0)
		{
			try
			{
				for (auto& Param : _CmdInfos[_CmdNum].Params)
					Param->Get(OutStream_);
			}
			catch (...)
			{
				return SQL_ERROR;
			}
		}

		return SQL_SUCCESS;
	}
	TReturn CMSSQL::Fetch(TResults::iterator itResult_, CStream& OutStream_)
	{
		TReturn Ret = SQLFetch(_STMTs[_CmdNum].STMT());
		if (Ret == SQL_NO_DATA)
			return Ret;

		if (!SQL_SUCCEEDED(Ret))
		{
			Cancel();
			return Ret;
		}

		try
		{
			for (TParamNum i = 0; i < itResult_->size(); ++i)
			{
				Ret = (*itResult_)[i]->GetData(_STMTs[_CmdNum].STMT(), i + 1, OutStream_);
				if (!SQL_SUCCEEDED(Ret))
					throw Ret;
			}

			return Ret;
		}
		catch (TReturn Ret_)
		{
			Cancel();
			return Ret_;
		}
		catch (...)
		{
			Cancel();
			return SQL_ERROR;
		}
	}
	TReturn CMSSQL::ExecuteDirect(const wstring& Query_)
	{
		try
		{
			if (!_DirectSTMT)
				_DirectSTMT.reset(new CSTMT(DBC(), _ConnectInfo.QueryTimeOut));
		}
		catch (...)
		{
			return SQL_ERROR;
		}

		TReturn Ret = SQLExecDirectW(_DirectSTMT->STMT(), (SQLWCHAR*)Query_.c_str(), SQL_NTS);
		if (!SQL_SUCCEEDED(Ret))
		{
			Error(_DirectSTMT->STMT());
			return Ret;
		}

		return Ret;
	}
	void CMSSQL::Log(const wstring& Msg_)
	{
		_Log.Push(Msg_);
	}
}
