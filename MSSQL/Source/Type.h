#pragma once

#include "Base.h"

namespace rso::mssql
{
	using TReturn = SQLRETURN;
	using TParamNum = SQLUSMALLINT;
	using TParamType = SQLSMALLINT;
	using TSqlCType = SQLSMALLINT;
	using TSqlType = SQLSMALLINT;
	using TColDef = SQLULEN;
	using TScale = SQLSMALLINT;
	using TValueSize = SQLLEN;

	struct SBuffer
	{
		const char* Buffer = nullptr;
		int32 Size = 0;

		SBuffer()
		{
		}
		SBuffer(const char* Buffer_, int32 Size_) :
			Buffer(Buffer_), Size(Size_)
		{
		}
	};

	class CType
	{
	protected:
		TValueSize _ValueSize = 0;

	public:
		CType(TValueSize ValueSize_) : _ValueSize(ValueSize_) {}
		virtual ~CType() {}
		virtual bool IsDAEType(void) const { return false; }
		virtual TReturn Bind(SQLHSTMT STMT_, TParamNum ParamNum_, TParamType ParamType_) = 0;
		virtual TReturn BindPrepare(SQLHSTMT /*STMT_*/, TParamNum /*ParamNum_*/, TParamType /*ParamType_*/) { return SQL_SUCCESS; }
		virtual TReturn BindExecute(SQLHSTMT /*STMT_*/, TParamNum /*ParamNum_*/, TParamType /*ParamType_*/) { return SQL_SUCCESS; }
		virtual void Set(CStream& /*Stream_*/) {}
		virtual void Get(CStream& /*Stream_*/) {}
		virtual void SetBuffer(const SBuffer& /*Buffer_*/) {} // TVP Type 등의 Input의 자료형의 필드들의 일부일 경우 해당 자료형에서 호출됨
		virtual SBuffer GetBuffer(CStream& /*Stream_*/) { return SBuffer(); } // DAE 타입용 SetBuffer 하기 위해 자신의 형식에 맞는 SBuffer를 Stream_에서 추출
		virtual TReturn ParamData(HSTMT /*STMT_*/) { return SQL_SUCCESS; } // 타입 자신이 DAE Type인 경우 구현할것.(TVP타입이라면 자신의 멤버의 SetDAE() 호출할것)
		virtual TReturn GetData(SQLHSTMT /*STMT_*/, SQLUSMALLINT /*ParamNum_*/, CStream& /*Stream_*/) { return SQL_SUCCESS; }
		virtual string GetName(void) const = 0;
	};

	template<typename _TType>
	struct STypeInfo
	{
		STypeInfo()
		{
			static_assert(false, "Invalid MSSQL Type");
		}
	};
	template<>
	struct STypeInfo<int8>
	{
		static const SQLSMALLINT CType = SQL_C_TINYINT;
		static const SQLSMALLINT SqlType = SQL_TINYINT;
	};
	template<>
	struct STypeInfo<int16>
	{
		static const SQLSMALLINT CType = SQL_C_SHORT;
		static const SQLSMALLINT SqlType = SQL_SMALLINT;
	};
	template<>
	struct STypeInfo<int32>
	{
		static const SQLSMALLINT CType = SQL_C_LONG;
		static const SQLSMALLINT SqlType = SQL_INTEGER;
	};
	template<>
	struct STypeInfo<int64>
	{
		static const SQLSMALLINT CType = SQL_C_SBIGINT;
		static const SQLSMALLINT SqlType = SQL_BIGINT;
	};
	template<>
	struct STypeInfo<float>
	{
		static const SQLSMALLINT CType = SQL_C_FLOAT;
		static const SQLSMALLINT SqlType = SQL_REAL;
	};
	template<>
	struct STypeInfo<double>
	{
		static const SQLSMALLINT CType = SQL_C_DOUBLE;
		static const SQLSMALLINT SqlType = SQL_FLOAT;
	};

	template<typename _TType>
	class CTypeNum : public CType
	{
	protected:
		_TType _Value{};

