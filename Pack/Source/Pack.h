/*

2010/06/30
rso


���� ��Ϲ��
1. ���� ������ �������� ����Ͽ� ���.
	M : 
	D : �˰��� ����, ����ȭ�� ���� �ӵ� ����(�̺�?)
	R : ����� ã�� �˰���, 

2. ���ڶ�� ������ ���� ����.
	M : 
	D : ���� ����


�ε��� ���� ������ġ
1. ���� ����
	M : 
	D : �뷮�� �޶����鼭 
	

2. ������ ��������
	M : 
	D : 

* �ε��� ��������
	1. �����ε��� �����Ϳ��� ���ο� �ε���ũ��*2 ��ŭ�ǰ� ����ũ�� ����
	2. �����ε��� �����Ϳ��� ���ο� �ε��� ũ�⸸ŭ �����Ͽ� ���ο� �ε��� ���
	3. �ε��� ������ ���ڷ� ����
	4. �����ε��� �����Ϳ� ���ο��ε��� ���
	5. �ε��� ������ ���ڷ� ����
	6, ���ο� �ε��� ũ�⸸ŭ ����ũ�� ����


Add �� ���� �ڿ� ������, �ε����� ���� ���� �ٴ´�.
�ε����� ������ ���� ���� ������ �����ϱ����� 2���� ������, Active Index�� Active �� ����Ų��.
������ Del�Ǹ� ���� ũ��� ���� �ʰ�, �ش� ������ ������� �ʰԵǸ�
������ Add�Ǹ� �տ������� �� ������ ã�� ������ ������ ������ ä���ְ�, �׷��� ������ �ڿ� �ٴ´�.

* Add, Del ��ƾ�� �߰��� �ٿ� �Ǵ��� ������ �� �ֵ��� �Ѵ�.
 - Add ���� -
	1) ���� �߰��� ���
		1. ���� ���� ������ �ø���
		3. ��ũ�� �ش� ������ ����.
		4. �ε����� �߰��Ѵ�.

	2) ���� ���Ͽ� ���� ��� ( ���ο� ������ ���ų� ���� ��� )
		1. ���� ������ ã�´�.
		2. ��ũ�� �ش� ������ ����.
		3. �ε����� �����Ѵ�.

	3) ���� ���Ͽ� ���� ��� ( ���ο� ������ ū ��� )
		1. �ε������� 
		2. ���� ������ ã�´�.
		3. ��ũ�� �ش� ������ ����.
		4. �ε����� �����Ѵ�.

	4) �����Ǵ� ���
		1. 

		Optimize �� ��� ��ü ������ ���� �� �� ������ �����Ѵ�.


< Questions >
1. Optimze�� ���� ȣ���� ���ΰ�???



!!!!!!!!!!!!!
1. �ε����� CRC ������ ��� ���� ����
2. ���� ������ CRC������ ��� �ش������� �����, ���������� CRC �� ��ü CRC�� �� ����
3. ���� ���� Read �ÿ� ����CRC�� üũ��
4. 
*/


/*
	���� ����
	|	�ε���������	|	������			|		���				    |					���� ������						|
	|	�ε���������	|	������	|	...	|	�ε���CRC | �ε��� Size	| PathLen | Path | CRC | PtrCnt | Ptr | Size | ...	|
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

		void			Optimize( void );	// �������� ����ȭ�� �����Ͽ� ��ü������ ���� ��ü��
	};
};

NS_END
