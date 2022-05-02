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
// ������ �߰� ���� �Ǵ� ������ ��������Ʈ�� ��������Ʈ�� ��ø�Ǵ��� �ڵ�üũ
NS_BEGIN

namespace pack
{
	typedef	TFileSize			TCheckSum;

	union UFileSize
	{
		TFileSize	Size;

		struct
		{
			DWORD	Low;
			DWORD	High;
		};

		UFileSize() : Size(0) {}
		UFileSize( TFileSize Size_ ){ Size = Size_; }
		UFileSize( DWORD Low_, DWORD High_ )
		{
			Low = Low_;
			High = High_;
		}

		UFileSize	operator + ( UFileSize& RVal_ )
		{
			return UFileSize(Low+RVal_.Low, High+RVal_.High);
		}
	};

	class CSpcMgr
	{
	public:
		typedef map<TFileSize, UFileSize, less<TFileSize> >	TSpcMap;
		typedef	TSpcMap::iterator							TSpcMapIt;

	private:
		TSpcMap		m_mapSpc;

	public:
		CSpcMgr();
		~CSpcMgr();

		void		Clear( void );

		ERet		Add( SBlock* pBlock_ );
		ERet		Del( SBlock* pBlock_ );
		TSpcMapIt	Begin( void ){ return m_mapSpc.begin(); }
		TSpcMapIt	End( void ){ return m_mapSpc.end(); }
		TSpcMapIt	Find( TFileSize& rPtr_ ){ return m_mapSpc.find(rPtr_); }
		int			Size( void ){ return (int)(m_mapSpc.size()); }
	};
};

NS_END
