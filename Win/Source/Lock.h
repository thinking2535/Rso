#pragma once

#include "Base.h"
#include <synchapi.h>
#include <atomic>
#include <chrono>
#include <thread>

namespace rso
{
	namespace win
	{
		// 주석코드는 속도는 더 빠르지만 동일 스레드에서 2회 이상 연속 Lock을 걸 수 없음
		//class CLock
		//{
		//private:
		//	SRWLOCK		m_Lock;

		//public:
		//	CLock(){ InitializeSRWLock(&m_Lock); }
		//	~CLock(){}

		//	void		Lock( void ){ AcquireSRWLockExclusive(&m_Lock); }
		//	void		UnLock( void ){ ReleaseSRWLockExclusive(&m_Lock); }
		//	void		SLock( void ){ AcquireSRWLockShared(&m_Lock); }
		//	void		SUnLock( void ){ ReleaseSRWLockShared(&m_Lock); }
		//};

		class CLock
		{
		public:
			struct SAuto
			{
				CRITICAL_SECTION*	pLock;

				SAuto(CLock* pLock_) : pLock(&pLock_->m_Lock)
				{
					EnterCriticalSection(pLock);
				}
				~SAuto()
				{
					LeaveCriticalSection(pLock);
				}
			};

		private:
			CRITICAL_SECTION		m_Lock;

		public:
			CLock(){ InitializeCriticalSection(&m_Lock); }
			~CLock(){ DeleteCriticalSection(&m_Lock); }

			void		Lock(void){ EnterCriticalSection(&m_Lock); }
			void		UnLock(void){ LeaveCriticalSection(&m_Lock); }
		};


		using namespace std;
		using namespace std::chrono;

		class CLock2
		{
		public:
			class CAuto
			{
				CLock2*		_pLock;

			public:
				CAuto(CLock2* pLock_) : _pLock(pLock_)
				{
					_pLock->Lock();
				}
				~CAuto()
				{
					_pLock->UnLock();
				}
			};

		private:
			atomic<int>			_Counter;
			microseconds		_SpinTime;

		public:
			CLock2() : _SpinTime(1) {}
			CLock2(const microseconds& SpinTime_) : _SpinTime(SpinTime_) {}

			void	SetSpinTime(const microseconds& SpinTime_) { _SpinTime = SpinTime_; }
			void	Lock(void);
			bool	LockTry(void);
			void	UnLock(void);
		};
	}
}
