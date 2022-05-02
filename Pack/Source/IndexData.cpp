/************************************************************************/
/* PREPROCESS                                                           */
/************************************************************************/

/************************************************************************/
/* INCLUDES                                                             */
/************************************************************************/
#include "IndexData.h"


/************************************************************************/
/* PRIVATE FUNCTIONS                                                    */
/************************************************************************/
NS_BEGIN

namespace pack
{
	ERet		CIndexData::AddCore( const TFilePath& FilePath_, SFile*& rpAddedFile_o )
	{
		ERet		Ret = Ret_Ok;
		SFile		AddedFile;
		TFilePath	FilePath = FilePath_;

		PathDelimToSlash( FilePath );

		try
		{
			pair<TMapIt, bool> ret = 
				m_Map.insert( TMap::value_type( tstring(FilePath.GetPtr()), AddedFile) );
			if( !ret.second ) GO_ERROR( Ret_MapInsertFail );
		}
		catch( ... )
		{
			GO_ERROR( Ret_MapInsertFail );
		}

		{
			TMapIt it = m_Map.find( FilePath.GetPtr() );
			if( it == m_Map.end() )
			{
				Ret = Ret_MapFindFail;
				goto LABEL_ERROR;
			}

			rpAddedFile_o = &(it->second);
		}

		return Ret;

	LABEL_ERROR:

		return Ret;
	}

	void						CIndexData::DelCore( const TFilePath& FilePath_ )
	{
		TFilePath	FilePath = FilePath_;

		PathDelimToSlash( FilePath );

		TMapIt	it = m_Map.find( FilePath.GetPtr() );
		if( it == m_Map.end() ) return;

		if( (it->second).pData != NULL ) free( (it->second).pData );

		m_Map.erase( it );
	}

	ERet			CIndexData::PushBlock( SFile* pFile_, SBlock* pBlock_ )
	{
		ERet	Ret = Ret_Ok;

		SBlock* pData = (SBlock*)realloc( pFile_->pData, sizeof(SBlock)*(pFile_->Cnt+1) );
		if( pData == NULL )
		{
			Ret = Ret_NotEnoughMemory;
			goto LABEL_ERROR;
		}

		pFile_->pData = pData;
		pFile_->pData[ (pFile_->Cnt)++ ] = *pBlock_;

		return Ret;

	LABEL_ERROR:

		return Ret;
	}

	/************************************************************************/
	/* PUBLIC FUNCTIONS                                                     */
	/************************************************************************/
	CIndexData::CIndexData()
	{
		m_pData = NULL;
		m_DataSize = 0;
		m_HeaderPtr = 0;
		m_HeaderSize = 0;
	}

	CIndexData::~CIndexData()
	{
	}

	ERet			CIndexData::Initialize( void )
	{
		ERet Ret = Ret_Ok;

		SBlock Block( 0, c_FileSize_Max );

		ERet SRet = m_FreeMgr.Add( &Block );
		if( SRet != Ret_Ok )
		{
			Ret = Ret_SpcMgrAddFail;
			goto LABEL_ERROR;
		}

		return Ret;

	LABEL_ERROR:

		return Ret;
	}

	void			CIndexData::Finalize( void )
	{
		if( m_pData != NULL )
		{
			free( m_pData );
			m_pData = NULL;
		}
		m_FreeMgr.Clear();
		m_FileMgr.Clear();
		Clear();
	}

	ERet		CIndexData::Add( const TFilePath& FilePath_, CHAR* pData_, TFileSize Size_, TFileSize& rOverSize_o )
	{
		ERet	Ret = Ret_Ok;

		TFileSize	DataSize = Size_;
		SFile*		pAddedFile = NULL;
		TFileSize	OverSize = 0;


		Ret = AddCore( FilePath_, pAddedFile );
		if( Ret != Ret_Ok ) return Ret;


		for( TSpcMapIt it = m_FreeMgr.Begin();
			it != m_FreeMgr.End();
			 )
		{
			if( DataSize == 0 ) break;

			TSpcMapIt	itCheck = it;
			++it;

			TFileSize	InsertSize = min( (itCheck->second).Size, DataSize );
			DataSize -= InsertSize;

			SBlock	Block( itCheck->first, InsertSize );
			Ret = PushBlock( pAddedFile, &Block );
			if( Ret != Ret_Ok ) goto LABEL_ERROR;

			ERet SRet = m_FileMgr.Add( &Block );
			if( SRet != Ret_Ok )
			{
				Ret = Ret_FileMgrAddFail;
				goto LABEL_ERROR;
			}

			if( it == m_FreeMgr.End() ) OverSize = InsertSize;
			m_FreeMgr.Del( &Block );
		}

		if( DataSize != 0 )
		{
			Ret = Ret_NotEnoughFreeSize;
			goto LABEL_ERROR;
		}

		pAddedFile->CheckSum = GetCheckSum( pData_, Size_ );
	
		Ret = WriteData();
		if( Ret != Ret_Ok ) goto LABEL_ERROR;

		rOverSize_o = OverSize;

		return Ret;

	LABEL_ERROR:

		if( pAddedFile != NULL  &&  pAddedFile->pData != NULL )
		{
			for( TBlockCnt cnt=0; cnt<pAddedFile->Cnt; ++cnt )
			{
				m_FileMgr.Del( &pAddedFile->pData[cnt] );
				m_FreeMgr.Add( &pAddedFile->pData[cnt] );
			}
		}

		DelCore( FilePath_ );

		return Ret;
	}

