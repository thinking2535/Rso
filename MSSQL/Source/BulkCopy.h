#pragma once

// CBulkCopy 가 종료될 때 잔여 쿼리전송이 보장되어야 하나 전송 완료 콜백에 대한 정보 부족으로 미설계.

#include "Base.h"
#include "ENV.h"
#include "DBC.h"
#include <Rso/Base/List.h>
#include <Rso/Base/Thread.h>
#include <Rso/Proto/Parser.h>
#include "BulkCopyType.h"

namespace rso::mssql::bulkcopy
{
	using namespace base;
	using namespace proto;

	template<typename _TProto>
	class CBulkCopy : private CENV, private CDBC
	{
		using _TRow = vector<unique_ptr<SType>>;

		SConnectInfo _ConnectInfo;
		CLog _Log{ Target_Console | Target_File, EPeriod::Day, L"BulkCopyLog/", L"Normal", ELocale::Korean };
		wstring _TblName;
		_TRow _Row;
		CStream _InBuf;
		volatile bool _Connected = false;
		unique_ptr<CThread> _WorkerThread;

		void _Error(void)
		{
			if (!SqlError(_Log, ENV(), DBC(), SQL_NULL_HSTMT))
				_Disconnect();
		}
		void _Worker(const volatile bool* Exit_)
		{
			while (*Exit_ == false)
			{
				if (!_Connect())
				{
					this_thread::sleep_for(seconds(1));
					continue;
				}

				DBINT RowsToBePop = bcp_batch(DBC());
				if (RowsToBePop > 0)
					; // 가장 빠르게 처리되어야 하는 조건 먼저
				else if (RowsToBePop == 0)
					this_thread::sleep_for(milliseconds(100));
				else
				{
					_Error();
					this_thread::sleep_for(seconds(1));
				}
			}

			// 디비 서버와 연결만 끊어지지 않았다면 LFQueue 로 Main측에서 Push한 만큼 체크하여 전송보장을 할 수 있으나, _Worker 의 bcp_batch 로 연결이 끊어진 것을 확인 할 수 없어 처리 불가하므로
			// 그냥 여기서 일정 시간 기다린 다음 bcp_batch 결과가 없다면 없는 것으로 간주하고 반환.(batch할 것이 있다면, 일정시간 sleep 하면 bcp_batch 결과로 나올 것이므로)
			for (this_thread::sleep_for(milliseconds(100)); bcp_batch(DBC()) > 0; this_thread::sleep_for(milliseconds(100)));
		}
		bool _Connect(void)
		{
			if (_Connected)
				return true;

			try
			{
				if (!SQL_SUCCEEDED(SQLSetConnectAttrW(DBC(), SQL_COPT_SS_BCP, (SQLPOINTER)SQL_BCP_ON, SQL_IS_INTEGER)))
					THROWEX();

				if (!SQL_SUCCEEDED(SQLDriverConnectW(DBC(), NULL, (SQLWCHAR*)_ConnectInfo.ConnStr.c_str(), SQL_NTS, 0, (SQLSMALLINT)_ConnectInfo.ConnStr.size(), 0, SQL_DRIVER_NOPROMPT)))
					THROWEX();

				try
				{
					if (bcp_initW(DBC(), _TblName.c_str(), NULL, NULL, DB_IN) != SUCCEED)
						THROWEX();

					// ID 로 설정된 필드에 강제로 값을 넣을 때
					//if (bcp_control(DBC(), BCPKEEPIDENTITY, (void*)TRUE) != SUCCEED)
					//	THROWEX();

					// 파일을 1000 행 단위로 보낼때
					//if (bcp_control(DBC(), BCPBATCH, (void*)1000) == FAIL)
					//	THROWEX();

					// Bind Input Parameter
					for (auto it = _Row.begin();
						it != _Row.end();
						++it)
					{
						if (bcp_bind(
							DBC(),
							(LPCBYTE)(*it)->pData,
							0,
							(*it)->Get_cbData(),
							(*it)->Get_pTerm(),
							(*it)->Get_cbTerm(),
							(*it)->Get_eDataType(),
							(INT)(it - _Row.begin()) + 1) != SUCCEED)
							THROWEX();
					}
				}
				catch (...)
				{
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
		void _Disconnect(void)
		{
			SQLDisconnect(DBC());
			_Connected = false;
		}

	public:
		CBulkCopy(
			const SDBOption& Option_,
			const wstring& TblName_,
			const wstring& Defaults_
		) :
			CDBC(ENV(), Option_.LoginTimeOut, Option_.ConnTimeOut), _ConnectInfo(Option_.ConnInfo),
			_TblName(TblName_)
		{
			wstring::size_type ParamNum = 0;
			CParser Parser(_TProto::StdName());

			for (auto Token = Parser.Get();
				Token != L"";
				Token = Parser.Get(), ++ParamNum)
			{
				if (ParamNum + 1 > Defaults_.size())
					THROWEX();

				auto Default = (Defaults_[ParamNum] == L'1' ? true : false);

				SType* pType = 0;

				if (Token == L"int8" ||
					Token == L"bool" ||
					Token == L"uint8")
				{
					pType = new STypeInt8(Default);
				}
				else if (Token == L"int16" ||
					Token == L"uint16")
				{
					pType = new STypeInt16(Default);
				}
				else if (Token == L"int32" ||
					Token == L"uint32")
				{
					pType = new STypeInt32(Default);
				}
				else if (Token == L"int64" ||
					Token == L"uint64")
				{
					pType = new STypeInt64(Default);
				}
				else if (Token == L"float")
				{
					pType = new STypeFloat(Default);
				}
				else if (Token == L"double")
				{
					pType = new STypeDouble(Default);
				}
				else if (Token == L"time_point")
				{
					pType = new STypeTimePoint(Default);
				}
				else if (Token == L"datetime")
				{
					pType = new STypeDateTime(Default);
				}
				else if (Token == L"string")
				{
					pType = new STypeString(Default);
				}
				else if (Token == L"wstring")
				{
					pType = new STypeWString(Default);
				}
				else if (Token == L"u16string")
				{
					pType = new STypeWString(Default);
				}
				else
				{
					THROWEX();
				}

				_Row.emplace_back(unique_ptr<SType>(pType));
			}

			if (!_Connect())
				THROWEX();

			_WorkerThread.reset(new CThread(std::bind(&CBulkCopy::_Worker, this, _1)));
		}
		bool Push(const _TProto& Row_)
		{
			if (!_Connected) // _Connected 는 Main에서 false로 _Worker에서 true로 변경하는 생산자 소비자 룰을 따름. _Error()는 Main에서만 호출하고 여기서 _Connected 를 false로 만듦
				return false;

			try
			{
				_InBuf << Row_;

				for (auto& Field : _Row)
					Field->SetData(_InBuf);
			}
			catch (...)
			{
				_InBuf.clear();
				return false;
			}

			if (bcp_sendrow(DBC()) != SUCCEED)
			{
				_Error();
				return false;
			}

			return true;
		}
	};
}
