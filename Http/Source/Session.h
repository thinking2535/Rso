#pragma once

#include "Base.h"
#include "InternetHandle.h"
#include <Rso/Base/LFQueueB.h>
#include <sstream>
#include <deque>

namespace rso::http
{
	using namespace std;

	template<typename... TArgs>
	class CSession
	{
	public:
		using TDownloadChangedFunc = function<void(size_t SessionIndex_, const string& ObjectName_, DWORD Received_, DWORD Total_, const TArgs&... Args_)>;
		using TDownloadCompletedFunc = function<void(size_t SessionIndex_, EHttpRet Ret_, const string& ObjectName_, const TBuffer& Buffer_, const TArgs&... Args_)>;

	private:
		using _TObjectIndex = uint32;
		struct _SObject
		{
			_TObjectIndex Index = 0;
			string Name;
			tuple<TArgs...> Args;

			_SObject(_TObjectIndex Index_, const string& Name_, const TArgs&... Args_) :
				Index(Index_), Name(Name_), Args(Args_...)
			{
			}
		};
		struct _SInObj : public SInObj
		{
			_TObjectIndex Index = 0;

			_SInObj(const SInObj& InObj_, _TObjectIndex Index_) :
				SInObj(InObj_), Index(Index_)
			{
			}
		};
		struct _SOutChangedObj
		{
			_TObjectIndex Index = 0;
			DWORD Received = 0;
			DWORD Total = 0;

			_SOutChangedObj(_TObjectIndex Index_, DWORD Received_, DWORD Total_) :
				Index(Index_), Received(Received_), Total(Total_)
			{
			}
		};
		struct _SOutCompletedObj
		{
			_TObjectIndex Index = 0;
			EHttpRet Ret = EHttpRet::Null;
			TBuffer Buffer;

			_SOutCompletedObj()
			{
			}
		};

		size_t _SessionIndex = 0;
		_TObjectIndex _ObjectCounter = 0;
		deque<_SObject> _Objects;
		CLFQueue<_SInObj> _InObjects;
		CLFQueue<_SOutChangedObj> _OutChangedObjects;
		CLFQueueB<_SOutCompletedObj> _OutCompletedObjects;
		CInternetHandle _Handle;
		TDownloadChangedFunc _DownloadChangedFunc;
		TDownloadCompletedFunc _DownloadCompletedFunc;
		CThread _WorkerThread;
		TErrorLogFunc _ErrorLogFunc;