	ERet			CIndexData::Del( const TFilePath& FilePath_ )
	{
		ERet	Ret = Ret_Ok;

		SFile* pFile = GetFile( FilePath_ );
		if( pFile == NULL )
		{
			Ret = Ret_GetFileFail;
			goto LABEL_ERROR;
		}


		for( TBlockCnt cnt=0; cnt<pFile->Cnt; ++cnt )
		{
			ERet SRet = m_FreeMgr.Add( &(pFile->pData[cnt]) );
			if( SRet != Ret_Ok )
			{
				Ret = Ret_FreeMgrAddFail;
				goto LABEL_ERROR;
			}
		}

		for( TBlockCnt cnt=0; cnt<pFile->Cnt; ++cnt )
			m_FileMgr.Del( &pFile->pData[cnt] );


		DelCore( FilePath_ );

		Ret = WriteData();
		if( Ret != Ret_Ok ) goto LABEL_ERROR;

		return Ret;

	LABEL_ERROR:

		return Ret;
	}

	void			CIndexData::Clear( void )
	{
		for( TMapIt it = m_Map.begin();
			it != m_Map.end();
			++it )
		{
			SFile* pFile = &(it->second);

			if( pFile->pData != NULL ) free( pFile->pData );
		}

		m_Map.clear();
	}

	ERet		CIndexData::ReadData( CHAR* pData_, DWORD Size_ )
	{
		// rso temp WriteData, ReadData 의  기록 순서는 enum 과 그와 연결된 함수로써 해결하는게 가장 이상적인가??
		ERet		Ret = Ret_Ok;
		DWORD		dwReadSize = 0;

		bool	bBrokenIndex = true;
		for( ; ; )
		{
			// Load CheckSum
			TCheckSum FileCheckSum = 0;
			if( (Size_ - dwReadSize) < sizeof(FileCheckSum) ) break;
			CopyMemory( &FileCheckSum, &pData_[dwReadSize], sizeof(FileCheckSum) );
			dwReadSize += sizeof( FileCheckSum );


			// Load PathLen
			if( (Size_ - dwReadSize) < sizeof(TFilePath::TSize) ) break;
			TFilePath::TSize	PathLen = * ( (TFilePath::TSize*)&pData_[dwReadSize] );


			if( PathLen > TFilePath::Cnt )
			{
				Ret = Ret_InvalidPathLen;
				goto LABEL_ERROR;
			}
			dwReadSize += sizeof(TFilePath::TSize);

			// Load Path
			DWORD		PathSize = PathLen * sizeof(TCHAR);
			if( (Size_ - dwReadSize) < PathSize ) break;

			TFilePath	Path;
			CopyMemory( Path.GetPtr(), &pData_[dwReadSize], PathSize );
			dwReadSize += PathSize;


			SFile*		pAddedFile = NULL;
			Ret = AddCore( Path, pAddedFile );
			if( Ret != Ret_Ok ) goto LABEL_ERROR;

			pAddedFile->CheckSum = FileCheckSum;

			// Load PtrCnt
			TBlockCnt PtrCnt = 0;
			if( (Size_ - dwReadSize) < sizeof(PtrCnt) ) break;
			CopyMemory( &PtrCnt, &pData_[dwReadSize], sizeof(PtrCnt) );
			dwReadSize += sizeof( PtrCnt );

			for( TBlockCnt cnt=0; cnt<PtrCnt; ++cnt )
			{
				SBlock Block;
				CopyMemory( &Block, &pData_[dwReadSize], sizeof(Block) );
				PushBlock( pAddedFile, &Block );
				dwReadSize += sizeof(Block);

				ERet SRet = m_FileMgr.Add( &Block );
				if( SRet != Ret_Ok )
				{
					Ret = Ret_FileMgrAddFail;
					goto LABEL_ERROR;
				}

				m_FreeMgr.Del( &Block );
			}

			if( dwReadSize == Size_ )
			{
				bBrokenIndex = false;
				break;
			}
		}

		if( bBrokenIndex == true )
		{
			Ret = Ret_BrokenIndex;
			goto LABEL_ERROR;
		}

		CHAR* pTmpData = (CHAR*)realloc( m_pData, Size_ );
		if( pTmpData == NULL )
		{
			Ret = Ret_NotEnoughMemory;
			goto LABEL_ERROR;
		}
		m_pData = pTmpData;
		CopyMemory( m_pData, pData_, Size_ );

		m_DataSize = Size_;

		return Ret;

	LABEL_ERROR:

		for( TMapIt it = m_Map.begin();
			it != m_Map.end();
			++it )
		{
			SFile* pFile = &(it->second);
			for( TBlockCnt cnt = 0; cnt < pFile->Cnt; ++cnt )
			{
				m_FreeMgr.Add( &pFile->pData[cnt] );
				m_FileMgr.Del( &pFile->pData[cnt] );
			}
		}

		Clear();

		return Ret;
	}

