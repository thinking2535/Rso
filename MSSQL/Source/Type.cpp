#include "Type.h"

namespace rso::mssql
{
	CType* GetType(const wstring& TypeName_)
	{
		if (TypeName_ == L"bool" ||
			TypeName_ == L"int8" ||
			TypeName_ == L"uint8")
			return new CTypeNum<int8>();
		else if (TypeName_ == L"int16" ||
			TypeName_ == L"uint16")
			return new CTypeNum<int16>();
		else if (TypeName_ == L"int32" ||
			TypeName_ == L"uint32")
			return new CTypeNum<int32>();
		else if (TypeName_ == L"int64" ||
			TypeName_ == L"uint64" ||
			TypeName_ == L"time_point" ||
			TypeName_ == L"microseconds" ||
			TypeName_ == L"milliseconds" ||
			TypeName_ == L"seconds" ||
			TypeName_ == L"minutes" ||
			TypeName_ == L"hours")
			return new CTypeNum<int64>();
		else if (TypeName_ == L"float")
			return new CTypeNum<float>();
		else if (TypeName_ == L"double")
			return new CTypeNum<double>();
		else if (TypeName_ == L"datetime")
			return new CTypeDateTime();
		else if (TypeName_ == L"stream")
			return new CTypeBinary();
		else if (TypeName_ == L"string")
			return new CTypeString();
		else if (TypeName_ == L"wstring")
			return new CTypeWString();
		else if (TypeName_ == L"u16string")
			return new CTypeWString();
		else
			THROWEXA(L"Invalid Sql Type[%s]", TypeName_);
	}
}