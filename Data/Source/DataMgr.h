#pragma once


// Set, Get 함수는 각각 다른 스레드에서 호출가능하도록 하여 Set 에의한 데이터 갱신으로 인한 지연을 메인 스레드에 영향을 미치지 않도록.
// Get은 메인 스레드에서 호출하도록 하여, 호출 지연 없도록


#include "Base.h"
#include <Rso/Base/Lock.h>
#include <chrono>


namespace rso
{
	namespace data
	{
		using namespace base;
		using namespace std;
		using namespace chrono;

		template< typename _TData >
		class CDataMgr
		{
			CLock2		_Lock;
			_TData		_aData[3];		// _aData 의 인덱스를 변경하는데 걸리는 시간만 락 하기 위해 3개의 복사본을 둔다.
			__int32		_RetDataNum;	// 외부에서 사용중인 _aData 의 인덱스
			__int32		_NewDataNum;	// 최근에 갱신된 _aData 의 인덱스
			bool		_Changed;
			__int32		_WriteDataNum;

			__inline void	_SetBegin( void );
			__inline void	_SetEnd( void );

		public:
			CDataMgr();
			virtual ~CDataMgr();

			const _TData&	Get(void);
			void		Set( const _TData& Data_ );	// ooo _TData&&
			__inline bool	Changed(void) const { return _Changed; }
		};

		template< typename _TData >
		void	CDataMgr<_TData>::_SetBegin( void )
		{
			_Lock.Lock();
			// _WriteDataNum 에 0~2 수 중에 _NewDataNum, _RetDataNum 둘다 아닌 수를 저장
			_WriteDataNum = (_NewDataNum+1);
			_WriteDataNum %= 3;
			if( _WriteDataNum == _RetDataNum )
			{
				++_WriteDataNum;
				_WriteDataNum %= 3;
			}
			_Lock.UnLock();
		}
		template< typename _TData >
		void	CDataMgr<_TData>::_SetEnd( void )
		{
			_Lock.Lock();
			_NewDataNum = _WriteDataNum;
			_Changed = true;
			_Lock.UnLock();
		}

		template< typename _TData >
		CDataMgr<_TData>::CDataMgr() :
			_RetDataNum(),
			_NewDataNum(),
			_Changed(false),
			_WriteDataNum()
		{
		}

		template< typename _TData >
		CDataMgr<_TData>::~CDataMgr()
		{
		}

		template< typename _TData >
		const _TData&	CDataMgr<_TData>::Get(void)
		{
			if( _Changed )
			{
				_Lock.Lock();
				_RetDataNum = _NewDataNum;
				_Changed = false;
				_Lock.UnLock();
			}

			return _aData[_RetDataNum];	// 본 함수에서만 변경하므로 읽는것은 락 걸지 않음.
		}

		template< typename _TData >
		void	CDataMgr<_TData>::Set( const _TData& Data_ )	// ooo _TData&&
		{
			_SetBegin();
			_aData[_WriteDataNum] = Data_;	// ooo _TData std::move, std::forward
			_SetEnd();
		}
	}
}