	public:
		CTypeNum() :
			CType(sizeof(_Value))
		{
		}
		TReturn Bind(SQLHSTMT STMT_, TParamNum ParamNum_, TParamType ParamType_) override
		{
			return SQLBindParameter(STMT_, ParamNum_, ParamType_, STypeInfo<_TType>::CType, STypeInfo<_TType>::SqlType, sizeof(_Value), 0, (SQLPOINTER)&_Value, sizeof(_Value), &_ValueSize);
		}
		TReturn BindPrepare(SQLHSTMT STMT_, TParamNum ParamNum_, TParamType ParamType_) override
		{
			return Bind(STMT_, ParamNum_, ParamType_);
		}
		void Set(CStream& Stream_) override
		{
			Stream_ >> _Value;
		}
		void Get(CStream& Stream_) override
		{
			Stream_ << _Value;
		}
		void SetBuffer(const SBuffer& Buffer_) override
		{
			_Value = *((_TType*)Buffer_.Buffer);
		}
		SBuffer GetBuffer(CStream& Stream_) override
		{
			auto Buffer = Stream_.buff();
			Stream_.PopSize(sizeof(_Value));
			return SBuffer(Buffer, sizeof(_Value));
		}
		TReturn GetData(SQLHSTMT STMT_, SQLUSMALLINT ParamNum_, CStream& Stream_) override
		{
			auto Ret = SQLGetData(STMT_, ParamNum_, STypeInfo<_TType>::CType, &_Value, sizeof(_Value), &_ValueSize);
			if (!SQL_SUCCEEDED(Ret))
				return Ret;

			Get(Stream_);

			return SQL_SUCCESS;
		}
		string GetName(void) const override
		{
			switch (sizeof(_Value))
			{
			case 1:
				return "TINYINT";
			case 2:
				return "SMALLINT";
			case 4:
				return "INT";
			case 8:
				return "BIGINT";
			default:
				THROWEX();
			}
		}
	};
	class CTypeDateTime : public CType
	{
	protected:
		char _Value[17];

	public:
		CTypeDateTime() :
			CType(sizeof(_Value))
		{
		}
		TReturn Bind(SQLHSTMT STMT_, TParamNum ParamNum_, TParamType ParamType_) override
		{
			return SQLBindParameter(STMT_, ParamNum_, ParamType_, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, sizeof(_Value), 3, (SQLPOINTER)_Value, SDateTime::c_StreamSize, &_ValueSize);
		}
		TReturn BindPrepare(SQLHSTMT STMT_, TParamNum ParamNum_, TParamType ParamType_) override
		{
			return Bind(STMT_, ParamNum_, ParamType_);
		}
		void Set(CStream& Stream_) override
		{
			Stream_ >> *(SDateTime*)_Value;
		}
		void Get(CStream& Stream_)
		{
			Stream_ << *(SDateTime*)_Value;
		}
		void SetBuffer(const SBuffer& Buffer_) override
		{
			*((SDateTime*)_Value) = *((SDateTime*)Buffer_.Buffer);
		}
		SBuffer GetBuffer(CStream& Stream_) override
		{
			auto Buffer = Stream_.buff();
			SDateTime DateTimeForPop;
			Stream_ >> DateTimeForPop;
			return SBuffer(Buffer, SDateTime::c_StreamSize);
		}
		TReturn GetData(SQLHSTMT STMT_, SQLUSMALLINT ParamNum_, CStream& Stream_) override
		{
			auto Ret = SQLGetData(STMT_, ParamNum_, SQL_C_TYPE_TIMESTAMP, (SQLPOINTER)_Value, SDateTime::c_StreamSize, &_ValueSize);
			if (!SQL_SUCCEEDED(Ret))
				return Ret;

			Get(Stream_);

			return SQL_SUCCESS;
		}
		string GetName(void) const override
		{
			return "DATETIME";
		}
	};
	class CTypeReturn : public CTypeNum<int32>
	{
	public:
		void SetValue(int32 Value_)
		{
			_Value = Value_;
		}
		int32 GetValue(void) const
		{
			return _Value;
		}
	};
	class CTypeBinary : public CType
	{
	protected:
		SBuffer _SetBuffer;
		vector<char> _GetBuffer{ 1 }; // SQLGetData 하기 위해서는 메모리 주소가 필요하므로 최소한의 사이즈(1)을 잡아둠.

