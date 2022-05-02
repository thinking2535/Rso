/************************************************************************/
/* PREPROCESS                                                           */
/************************************************************************/

/************************************************************************/
/* INCLUDES                                                             */
/************************************************************************/
#include "IndexPtr.h"


/************************************************************************/
/* PRIVATE FUNCTIONS                                                    */
/************************************************************************/


/************************************************************************/
/* PUBLIC FUNCTIONS                                                     */
/************************************************************************/
NS_BEGIN

namespace pack
{
	void		CIndexPtr::ClearPtr( void )
	{
		for( TSafeIndexCnt cnt=0; cnt<c_SafeIndexPtrCnt; ++cnt )
		{
			new (&m_Ptr.aPtr[cnt]) UFileSize;
		}
	}

	CIndexPtr::CIndexPtr()
	{
		m_hFile = INVALID_HANDLE_VALUE;
	}

	CIndexPtr::~CIndexPtr()
	{
	}

	ERet		CIndexPtr::Initialize( HANDLE hFile_ )
	{
		if( m_hFile != INVALID_HANDLE_VALUE ) return Ret_AlreadyInit;

		m_hFile = hFile_;

		return Ret_Ok;
	}

	void						CIndexPtr::Finalize( void )
	{
		if( m_hFile != INVALID_HANDLE_VALUE ) m_hFile = INVALID_HANDLE_VALUE;

		ClearPtr();
	}

	UFileSize					CIndexPtr::Get( void )
	{
		// Check IndexPtr ( 4개의 값 중, 2개 이상 동일한 값을 유효한 값으로 간주 )
		bool		bOk = false;
		UFileSize	CheckPtr;

		for( TSafeIndexCnt cnt=0; cnt<c_SafeIndexPtrCnt-1; ++cnt )
		{
			CheckPtr = m_Ptr.aPtr[cnt];

			for( TSafeIndexCnt cnt2=cnt+1; cnt2<c_SafeIndexPtrCnt; ++cnt2 )
			{
				if( m_Ptr.aPtr[cnt2].Size == CheckPtr.Size )
				{
					bOk = true;
					break;
				}
			}
		}

		if( bOk == false ) return UFileSize(c_FileSize_Max);

		return CheckPtr;
	}

	void		CIndexPtr::Set( TFileSize Size_ )
	{
		for( TSafeIndexCnt cnt=0; cnt<c_SafeIndexPtrCnt; ++ cnt )
		{
			m_Ptr.aPtr[cnt].Size = Size_;
		}
	}
};

NS_END
