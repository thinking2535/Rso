
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

				TFileSize Gap = pBlock_->Ptr.Size - (itCheck->first);	// �ӽ÷� ���Ե� �����Ϳ� ���� ������ ���� ( ������ �̸� �ȵǰ�, �׷��� ���� )

				// ���� ����Ʈ�� ������ ������ ������ �����ϸ�
				if( (itCheck->second).Size > Gap + pBlock_->Size.Size )
				{
					// �ӽ� ���Ե� ����Ʈ�� ���� ����Ʈ�� �����Ϳ��� ���ʿ� ���Ե� ������ ��ŭ ���� ũ�⸦ ���� ������ ũ��� �Ͽ� ����
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

				// ���� ����Ʈ ���� �������� �����
				(itCheck->second).Size = Gap;

				// �Խ� ���Ե� ����Ʈ ����
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

						// ������ �� ����. rso temp �̷����� ��� �����˻� ���ϰ� �� �� ������??
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