	public:
		CTypeBinary() :
			CType(SQL_DATA_AT_EXEC)
		{
		}
		bool IsDAEType(void) const override { return true; }
		TReturn Bind(SQLHSTMT STMT_, TParamNum ParamNum_, TParamType ParamType_) override
		{
			return SQLBindParameter(STMT_, ParamNum_, ParamType_, SQL_C_BINARY, SQL_VARBINARY, 0, 0, NULL, 0, &_ValueSize);
		}
		TReturn BindExecute(SQLHSTMT STMT_, TParamNum ParamNum_, TParamType ParamType_) override
		{
			return Bind(STMT_, ParamNum_, ParamType_);
		}
		void Set(CStream& Stream_) override
		{
			Stream_.Pop(_SetBuffer.Size, _SetBuffer.Buffer);
		}
		void Get(CStream& Stream_) override
		{
			Stream_.Push(int32(_GetBuffer.size()), _GetBuffer.data());
		}
		void SetBuffer(const SBuffer& Buffer_) override
		{
			_SetBuffer = Buffer_;
		}
		SBuffer GetBuffer(CStream& Stream_) override
		{
			int32 Size = 0;
			const char* Buffer = nullptr;
			Stream_.Pop(Size, Buffer);
			return SBuffer(Buffer, Size);
		}
		TReturn ParamData(HSTMT STMT_) override
		{
			auto Ret = SQLPutData(STMT_, (SQLPOINTER)_SetBuffer.Buffer, _SetBuffer.Size);
			if (Ret != SQL_NEED_DATA)
			{
				if (!SQL_SUCCEEDED(Ret))
					return Ret;
			}

			return SQLParamData(STMT_, NULL);
		}
		TReturn GetData(SQLHSTMT STMT_, SQLUSMALLINT ParamNum_, CStream& Stream_) override
		{
			auto Ret = SQLGetData(STMT_, ParamNum_, SQL_C_BINARY, _GetBuffer.data(), 0, &_ValueSize); // 0 으로 설정하여 _ValueSize 변수로 실제 크기를 먼저 받아옴.
			if (!SQL_SUCCEEDED(Ret))
				return Ret;

			if (_ValueSize == SQL_NULL_DATA || _ValueSize == 0)
			{
				_GetBuffer.resize(0);
			}
			else
			{
				_GetBuffer.resize(_ValueSize);
				Ret = SQLGetData(STMT_, ParamNum_, SQL_C_BINARY, _GetBuffer.data(), _ValueSize, &_ValueSize); // 실제데이터 크기 _ValueSize 만큼 재 요청.
				if (!SQL_SUCCEEDED(Ret))
					return Ret;
			}

			Get(Stream_);

			return SQL_SUCCESS;
		}
		string GetName(void) const override
		{
			return "VARBINARY(MAX)";
		}
	};
	class CTypeString : public CTypeBinary
	{
		string GetName(void) const override
		{
			return "VARCHAR(MAX)";
		}
	};
	class CTypeWString : public CTypeBinary
	{
		void Set(CStream& Stream_) override
		{
			Stream_.Pop(_SetBuffer.Size, (const wchar_t*&)_SetBuffer.Buffer);
			_SetBuffer.Size <<= 1;
		}
		void Get(CStream& Stream_) override
		{
			Stream_.Push(int32(_GetBuffer.size() >> 1), (const wchar_t*)_GetBuffer.data());
		}
		SBuffer GetBuffer(CStream& Stream_) override
		{
			int32 Size = 0;
			const char* Buffer = nullptr;
			Stream_.Pop(Size, (const wchar_t*&)Buffer);
			Size <<= 1;
			return SBuffer(Buffer, Size);
		}
		string GetName(void) const override
		{
			return "NVARCHAR(MAX)";
		}
	};
	class CTypeTVP : public CType
	{
		struct _STypeBuffer
		{
			unique_ptr<CType> Type;
			vector<SBuffer> Buffers;

			_STypeBuffer(CType* Type_) :
				Type(Type_)
			{
			}
			void GetBuffer(CStream& Stream_)
			{
				Buffers.emplace_back(Type->GetBuffer(Stream_));
			}
			void SetBuffer(int32 RowNum_)
			{
				Type->SetBuffer(Buffers[RowNum_]);
			}
		};

		bool _Binded = false;
		int32 _RowCount = 0;
		list<_STypeBuffer*> _Fields; // 입력 데이터는 Fields 순서대로 들어오지만 데이터 바인딩 순서는 NormalTypes, DAETypes 순서대로 처리
		list<_STypeBuffer> _NormalTypes;
		list<_STypeBuffer> _DAETypes;

