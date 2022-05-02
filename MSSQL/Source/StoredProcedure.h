#pragma once

#include "MSSQL.h"
#include <Rso/Base/Thread.h>

namespace rso
{
	namespace mssql
	{
		using namespace std;

		template<typename _TKey>
		class CStoredProcedure : protected CMSSQL
		{
		public:
			using TSPRet = int32;

			struct SInObject
			{
				size_t SPNum = 0;
				_TKey Key{};
				CStream InStream;

				template< typename... _TProtos>
				void Set(size_t SPNum_, const _TKey& Key_, const _TProtos&... Protos_)
				{
					SPNum = SPNum_;
					Key = Key_;
					InStream.Send(Protos_...);
				}
			};
			struct SOutObject : public SInObject
			{
				TReturn Ret = 0;
				TSPRet SPRet = 0;
				CStream OutStream;

				void Set(SInObject&& InObject_)
				{
					new (this) SInObject(std::move(InObject_));
					Ret = SQL_SUCCESS;
					SPRet = 0;
					OutStream.clear();
				}
			};
			using TCallback = function<void(SOutObject&)>;
			using TCallbacks = CList<TCallback>;

		private:
			TCallback _Callback;
			CLFQueueB<SInObject> _InBuf;
			CLFQueueB<SOutObject> _OutBuf;
			DWORD _WaitTickForFinal;
			unique_ptr<CThread> _ConnectorThread;
			unique_ptr<CThread> _WorkerThread;

			void _Execute(SOutObject& OutObject_)
			{
				if (!Connected())
				{
					OutObject_.Ret = SQL_ERROR;
					return;
				}

				auto& CmdInfo = _SetCmd(OutObject_.SPNum);

				OutObject_.Ret = ExecuteBegin(OutObject_.InStream);
				if (!SQL_SUCCEEDED(OutObject_.Ret))
					return;

				for (auto itResult = CmdInfo.Results.begin(); itResult != CmdInfo.Results.end(); )
				{
					auto RowCntIDX = OutObject_.OutStream.size();
					TValueSize RowCount = 0;

					try
					{
						OutObject_.OutStream << (int32)RowCount;
					}
					catch (...)
					{
						OutObject_.Ret = SQL_ERROR;
						return;
					}

					// Sp가 바인딩된 row를 반환하지 않고 return 해버린 경우 체크
					// 마지막 Row의 결과가 없다면  직전에 Return 이 넘어오고, 아니면 이후에 넘어옴.
					if (CmdInfo.GetReturn() != 0)
						break;

					for (;;)
					{
						OutObject_.Ret = Fetch(itResult, OutObject_.OutStream);
						if (OutObject_.Ret == SQL_NO_DATA)
							break;

						if (!SQL_SUCCEEDED(OutObject_.Ret))
							return;

						++RowCount;
					}

					//////////////////////////////////////////////////////////////////////////////////////////////////
					// SELECT 중에 락 타임아웃으로 익셉션 난 경우 그 결과셋에대해서는 RowCount 가 0 으로 처리되어 아래 주석
					//SQLINTEGER TestRowCount = 0;
					//auto TestReturn = SQLRowCount(STMT, &TestRowCount);
					////////////////////////////////////////////////////////////////////////////////////////////////////////

					*((int32*)OutObject_.OutStream.at(RowCntIDX)) = (int32)RowCount;

					if ((++itResult) == CmdInfo.Results.end()) // 바인딩된 RowSet과 실제 DB RowSet의 일치 여부 확인 위함
						break;

					OutObject_.Ret = SQLMoreResults(STMT());
					if (OutObject_.Ret == SQL_NO_DATA) // Binding 된 RowSet보다 적게 들어왔을때
					{
						OutObject_.Ret = SQL_ERROR;
						Log(L"Need More DB ResultSet");
						return;
					}
					else if (!SQL_SUCCEEDED(OutObject_.Ret))
					{
						return Cancel();
					}
				}

				OutObject_.Ret = ExecuteEnd(OutObject_.SPRet, OutObject_.OutStream);
			}
			void _Connector(const volatile bool* Exit_)
			{
				while (*Exit_ == false)
				{
					CMSSQL::Connect();
					this_thread::sleep_for(seconds(1));
				}
			}
			void _Worker(const volatile bool* Exit_)
			{
				while (*Exit_ == false)
				{
					SInObject* pInObject = nullptr;
					SOutObject* pOutObject = nullptr;

					try
					{
						for (pInObject = _InBuf.get(), pOutObject = _OutBuf.new_buf();
							pInObject;
							pInObject = _InBuf.get(), pOutObject = _OutBuf.new_buf())
						{
							pOutObject->Set(std::move(*pInObject));

							pOutObject->InStream.SaveState();
							_Execute(*pOutObject);
							pOutObject->InStream.LoadState();

							_OutBuf.push();
							pInObject->InStream.clear();
							_InBuf.pop();
						}
					}
					catch (...)
					{
					}

					this_thread::sleep_for(milliseconds(1));
				}
			}

		public:
			using CMSSQL::AddCmd;
			using CMSSQL::ExecuteDirect;
			using CMSSQL::Connected;

			CStoredProcedure(const SDBOption& Option_, TCallback Callback_, const DWORD WaitTickForFinal_ = MAXDWORD) :
				CMSSQL(Option_), _Callback(Callback_), _WaitTickForFinal(WaitTickForFinal_)
			{
			}
			virtual ~CStoredProcedure()
			{
				while (true)
				{
					this_thread::sleep_for(milliseconds(100));

					if (_OutBuf.get() == nullptr)
						break;

					Proc();
				}
			}
			template<typename... _TProtos>
			bool Push(size_t SPNum_, const _TKey& Key_, const _TProtos&... Protos_)
			{
				if (!Connected())
					return false;

				auto* pInObject = _InBuf.new_buf();
				pInObject->Set(SPNum_, Key_, Protos_...); // throw 하더라도 _InBuf.push() 하지 않았으므로 누수 없음.
				_InBuf.push();

				return true;
			}
			void Connect(void) = delete;
			void Disconnect(void) = delete;
			void Start(int32 TimeOutSeconds_)
			{
				if (_ConnectorThread)
					THROWEX();

				_ConnectorThread.reset(new CThread(std::bind(&CStoredProcedure::_Connector, this, _1)));
				_WorkerThread.reset(new CThread(std::bind(&CStoredProcedure::_Worker, this, _1)));

				auto TimeOutTimePoint = system_clock::now() + seconds(TimeOutSeconds_);
				while (!Connected())
				{
					if (system_clock::now() >= TimeOutTimePoint)
						THROWEXA(L"DB Connect Fail");

					this_thread::sleep_for(milliseconds(100));
				}
			}
			void Proc(void)
			{
				for (auto pOutObject = _OutBuf.get();
					pOutObject != 0;
					pOutObject = _OutBuf.get())
				{
					_Callback(*pOutObject);

					pOutObject->InStream.clear();
					pOutObject->OutStream.clear();

					_OutBuf.pop();
				}
			}
		};
	}
}
