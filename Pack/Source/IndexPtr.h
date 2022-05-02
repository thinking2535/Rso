/************************************************************************/
/* PREPROCESS                                                           */
/************************************************************************/
#pragma once



/************************************************************************/
/* INCLUDES                                                             */
/************************************************************************/
#include "Base.h"



/************************************************************************/
/* DEFINITIONS                                                          */
/************************************************************************/


/************************************************************************/
/* CLASS                                                                */
/************************************************************************/
NS_BEGIN

namespace pack
{
	class	CIndexPtr
	{
	private:
		HANDLE			m_hFile;
		SPtr			m_Ptr;
		void			ClearPtr( void );

	public:
		CIndexPtr();
		~CIndexPtr();

		ERet			Initialize( HANDLE hFile_ );
		void			Finalize( void );
		UFileSize		Get( void );
		void			Set( TFileSize Size_ );

		DWORD			GetDataSize( void ){ return sizeof(m_Ptr); }
		CHAR*			GetDataPtr( void ){ return (CHAR*)&m_Ptr; }
	};
};

NS_END