	public:
		CTypeTVP() :
			CType(SQL_DATA_AT_EXEC)
		{
		}
		void AddField(CType* Type_)
		{
			if (Type_->IsDAEType())
			{
				_DAETypes.emplace_back(_STypeBuffer(Type_));
				_Fields.emplace_back(&_DAETypes.back());
			}
			else
			{
				_NormalTypes.emplace_back(_STypeBuffer(Type_));
				_Fields.emplace_back(&_NormalTypes.back());
			}
		}
		TReturn Bind(SQLHSTMT STMT_, TParamNum ParamNum_, TParamType /*ParamType_*/) override
		{
			auto Ret = SQLBindParameter(STMT_, ParamNum_, SQL_PARAM_INPUT, SQL_C_DEFAULT, SQL_SS_TABLE, _RowCount, 0, (SQLPOINTER)1, 0, &_ValueSize);
			if (!SQL_SUCCEEDED(Ret))
				return Ret;

			if (_Binded)
			{
				Ret = SQLBindParameter(STMT_, ParamNum_, SQL_PARAM_INPUT, SQL_C_DEFAULT, SQL_SS_TABLE, _RowCount, 0, (SQLPOINTER)1, 0, &_ValueSize);
				if (!SQL_SUCCEEDED(Ret))
					return Ret;
			}
			else
			{
				_Binded = true;
			}

			Ret = SQLSetStmtAttrW(STMT_, SQL_SOPT_SS_PARAM_FOCUS, (SQLPOINTER)ParamNum_, SQL_IS_INTEGER);
			if (!SQL_SUCCEEDED(Ret))
				return Ret;

			TParamNum ParamNum = 0;
			for (auto& Field : _Fields)
			{
				Ret = Field->Type->Bind(STMT_, ++ParamNum, SQL_PARAM_INPUT);
				if (!SQL_SUCCEEDED(Ret))
					return Ret;
			}

			return SQLSetStmtAttrW(STMT_, SQL_SOPT_SS_PARAM_FOCUS, (SQLPOINTER)0, SQL_IS_INTEGER);
		}
		TReturn BindPrepare(SQLHSTMT /*STMT_*/, TParamNum /*ParamNum_*/, TParamType /*ParamType_*/) override
		{
			_Binded = false;
			return SQL_SUCCESS;
		}
		TReturn BindExecute(SQLHSTMT STMT_, TParamNum ParamNum_, TParamType ParamType_) override
		{
			return Bind(STMT_, ParamNum_, ParamType_);
		}
		void Set(CStream& Stream_) override
		{
			for (auto& Field : _Fields)
				Field->Buffers.clear();

			Stream_ >> _RowCount;

			for (int32 r = 0; r < _RowCount; ++r)
			{
				for (auto& Field : _Fields)
					Field->GetBuffer(Stream_);
			}
		}
		TReturn ParamData(HSTMT STMT_) override
		{
			TReturn Ret = SQL_SUCCESS;

			for (int32 r = 0; r < _RowCount; ++r)
			{
				for (auto& Type : _NormalTypes)
					Type.SetBuffer(r);

				Ret = SQLPutData(STMT_, (SQLPOINTER)1, 1);
				if (Ret != SQL_NEED_DATA)
				{
					if (!SQL_SUCCEEDED(Ret))
						return Ret;
				}

				Ret = SQLParamData(STMT_, NULL);
				if (Ret != SQL_NEED_DATA)
				{
					if (!SQL_SUCCEEDED(Ret))
						return Ret;
				}

				for (auto& Type : _DAETypes)
				{
					Ret = SQLPutData(STMT_, (SQLPOINTER)Type.Buffers[r].Buffer, Type.Buffers[r].Size);
					if (Ret != SQL_NEED_DATA)
					{
						if (!SQL_SUCCEEDED(Ret))
							return Ret;
					}

					Ret = SQLParamData(STMT_, NULL);
					if (Ret != SQL_NEED_DATA)
					{
						if (!SQL_SUCCEEDED(Ret))
							return Ret;
					}
				}
			}

			Ret = SQLPutData(STMT_, NULL, 0);
			if (Ret != SQL_NEED_DATA)
			{
				if (!SQL_SUCCEEDED(Ret))
					return Ret;
			}
			return SQLParamData(STMT_, NULL);
		}
		string GetName(void) const override
		{
			THROWEX();
		}
	};

