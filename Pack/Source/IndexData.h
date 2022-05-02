/************************************************************************/
/*                                                                      */
/************************************************************************/
#pragma once



/************************************************************************/
/* INCLUDES                                                             */
/************************************************************************/
#include "SpcMgr.h"



/************************************************************************/
/* DEFINITIONS                                                          */
/************************************************************************/


/************************************************************************/
/* CLASS                                                                */
/************************************************************************/
NS_BEGIN

namespace pack
{
	using namespace std;

	class	CIndexData
	{
	public:
		typedef	map<tstring, SFile, less<tstring>>	TMap;	// 파일 조정
		typedef	TMap::iterator						TMapIt;
		typedef	CSpcMgr::TSpcMap					TSpcMap;
		typedef	CSpcMgr::TSpcMapIt					TSpcMapIt;


	private:
		TMap			m_Map;
		DWORD			m_DataSize;
		CHAR*			m_pData;	// 파일에 저장되는 구조로 저장됨
		DWORD			m_HeaderPtr;
		DWORD			m_HeaderSize;
		CSpcMgr	m_FreeMgr;
		CSpcMgr	m_FileMgr;

		ERet			AddCore( const TFilePath& FilePath_, SFile*& rpAddedFile_o );
		void			DelCore( const TFilePath& FilePath_ );
		ERet			PushBlock( SFile* pFile_, SBlock* pBlock_ );


	public:
		CIndexData();
		~CIndexData();

		ERet			Initialize( void );
		void			Finalize( void );

		TMapIt			Begin( void ){ return m_Map.begin(); }
		TMapIt			End( void ){ return m_Map.end(); }
		CHAR*			GetDataPtr( void ){ return m_pData; }
		DWORD			GetDataSize( void ){ return m_DataSize; }
		ERet			Add( const TFilePath& FilePath_, CHAR* pData_, TFileSize Size_, TFileSize& rOverSize_o );
		ERet			Del( const TFilePath& FilePath_ );
		void			Clear( void );
		ERet			ReadData( CHAR* pData_, DWORD Size_ );
		ERet			WriteData( void );
		SFile*			GetFile( const TFilePath& FilePath_ );
		TFileSize		GetDataOccupySize( void );
	};
};

NS_END
