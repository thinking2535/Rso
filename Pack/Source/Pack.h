/*

2010/06/30
rso


파일 기록방식
1. 파일 공간은 자투리를 사용하여 기록.
	M : 
	D : 알고리즘 복잡, 단편화로 인한 속도 저하(미비?)
	R : 빈공간 찾는 알고리즘, 

2. 모자라는 공간은 쓰지 않음.
	M : 
	D : 공간 낭비


인덱스 수정 안전장치
1. 이중 저장
	M : 
	D : 용량이 달라지면서 
	

2. 안전한 수정절차
	M : 
	D : 

* 인덱스 수정절차
	1. 기존인덱스 포인터에서 새로운 인덱스크기*2 만큼되게 파일크기 조정
	2. 기존인덱스 포인터에서 새로운 인덱스 크기만큼 증가하여 새로운 인덱스 기록
	3. 인덱스 포인터 후자로 변경
	4. 기존인덱스 포인터에 새로운인덱스 기록
	5. 인덱스 포인터 전자로 변경
	6, 새로운 인덱스 크기만큼 파일크기 줄임


Add 시 파일 뒤에 붙으며, 인덱스는 파일 끝에 붙는다.
인덱스는 오류로 인한 파일 깨짐을 방지하기위해 2개를 가지며, Active Index로 Active 를 가리킨다.
파일이 Del되면 파일 크기는 줄지 않고, 해당 공간만 사용하지 않게되며
파일이 Add되면 앞에서부터 빈 공간을 찾아 여유가 있으면 공간에 채워넣고, 그렇지 않으면 뒤에 붙는다.

* Add, Del 루틴은 중간에 다운 되더라도 안전할 수 있도록 한다.
 - Add 절차 -
	1) 새로 추가될 경우
		1. 파일 실제 공간을 늘린다
		3. 디스크에 해당 파일을 쓴다.
		4. 인덱스에 추가한다.

	2) 기존 파일에 덮을 경우 ( 새로운 파일이 같거나 작은 경우 )
		1. 기존 공간을 찾는다.
		2. 디스크에 해당 파일을 쓴다.
		3. 인덱스를 수정한다.

	3) 기존 파일에 덮을 경우 ( 새로운 파일이 큰 경우 )
		1. 인덱스에서 
		2. 기존 공간을 찾는다.
		3. 디스크에 해당 파일을 쓴다.
		4. 인덱스를 수정한다.

	4) 삭제되는 경우
		1. 

		Optimize 할 경우 전체 파일을 새로 써 빈 공간을 제거한다.


< Questions >
1. Optimze는 언제 호출할 것인가???



!!!!!!!!!!!!!
1. 인덱스의 CRC 에러일 경우 파일 버림
2. 내부 파일의 CRC에러일 경우 해당파일을 지우고, 내부파일의 CRC 로 전체 CRC를 재 설정
3. 내부 파일 Read 시에 파일CRC를 체크함
4. 
*/


/*
	파일 구조
	|	인덱스포인터	|	데이터			|		헤더				    |					파일 데이터						|
	|	인덱스포인터	|	데이터	|	...	|	인덱스CRC | 인덱스 Size	| PathLen | Path | CRC | PtrCnt | Ptr | Size | ...	|
*/

// File Header Structure
// CheckSum[8] | FileNameLen[4] | FileName[FileNameLen] | BlockCnt[4] | TBlock[BlockCnt]


/************************************************************************/
/* PREPROCESS                                                           */
/************************************************************************/
#pragma once



/************************************************************************/
/* INCLUDES                                                             */
/************************************************************************/
#include "Base.h"
#include "Rso/Base/Crypt.h"
#include "IndexPtr.h"
#include "IndexData.h"




/************************************************************************/
/* DEFINITIONS                                                          */
/************************************************************************/


/************************************************************************/
/* CLASS                                                                */
/************************************************************************/
NS_BEGIN

namespace pack
{
	class CPack
	{
	public:
		typedef	CIndexData::TMap				TDataMap;
		typedef CIndexData::TMapIt				TDataMapIt;

	#pragma pack( push, 1 )
		struct SIndexHeader
		{
			DWORD		Size;
			TCheckSum	CheckSum;

			SIndexHeader()
			{
				Size = 0;
				CheckSum = 0;
			}
		};
	#pragma pack( pop )

	private:
		HANDLE			m_hFile;
		CSpcMgr			m_FileMgr;
		CIndexPtr		m_IndexPtr;
		CIndexData		m_IndexData;
		CCrypt			m_BF;

		ERet			ResizeFile( UFileSize Size_ );
		ERet			LoadIndexPtr( void );
		ERet			LoadIndex( void );
		ERet			SaveIndexPtr( UFileSize Ptr_ );
		ERet			SaveIndex( UFileSize Ptr_, CHAR* pData_, DWORD Size_ );
		ERet			FixIndex( void );

	public:
		CPack();
		~CPack();

		void			SetPasswd( CHAR* pPass_, unsigned int Size_ ){ m_BF.SetPasswd(pPass_, Size_); }
		ERet			Create( const TFilePath& FilePath_ );
		ERet			Open( const TFilePath& FilePath_ );
		void			Close( void );
		ERet			Add( const TFilePath& FilePath_, CHAR* pData_, TFileSize DataSize_ );
		ERet			Del( const TFilePath& FilePath_ );
		TFileSize		GetFileSize( const TFilePath& FilePath_ );
		ERet			GetFileData( const TFilePath& FilePath_, TFileSize Size_, TFileSize& rSize_o, CHAR* pData_o );
		TCheckSum		GetAllFileCheckSum( void );

		TDataMapIt		Begin( void ){ return m_IndexData.Begin(); }
		TDataMapIt		End( void ){ return m_IndexData.End(); }
		SFile*			GetFile( const TFilePath& FilePath_ ){ return m_IndexData.GetFile( FilePath_ ); }

		void			Optimize( void );	// 내부파일 단편화를 제거하여 전체파일을 새로 교체함
	};
};

NS_END
