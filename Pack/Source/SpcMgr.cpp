
/************************************************************************/
/* PREPROCESS                                                           */
/************************************************************************/


/************************************************************************/
/* INCLUDES                                                             */
/************************************************************************/
#include "SpcMgr.h"



/************************************************************************/
/* PRIVATE FUNCTIONS                                                    */
/************************************************************************/



/************************************************************************/
/* PUBLIC FUNCTIONS                                                     */
/************************************************************************/
NS_BEGIN

namespace pack
{
	CSpcMgr::CSpcMgr()
	{
	}

	CSpcMgr::~CSpcMgr()
	{
		Clear();
	}

	void		CSpcMgr::Clear( void )
	{
		m_mapSpc.clear();
	}

	ERet		CSpcMgr::Add( SBlock* pBlock_ )
	{
		ERet	Ret = Ret_Ok;

		try
		{
			pair<TSpcMapIt, bool> ret_pair = 
				m_mapSpc.insert( TSpcMap::value_type( pBlock_->Ptr.Size, pBlock_->Size ) );

			if( !ret_pair.second ) return Ret_MapInsertFail;
		}
		catch( ... )
		{
			return Ret_MapInsertFail;
		}

		TSpcMapIt itFound = m_mapSpc.find( pBlock_->Ptr.Size );
		if( itFound == m_mapSpc.end() ) return Ret_MapFindFail;


		TSpcMapIt itPrev = m_mapSpc.end();
		TSpcMapIt itNext = m_mapSpc.end();

		if( itFound != m_mapSpc.begin() )
		{
			itPrev = itFound;
			--itPrev;

			if( itPrev->first+(itPrev->second).Size > itFound->first )
			{
				Ret = Ret_PrevSizeError;
				goto LABEL_ERROR;
			}
		}

		itNext = itFound;
		++itNext;
		if( itNext != m_mapSpc.end() )
		{
			if( itFound->first+(itFound->second).Size > itNext->first )
			{
				Ret = Ret_NextSizeError;
				goto LABEL_ERROR;
			}
		}


		{
			TSpcMapIt itCheck = itFound;
			if( itPrev != m_mapSpc.end() )
			{
				if( itPrev->first+(itPrev->second).Size == itCheck->first )
				{
					itPrev->second.Size += (itCheck->second).Size;
					m_mapSpc.erase( itCheck );
					itCheck = itPrev;
				}
			}

			if( itNext != m_mapSpc.end() )
			{
				if( itCheck->first+(itCheck->second).Size == itNext->first )
				{
					itCheck->second.Size += (itNext->second).Size;
					m_mapSpc.erase( itNext );
				}
			}
		}

		return Ret;

	LABEL_ERROR:

		TSpcMapIt it = m_mapSpc.find( pBlock_->Ptr.Size );
		if( it != m_mapSpc.end() ) m_mapSpc.erase( it );

		return Ret;
	}

	ERet	CSpcMgr::Del( SBlock* pBlock_ )
	{
		TSpcMapIt it = m_mapSpc.find( pBlock_->Ptr.Size );
		if( it == m_mapSpc.end() )
		{
			try
			{
				pair<TSpcMapIt, bool> ret_pair = 
					m_mapSpc.insert( TSpcMap::value_type( (pBlock_->Ptr.Size), pBlock_->Size ) );
				if( !ret_pair.second ) return Ret_MapInsertFail;
			}
			catch( ... )
			{
				return Ret_MapInsertFail;
			}

			TSpcMapIt itCheck = m_mapSpc.find( pBlock_->Ptr.Size );
			if( itCheck == m_mapSpc.end() ) return Ret_MapFindFail;
		
			if( itCheck != m_mapSpc.begin() )
			{
				--itCheck;

				TFileSize Gap = pBlock_->Ptr.Size - (itCheck->first);	// 임시로 삽입된 포인터와 앞의 포인터 차이 ( 음수일 이면 안되고, 그럴리 없음 )

				// 이전 리스트의 영역이 삭제될 영역을 포함하면
				if( (itCheck->second).Size > Gap + pBlock_->Size.Size )
				{
					// 임시 삽입된 리스트의 앞쪽 리스트의 포인터에서 뒤쪽에 삽입될 포인터 만큼 빠진 크기를 뒤쪽 포인터 크기로 하여 삽입
					TFileSize NextPtr = itCheck->first + Gap + pBlock_->Size.Size;
					TFileSize NextSize = (itCheck->second).Size - ( Gap + pBlock_->Size.Size );

					try
					{
						pair<TSpcMapIt, bool> pair = 
							m_mapSpc.insert( TSpcMap::value_type( NextPtr, NextSize ) );
						if( !pair.second ) return Ret_MapInsertFail;
					}
					catch( ... )
					{
						return Ret_MapInsertFail;
					}
				}

				// 앞쪽 리스트 에서 삭제영역 지우고
				(itCheck->second).Size = Gap;

				// 입시 삽입된 리스트 삭제
				m_mapSpc.erase( pBlock_->Ptr.Size );
			}
		}
		else
		{
			if( pBlock_->Size.Size != 0 )
			{
				if( (it->second).Size > pBlock_->Size.Size )
				{
					try
					{
						pair<TSpcMapIt, bool> ret_pair = 
							m_mapSpc.insert( TSpcMap::value_type( (pBlock_->Ptr.Size+pBlock_->Size.Size), (it->second).Size-pBlock_->Size.Size ) );

						// 실패할 리 없음. rso temp 이런것은 어떻게 오류검사 안하고 할 수 있을까??
						// rso temp		if( !ret_pair.second ) do error routine
					}
					catch( ... )
					{
					}
				}

				m_mapSpc.erase( it );
			}
		}

		return Ret_Ok;
	}
};

NS_END
