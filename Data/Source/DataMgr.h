#pragma once


// Set, Get �Լ��� ���� �ٸ� �����忡�� ȣ�Ⱑ���ϵ��� �Ͽ� Set ������ ������ �������� ���� ������ ���� �����忡 ������ ��ġ�� �ʵ���.
// Get�� ���� �����忡�� ȣ���ϵ��� �Ͽ�, ȣ�� ���� ������


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
			_TData		_aData[3];		// _aData �� �ε����� �����ϴµ� �ɸ��� �ð��� �� �ϱ� ���� 3���� ���纻�� �д�.
			__int32		_RetDataNum;	// �ܺο��� ������� _aData �� �ε���
			__int32		_NewDataNum;	// �ֱٿ� ���ŵ� _aData �� �ε���
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
			// _WriteDataNum �� 0~2 �� �߿� _NewDataNum, _RetDataNum �Ѵ� �ƴ� ���� ����
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

			return _aData[_RetDataNum];	// �� �Լ������� �����ϹǷ� �д°��� �� ���� ����.
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