#pragma once

#include "MSSQL.h"

namespace rso::mssql
{
	template<typename _TIn, typename _TOut>
	class CBulkSelect : protected CMSSQL
	{
		using _TRowCallback = function<bool(TValueSize RowNum_, CStream& Row_)>;
		using _TParamCallback = function<void(TReturn Ret_, int32 SpRet_, CStream& OutParams_)>;

		bool _Busy = false;
		TResults::iterator _itRow;
		_TRowCallback _RowCallback;
		_TParamCallback _ParamCallback;
		CStream _Stream;
		TValueSize _RowCount = 0;

		void _Fetch(void)
		{
			TReturn Ret = SQL_SUCCESS;

			for (;;)
			{
				_Stream.clear();
				Ret = Fetch(_itRow, _Stream);
				if (Ret == SQL_NO_DATA)
					break;

				if (!SQL_SUCCEEDED(Ret))
				{
					_ParamCallback(Ret, 0, _Stream);
					_Busy = false;
					return;
				}

				++_RowCount;

				if (!_RowCallback(_RowCount, _Stream))
					return;
			}

			int32 SPRet = 0;
			_Stream.clear();
			Ret = ExecuteEnd(SPRet, _Stream);

			_ParamCallback(Ret, SPRet, _Stream);

			_Busy = false;
		}

	public:
		CBulkSelect(const SDBOption& Option_, _TRowCallback RowCallback_, _TParamCallback ParamCallback_, const wstring& SPName_) :
			CMSSQL(Option_),
			_RowCallback(RowCallback_),
			_ParamCallback(ParamCallback_)
		{
			AddCmd<_TIn, _TOut>(0, SPName_, true);

			auto& CmdInfo = _SetCmd(0);

			if (CmdInfo.Results.size() != 1)
				THROWEXA(L"Results Cnt must be 1");

			_itRow = CmdInfo.Results.begin();
		}
		void Connect(void) = delete;
		void Disconnect(void) = delete;
		bool Push(const _TIn& In_)
		{
			if (!CMSSQL::Connect())
				return false;

			if (!Connected())
				return false;

			if (_Busy)
				return false;

			_Stream.clear();

			try
			{
				_Stream << In_;
			}
			catch (...)
			{
				return false;
			}

			TReturn Ret = ExecuteBegin(_Stream);
			if (!SQL_SUCCEEDED(Ret))
			{
				_ParamCallback(Ret, 0, _Stream);
				return true;
			}

			_RowCount = 0;

			_Busy = true;

			_Fetch();

			return true;
		}
		void Proc(void)
		{
			if (!_Busy)
				return;

			_Fetch();
		}
	};
}