		template<size_t... Indices_>
		void _DownloadChanged(const _SOutChangedObj* pObj_, index_sequence<Indices_...>)
		{
			_DownloadChangedFunc(_SessionIndex, _Objects.front().Name, pObj_->Received, pObj_->Total, get<Indices_>(_Objects.front().Args)...);
		}
		template<size_t... Indices_>
		void _DownloadCompleted(EHttpRet HttpRet_, const TBuffer& Buffer_, index_sequence<Indices_...>)
		{
			_DownloadCompletedFunc(_SessionIndex, HttpRet_, _Objects.front().Name, Buffer_, get<Indices_>(_Objects.front().Args)...);
		}
		void _Worker(const volatile bool* Exit_)
		{
			while (*Exit_ == false)
			{
				for (auto* pInObject = _InObjects.get();
					pInObject != 0;
					pInObject = _InObjects.get())
				{
					try
					{
						auto pOutCompletedObject = _OutCompletedObjects.new_buf();
						pOutCompletedObject->Index = pInObject->Index;

						try
						{
							// Connect 
							HINTERNET hConnect = InternetConnectA(
								_Handle.GetHandle(),
								pInObject->ServerName.c_str(),
								pInObject->Port,
								pInObject->UserName.c_str(),
								pInObject->PassWord.c_str(),
								INTERNET_SERVICE_HTTP, 0, 0);
							if (hConnect == NULL)
								throw EHttpRet::InternetConnectFail;

							try
							{
								// OpenRequest
								const char* pczRequestObject = 0;
								stringstream Header;
								string ObjParams;

								Header << "Accept: */*\r\n";
								Header << "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0;* Windows NT)\r\n";

								// Add Header User Added
								if (pInObject->Header.size() > 0)
									Header << pInObject->Header;


								switch (pInObject->Method)
								{
								case EMethod::Get:
								{
									ObjParams += pInObject->ObjectName;
									if (pInObject->Params.empty() == false)
									{
										ObjParams += "?";
										ObjParams += pInObject->Params;
									}

									pczRequestObject = ObjParams.data();
								}
								break;

								case EMethod::Post:
								{
									pczRequestObject = pInObject->ObjectName.data();

									//Header << "Content-length: ";
									//Header << pInObject->Params.size();
									//Header << "\r\n\n";
								}
								break;
								}

								Header << "Range: bytes=0-\r\n\n"; // If Request Size 10       0-9\r\n\n

								DWORD Flag = 0;
								if (pInObject->Secure)
									Flag = INTERNET_FLAG_SECURE;

								HINTERNET hRequest = HttpOpenRequestA(hConnect, c_Methos[size_t(pInObject->Method)], pczRequestObject, NULL, NULL, NULL, Flag, 0);
								if (hRequest == NULL)
									throw EHttpRet::OpenRequestFail;

								try
								{
									DWORD BuffLen = sizeof(Flag);
									if (InternetQueryOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&Flag, &BuffLen) == FALSE)
										throw EHttpRet::InternetQueryOptionFail;

									Flag |= SECURITY_IGNORE_ERROR_MASK;
									// 아래 주석은 INTERNET_FLAGS_MASK 이 모두 포함하고 있으나 마지막 INTERNET_FLAG_BGUPDATE 이 정의되어있지 않아 아래와 같이 수정하였음.
									//							INTERNET_FLAG_RELOAD              \
																		//                        | INTERNET_FLAG_RAW_DATA            \
									//                        | INTERNET_FLAG_EXISTING_CONNECT    \
									//                        | INTERNET_FLAG_ASYNC               \
									//                        | INTERNET_FLAG_PASSIVE             \
									//                        | INTERNET_FLAG_NO_CACHE_WRITE      \
									//                        | INTERNET_FLAG_MAKE_PERSISTENT     \
									//                        | INTERNET_FLAG_FROM_CACHE          \
									//                        | INTERNET_FLAG_SECURE              \
									//                        | INTERNET_FLAG_KEEP_CONNECTION     \
									//                        | INTERNET_FLAG_NO_AUTO_REDIRECT    \
									//                        | INTERNET_FLAG_READ_PREFETCH       \
									//                        | INTERNET_FLAG_NO_COOKIES          \
									//                        | INTERNET_FLAG_NO_AUTH             \
									//                        | INTERNET_FLAG_CACHE_IF_NET_FAIL   \
									//                        | SECURITY_INTERNET_MASK            \
									//                        | INTERNET_FLAG_RESYNCHRONIZE       \
									//                        | INTERNET_FLAG_HYPERLINK           \
									//                        | INTERNET_FLAG_NO_UI               \
									//                        | INTERNET_FLAG_PRAGMA_NOCACHE      \
									//                        | INTERNET_FLAG_CACHE_ASYNC         \
									//                        | INTERNET_FLAG_FORMS_SUBMIT        \
									//                        | INTERNET_FLAG_NEED_FILE           \
									//                        | INTERNET_FLAG_RESTRICTED_ZONE     \
									//                        | INTERNET_FLAG_TRANSFER_BINARY     \
									//                        | INTERNET_FLAG_TRANSFER_ASCII      \
									//                        | INTERNET_FLAG_FWD_BACK);

									if (InternetSetOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &Flag, sizeof(Flag)) == FALSE)
										throw EHttpRet::InternetSetOptionFail;

									if (HttpAddRequestHeadersA(hRequest, Header.str().c_str(), (DWORD)-1L, HTTP_ADDREQ_FLAG_ADD) == FALSE)
										throw EHttpRet::AddRequestHeadersFail;

									DWORD dwOptionalLen = 0;
									if (pInObject->Method == EMethod::Post)
										dwOptionalLen = (DWORD)pInObject->Params.size();

									if (HttpSendRequestA(hRequest, NULL, 0, (void*)pInObject->Params.c_str(), dwOptionalLen) == FALSE)
										throw EHttpRet::SendRequestFail;

									DWORD dwTotalSize = 0;
									DWORD dwBuffLen = sizeof(dwTotalSize);
									if (HttpQueryInfoA(hRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &dwTotalSize, &dwBuffLen, 0) == FALSE)
										throw EHttpRet::QueryInfoFail;

									pOutCompletedObject->Buffer.resize(dwTotalSize);

									// pHttpObject_->dwSize = dwSize;

									// rso temp HTTP 버젼 구하기
									//  CHAR aczVer[256] = {};
									//  dwBuffLen = sizeof( aczVer );
									//  if( HttpQueryInfo(pHttpObject_->hRequest, HTTP_QUERY_VERSION, aczVer, &dwBuffLen, 0) == FALSE )
									//  {
									//  LOG( Base_Err_System, GetLastError(), "HttpQueryInfo()" );
									//  return Ret_QueryInfoFail;
									//  }


									//if( pHttpObject_->Info.bDateCheck == true )
									//{
									// SYSTEMTIME SysTime;
									// dwBuffLen = sizeof( SysTime );
									// if( HttpQueryInfo(pHttpObject_->hRequest, HTTP_QUERY_LAST_MODIFIED|HTTP_QUERY_FLAG_SYSTEMTIME, &SysTime, &dwBuffLen, 0) == FALSE )
									// {
									// if( GetLastError() == ERROR_HTTP_HEADER_NOT_FOUND ) return Ret_ObjectNotFound;
									// else return Ret_QueryInfoFail;
									// }


									// if( pHttpObject_->Info.LastModified.wYear == SysTime.wYear &&
									// pHttpObject_->Info.LastModified.wMonth == SysTime.wMonth &&
									// pHttpObject_->Info.LastModified.wDayOfWeek == SysTime.wDayOfWeek &&
									// pHttpObject_->Info.LastModified.wDay == SysTime.wDay &&
									// pHttpObject_->Info.LastModified.wHour == SysTime.wHour &&
									// pHttpObject_->Info.LastModified.wMinute == SysTime.wMinute &&
									// pHttpObject_->Info.LastModified.wSecond == SysTime.wSecond &&
									// pHttpObject_->Info.LastModified.wMilliseconds == SysTime.wMilliseconds )
									// {
									// return Ret_AlreadyHave;
									// }

									// pHttpObject_->Info.LastModified = SysTime;
									//}

									//return Ret_Ok;


									//--------------------------------------------------------------------------------------------------------------
									// 여기는 InternetOpenUrl 만을 사용하는 단순한 Get만 가능한 루틴
									//HINTERNET hHttpFile = InternetOpenUrl(_Session, pInObject->Url.c_str(), NULL, (DWORD)-1, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID, 0);
									//if( hHttpFile == NULL )
									//{
									// _InObjects.Pop();

									// pOutObject->State = EQueStat::Error;
									// _OutObjects.Push();
									// continue;
									//}
									//--------------------------------------------------------------------------------------------------------------



									//DWORD dwSize = 0;
									//if( InternetQueryDataAvailable(pHttpObject->hRequest, &dwSize, 0, 0) == FALSE )
									//{
									// LOG( Base_Err_System, GetLastError(), "InternetQueryDataAvailable() fail" );
									// goto LABEL_ERROR_Downloading;
									//}




									//WCHAR ByteToRead[32] = {};
									//DWORD SizeOfRq = 32;
									//if( HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_REQUEST_HEADERS, ByteToRead, &SizeOfRq , NULL) == FALSE )
									//{
									// Error2();
									// continue;
									//}

									//pOutObject->Size = _ttol(ByteToRead);

									DWORD dwReceivedSize = 0;
									while (dwReceivedSize < dwTotalSize)
									{
										DWORD dwAvailableSize = 0;
										if (InternetQueryDataAvailable(hRequest, &dwAvailableSize, 0, 0) == FALSE)
											throw EHttpRet::InternetQueryDataAvailableFail;

										if (dwAvailableSize == 0) // while (dwReceivedSize < pOutCompletedObject->Total) 에서 걸리므로 여기가 참이면 안됨.
											throw EHttpRet::InvalidAvailableSize;

										DWORD dwReadSize = 0;
										if (InternetReadFile(hRequest, &pOutCompletedObject->Buffer[dwReceivedSize], dwAvailableSize, &dwReadSize) == FALSE)
											throw EHttpRet::InternetReadFileFail;

										if (dwReadSize == 0) // while (dwReceivedSize < pOutCompletedObject->Total) 에서 걸리므로 여기가 참이면 안됨.
											throw EHttpRet::InvalidReadSize;

										dwReceivedSize += dwReadSize;

										try
										{
											_OutChangedObjects.emplace(pInObject->Index, dwReceivedSize, dwTotalSize);
										}
										catch (...)
										{
										}
									}

									pOutCompletedObject->Ret = EHttpRet::Ok;
									_OutCompletedObjects.push();
									_InObjects.pop();
									InternetCloseHandle(hRequest);
									InternetCloseHandle(hConnect);
								}
								catch (...)
								{
									InternetCloseHandle(hRequest);
									throw;
								}
							}
							catch (...)
							{
								InternetCloseHandle(hConnect);
								throw;
							}
						}
						catch (const EHttpRet& HttpRet_)
						{
							pOutCompletedObject->Ret = HttpRet_;
							_OutCompletedObjects.push();
							throw;
						}
						catch (...)
						{
							pOutCompletedObject->Ret = EHttpRet::SystemError;
							_OutCompletedObjects.push();
							throw;
						}
					}
					catch (...)
					{
						_InObjects.pop();
						_ErrorLogFunc();
					}
				}

