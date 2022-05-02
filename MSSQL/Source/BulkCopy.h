#pragma once

// CBulkCopy �� ����� �� �ܿ� ���������� ����Ǿ�� �ϳ� ���� �Ϸ� �ݹ鿡 ���� ���� �������� �̼���.

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
					; // ���� ������ ó���Ǿ�� �ϴ� ���� ����
				else if (RowsToBePop == 0)
					this_thread::sleep_for(milliseconds(100));
				else
				{
					_Error();
					this_thread::sleep_for(seconds(1));
				}
			}

			// ��� ������ ���Ḹ �������� �ʾҴٸ� LFQueue �� Main������ Push�� ��ŭ üũ�Ͽ� ���ۺ����� �� �� ������, _Worker �� bcp_batch �� ������ ������ ���� Ȯ�� �� �� ���� ó�� �Ұ��ϹǷ�
			// �׳� ���⼭ ���� �ð� ��ٸ� ���� bcp_batch ����� ���ٸ� ���� ������ �����ϰ� ��ȯ.(batch�� ���� �ִٸ�, �����ð� sleep �ϸ� bcp_batch ����� ���� ���̹Ƿ�)
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

					// ID �� ������ �ʵ忡 ������ ���� ���� ��
					//if (bcp_control(DBC(), BCPKEEPIDENTITY, (void*)TRUE) != SUCCEED)
					//	THROWEX();

					// ������ 1000 �� ������ ������
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
			return true; // �ٸ� �����忡�� _Connected�� false�� �ٲ� �� �����Ƿ� ���⼭�� _Connected �� ��ȯ���� �ʰ� true�� ��ȯ
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
			if (!_Connected) // _Connected �� Main���� false�� _Worker���� true�� �����ϴ� ������ �Һ��� ���� ����. _Error()�� Main������ ȣ���ϰ� ���⼭ _Connected �� false�� ����
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
