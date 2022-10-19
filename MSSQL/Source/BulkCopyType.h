#pragma once

#include "Base.h"

namespace rso::mssql::bulkcopy
{
	struct SType
	{
		LPBYTE pData;
		bool Default;

		SType(const SType&) = delete;
		SType(SType&& Val_) : pData(Val_.pData), Default(Val_.Default)
		{
			Val_.pData = nullptr;
		}
		SType(LPBYTE pData_, bool Default_) : pData(pData_), Default(Default_)
		{
		}
		virtual ~SType()
		{
			delete[] pData;
		}
		virtual DBINT Get_cbData(void) const = 0;
		virtual LPCBYTE Get_pTerm(void) const = 0;
		virtual INT Get_cbTerm(void) const = 0;
		virtual INT Get_eDataType(void) const = 0;
		virtual void SetData(CStream& Stream_) = 0;
	};

	struct STypeInt8 : public SType
	{
		STypeInt8(bool Default_) : SType(new BYTE[1], Default_) {}

		DBINT Get_cbData(void) const override { return Default ? SQL_NULL_DATA : SQL_VARLEN_DATA; }
		LPCBYTE Get_pTerm(void) const override { return NULL; }
		INT Get_cbTerm(void) const override { return 0; }
		INT Get_eDataType(void) const override { return SQLINT1; }
		void SetData(CStream& Stream_) override
		{
			Stream_ >> *(int8*)pData;
		}
	};

	struct STypeInt16 : public SType
	{
		STypeInt16(bool Default_) : SType(new BYTE[2], Default_) {}

		DBINT Get_cbData(void) const override { return Default ? SQL_NULL_DATA : SQL_VARLEN_DATA; }
		LPCBYTE Get_pTerm(void) const override { return NULL; }
		INT Get_cbTerm(void) const override { return 0; }
		INT Get_eDataType(void) const override { return SQLINT2; }
		void SetData(CStream& Stream_) override
		{
			Stream_ >> *(int16*)pData;
		}
	};

	struct STypeInt32 : public SType
	{
		STypeInt32(bool Default_) : SType(new BYTE[4], Default_) {}

		DBINT Get_cbData(void) const override { return Default ? SQL_NULL_DATA : SQL_VARLEN_DATA; }
		LPCBYTE Get_pTerm(void) const override { return NULL; }
		INT Get_cbTerm(void) const override { return 0; }
		INT Get_eDataType(void) const override { return SQLINT4; }
		void SetData(CStream& Stream_) override
		{
			Stream_ >> *(int32*)pData;
		}
	};

	struct STypeInt64 : public SType
	{
		STypeInt64(bool Default_) : SType(new BYTE[8], Default_) {}

		DBINT Get_cbData(void) const override { return Default ? SQL_NULL_DATA : SQL_VARLEN_DATA; }
		LPCBYTE Get_pTerm(void) const override { return NULL; }
		INT Get_cbTerm(void) const override { return 0; }
		INT Get_eDataType(void) const override { return SQLINT8; }
		void SetData(CStream& Stream_) override
		{
			Stream_ >> *(int64*)pData;
		}
	};

	struct STypeFloat : public SType
	{
		STypeFloat(bool Default_) : SType(new BYTE[4], Default_) {}

		DBINT Get_cbData(void) const override { return Default ? SQL_NULL_DATA : SQL_VARLEN_DATA; }
		LPCBYTE Get_pTerm(void) const override { return NULL; }
		INT Get_cbTerm(void) const override { return 0; }
		INT Get_eDataType(void) const override { return SQLFLT4; }
		void SetData(CStream& Stream_) override
		{
			Stream_ >> *(float*)pData;
		}
	};

	struct STypeDouble : public SType
	{
		STypeDouble(bool Default_) : SType(new BYTE[8], Default_) {}

		DBINT Get_cbData(void) const override { return Default ? SQL_NULL_DATA : SQL_VARLEN_DATA; }
		LPCBYTE Get_pTerm(void) const override { return NULL; }
		INT Get_cbTerm(void) const override { return 0; }
		INT Get_eDataType(void) const override { return SQLFLT8; }
		void SetData(CStream& Stream_) override
		{
			Stream_ >> *(double*)pData;
		}
	};

	struct STypeString : public SType
	{
		STypeString(bool Default_) : SType(new BYTE[8001], Default_) {}

		DBINT Get_cbData(void) const override { return Default ? SQL_NULL_DATA : SQL_VARLEN_DATA; }
		LPCBYTE Get_pTerm(void) const override { return (LPBYTE)""; }
		INT Get_cbTerm(void) const override { return 1; }
		INT Get_eDataType(void) const override { return SQLVARCHAR; }
		void SetData(CStream& Stream_) override
		{
			auto Size = Stream_.get_pop<int32>();
			auto Length = Size > 8000 ? 8000 : Size;
			memcpy(pData, Stream_.buff(), Length);
			((char*)pData)[Length] = '\0';
			Stream_.PopSize(Size);
		}
	};

	struct STypeWString : public SType
	{
		STypeWString(bool Default_) : SType(new BYTE[8002], Default_) {}

		DBINT Get_cbData(void) const override { return Default ? SQL_NULL_DATA : SQL_VARLEN_DATA; }
		LPCBYTE Get_pTerm(void) const override { return (LPBYTE)L""; }
		INT Get_cbTerm(void) const override { return 2; }
		INT Get_eDataType(void) const override { return SQLNVARCHAR; }
		void SetData(CStream& Stream_) override
		{
			auto Size = Stream_.get_pop<int32>();
			Size <<= 1;
			auto Length = Size > 8000 ? 8000 : Size;
			memcpy(pData, Stream_.buff(), Length);
			((wchar_t*)pData)[Length >> 1] = L'\0';
			Stream_.PopSize(Size);
		}
	};

	struct STypeDateTime : public SType
	{
		STypeDateTime(bool Default_) : SType(new BYTE[SDateTime::c_StreamSize], Default_) {}

		DBINT Get_cbData(void) const override { return Default ? SQL_NULL_DATA : SQL_VARLEN_DATA; }
		LPCBYTE Get_pTerm(void) const override { return NULL; }
		INT Get_cbTerm(void) const override { return 0; }
		INT Get_eDataType(void) const override { return BCP_TYPE_SQLDATETIME2; }
		void SetData(CStream& Stream_) override
		{
			Stream_ >> *(SDateTime*)pData;
		}
	};
}