	class CParam
	{
	protected:
		unique_ptr<CType> _Type;

	public:
		CParam(CType* Type_) :
			_Type(Type_)
		{
		}
		virtual bool IsReturn(void) const { return false; }
		virtual TReturn BindPrepare(HSTMT /*STMT_*/, TParamNum /*ParamNum_*/) { return SQL_SUCCESS; }
		virtual TReturn BindExecute(HSTMT /*STMT_*/, TParamNum /*ParamNum_*/) { return SQL_SUCCESS; }
		virtual void Set(CStream& /*Stream_*/) {}
		virtual void Get(CStream& /*Stream_*/) {}
		virtual TReturn ParamData(HSTMT /*STMT_*/) { return SQL_SUCCESS; }
	};
	class CParamReturn : public CParam
	{
	public:
		CParamReturn() :
			CParam(new CTypeReturn())
		{
		}
		bool IsReturn(void) const { return true; }
		TReturn BindPrepare(HSTMT STMT_, TParamNum ParamNum_) override
		{
			return _Type->BindPrepare(STMT_, ParamNum_, SQL_PARAM_OUTPUT);
		}
		void SetValue(int32 Value_)
		{
			((CTypeReturn*)_Type.get())->SetValue(Value_);
		}
		int32 GetValue(void) const
		{
			return ((CTypeReturn*)_Type.get())->GetValue();
		}
	};
	class CParamInput : public CParam
	{
	public:
		CParamInput(CType* Type_) :
			CParam(Type_)
		{
		}
		TReturn BindPrepare(HSTMT STMT_, TParamNum ParamNum_) override
		{
			return _Type->BindPrepare(STMT_, ParamNum_, SQL_PARAM_INPUT);
		}
		TReturn BindExecute(HSTMT STMT_, TParamNum ParamNum_) override
		{
			return _Type->BindExecute(STMT_, ParamNum_, SQL_PARAM_INPUT);
		}
		void Set(CStream& Stream_) override
		{
			_Type->Set(Stream_);
		}
		TReturn ParamData(HSTMT STMT_) override
		{
			return _Type->ParamData(STMT_);
		}
	};
	class CParamInputOutput : public CParamInput // 코드만 복잡해지므로 현재는 안씀.
	{
	public:
		CParamInputOutput(CType* Type_) :
			CParamInput(Type_)
		{
		}
		TReturn BindPrepare(HSTMT STMT_, TParamNum ParamNum_) override
		{
			return _Type->BindPrepare(STMT_, ParamNum_, SQL_PARAM_INPUT_OUTPUT);
		}
		TReturn BindExecute(HSTMT STMT_, TParamNum ParamNum_) override
		{
			return _Type->BindExecute(STMT_, ParamNum_, SQL_PARAM_INPUT_OUTPUT);
		}
		void Get(CStream& Stream_) override
		{
			_Type->Get(Stream_);
		}
		TReturn ParamData(HSTMT STMT_) override
		{
			return _Type->ParamData(STMT_);
		}
	};
	class CParamTVP : public CParam
	{
	public:
		CParamTVP() :
			CParam(new CTypeTVP())
		{
		}
		TReturn BindPrepare(HSTMT STMT_, TParamNum ParamNum_) override
		{
			return _Type->BindPrepare(STMT_, ParamNum_, SQL_PARAM_INPUT);
		}
		TReturn BindExecute(HSTMT STMT_, TParamNum ParamNum_) override
		{
			return _Type->BindExecute(STMT_, ParamNum_, SQL_PARAM_INPUT);
		}
		void Set(CStream& Stream_) override
		{
			_Type->Set(Stream_);
		}
		TReturn ParamData(HSTMT STMT_) override
		{
			return _Type->ParamData(STMT_);
		}
		void AddField(CType* Type_)
		{
			((CTypeTVP*)_Type.get())->AddField(Type_);
		}
	};
	class CResult
	{
		unique_ptr<CType> _Type;

	public:
		CResult(CType* Type_) :
			_Type(Type_)
		{
		}
		TReturn GetData(SQLHSTMT STMT_, SQLUSMALLINT ParamNum_, CStream& Stream_)
		{
			return _Type->GetData(STMT_, ParamNum_, Stream_);
		}
	};
	using TResult = vector<unique_ptr<CResult>>;
	using TResults = vector<TResult>;

	CType* GetType(const wstring& TypeName_);
}