				this_thread::sleep_for(milliseconds(1));
			}
		}

	public:
		CSession(size_t SessionIndex_, const string& Agent_, TDownloadChangedFunc DownloadChangedFunc_, TDownloadCompletedFunc DownloadCompletedFunc_, TErrorLogFunc ErrorLogFunc_) :
			_SessionIndex(SessionIndex_),
			_Handle(Agent_),
			_DownloadChangedFunc(DownloadChangedFunc_),
			_DownloadCompletedFunc(DownloadCompletedFunc_),
			_WorkerThread(std::bind(&CSession::_Worker, this, _1)),
			_ErrorLogFunc(ErrorLogFunc_)
		{
		}
		void Push(const SInObj& Obj_, const TArgs&... Args_)
		{
			_Objects.emplace_back(_SObject(_ObjectCounter, Obj_.ObjectName, Args_...));

			try
			{
				_InObjects.emplace(_SInObj(Obj_, _ObjectCounter));
			}
			catch (...)
			{
				_Objects.pop_back();
			}

			++_ObjectCounter;
		}
		void Proc(void)
		{
			while (!_Objects.empty())
			{
				auto pCompletedObj = _OutCompletedObjects.get();

				for (auto pObj = _OutChangedObjects.get();
					pObj != nullptr;
					pObj = _OutChangedObjects.get())
				{
					if (pObj->Index != _Objects.front().Index)
						break;

					_DownloadChanged(pObj, index_sequence_for<TArgs...>());
					_OutChangedObjects.pop();
				}

				// pObj 를 _OutChangedObjects.get() 이후에 얻어올 경우 _OutChangedObjects 에 해당 Index의 잔여 Obj 가 남을 수 있기 때문에 이전에 get
				if (pCompletedObj == nullptr) // pObj->Index 가 _Objects.front().Index 와 같음을 보장하므로 Index 비교는 하지 않음.
					break;

				if (pCompletedObj->Index == _Objects.front().Index)
				{
					_DownloadCompleted(pCompletedObj->Ret, pCompletedObj->Buffer, index_sequence_for<TArgs...>());
					pCompletedObj->Buffer.clear();
					_OutCompletedObjects.pop();
				}
				else
				{
					_DownloadCompleted(EHttpRet::NotEnoughMemory, TBuffer(), index_sequence_for<TArgs...>());
				}

				_Objects.pop_front();
			}
		}
	};
}