	ERet		CIndexData::WriteData( void )
	{
		// rso temp WriteData, ReadData 의  기록 순서는 enum 과 그와 연결된 함수로써 해결하는게 가장 이상적인가??
		ERet	Ret = Ret_Ok;
		DWORD	dwAddedSize = 0;
		DWORD	dwIndexPtr = 0;
		CHAR*	pTmpBuf = NULL;
		CHAR*	pTempData = NULL;


		for( TMapIt it = m_Map.begin();
			it != m_Map.end();
			++it )
		{
			TCheckSum CheckSum = (it->second).CheckSum;
			dwAddedSize = sizeof( CheckSum );
			if( dwIndexPtr + dwAddedSize < dwIndexPtr ){ Ret = Ret_SizeOverFlow; goto LABEL_ERROR; }

			pTmpBuf = (CHAR*)realloc( pTempData, dwIndexPtr+dwAddedSize );
			if( pTmpBuf == NULL ){ Ret = Ret_NotEnoughMemory; goto LABEL_ERROR; }
			pTempData = pTmpBuf;
			CopyMemory( &pTempData[dwIndexPtr], &CheckSum, dwAddedSize );
			dwIndexPtr += dwAddedSize;


			TFilePath::TSize	PathLen = (TFilePath::TSize)it->first.length();
			dwAddedSize = sizeof(PathLen);
			if( dwIndexPtr + dwAddedSize < dwIndexPtr ){ Ret = Ret_SizeOverFlow; goto LABEL_ERROR; }

			pTmpBuf = (CHAR*)realloc( pTempData, dwIndexPtr+dwAddedSize );
			if( pTmpBuf == NULL ){ Ret = Ret_NotEnoughMemory; goto LABEL_ERROR; }
			pTempData = pTmpBuf;
			CopyMemory( &pTempData[dwIndexPtr], &PathLen, dwAddedSize );
			dwIndexPtr += dwAddedSize;


			dwAddedSize = sizeof(TCHAR) * PathLen;
			if( dwIndexPtr + dwAddedSize < dwIndexPtr ){ Ret = Ret_SizeOverFlow; goto LABEL_ERROR; }

			pTmpBuf = (CHAR*)realloc( pTempData, dwIndexPtr+dwAddedSize );
			if( pTmpBuf == NULL ){ Ret = Ret_NotEnoughMemory; goto LABEL_ERROR; }

			pTempData = pTmpBuf;
			CopyMemory( &pTempData[dwIndexPtr], (it->first).c_str(), dwAddedSize );
			dwIndexPtr += dwAddedSize;


			dwAddedSize = sizeof((it->second).Cnt);
			if( dwIndexPtr + dwAddedSize < dwIndexPtr ){ Ret = Ret_SizeOverFlow; goto LABEL_ERROR; }

			pTmpBuf = (CHAR*)realloc( pTempData, dwIndexPtr+ dwAddedSize );
			if( pTmpBuf == NULL ){ Ret = Ret_NotEnoughMemory; goto LABEL_ERROR; }

			pTempData = pTmpBuf;
			CopyMemory( &pTempData[dwIndexPtr], &((it->second).Cnt), dwAddedSize );
			dwIndexPtr += dwAddedSize;


			dwAddedSize = sizeof(SBlock)*(it->second).Cnt;
			if( dwIndexPtr + dwAddedSize < dwIndexPtr ){ Ret = Ret_SizeOverFlow; goto LABEL_ERROR; }

			pTmpBuf = (CHAR*)realloc( pTempData, dwIndexPtr+dwAddedSize );
			if( pTmpBuf == NULL ){ Ret = Ret_NotEnoughMemory; goto LABEL_ERROR; }

			pTempData = pTmpBuf;
			CopyMemory( &pTempData[dwIndexPtr], (it->second).pData, dwAddedSize );
			dwIndexPtr += dwAddedSize;
		}


		// Replace New Buffer
		if( m_pData != NULL ) free( m_pData );

		m_DataSize = dwIndexPtr;
		m_pData = pTempData;

		return Ret;

	LABEL_ERROR:

		if( pTempData != NULL ) free( pTempData );

		return Ret;
	}

	SFile*		CIndexData::GetFile( const TFilePath& FilePath_ )
	{
		TFilePath	FilePath = FilePath_;

		PathDelimToSlash( FilePath );

		TMapIt	it = m_Map.find( FilePath.GetPtr() );
		if( it == m_Map.end() ) return NULL;

		return &(it->second);
	}

	TFileSize						CIndexData::GetDataOccupySize( void )
	{
		if( m_FileMgr.Size() <= 0 ) return 0;

		TSpcMapIt	it = m_FileMgr.End();

		--it;

		return ( (it->first) + (it->second).Size );
	}
};

NS_END
