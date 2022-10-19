#include "ProtoCPP.h"

namespace rso
{
	namespace proto
	{
		CProtoBase& CProtoCPP::operator << (const wstring& Str_)
		{
			if (_IncludedCnt <= 0)
				_TempFS << Str_;

			return *this;
		}
		void CProtoCPP::InsertMember(const wstring& VarName_)
		{
			_pProtoStruct->AddMember(make_shared<_SMember>(_pType, VarName_));
		}
		void CProtoCPP::CreateStruct(const _STypeInfo& StructInfo_)
		{
			_pProtoStruct = make_shared<_CStruct>(StructInfo_, CurrentFullNameSpace());
		}
		TTypePtr CProtoCPP::InsertDefine(const wstring& Name_, const wstring& NameSpace_, TTypePtr Type_)
		{
			return InsertType(Name_, make_shared<_CDefine>(NameSpace_, Name_, Type_));
		}
		wstring CProtoCPP::CurrentFullNameSpace(void) const
		{
			auto Names = _NameSpace.CurrentFullName();

			wstring CurrentFullName;
			for (auto it = Names.begin();
				it != Names.end();
				++it)
			{
				CurrentFullName += (*it + L"::");
			}

			return CurrentFullName;
		}
		void CProtoCPP::WriteInclude(const wstring& Str_)
		{
			(*this) << Str_;
		}
		void CProtoCPP::InsertEnum(const wstring& Name_, TTypePtr BaseType_)
		{
			InsertType(Name_, make_shared<_CEnum>(Name_, BaseType_));
		}
		void CProtoCPP::WriteNameSpaceOpen(void)
		{
			(*this) << _Indentation;
			(*this) << L"namespace " << _NameSpace.CurrentName() << L"\n";

			(*this) << _Indentation;
			(*this) << L"{\n";

			_Indentation.push_back(L'\t');
		}
		void CProtoCPP::WriteNameSpaceClose(void)
		{
			_Indentation.pop_back();
			(*this) << _Indentation;
			(*this) << L"}\n";
		}
		TTypePtr CProtoCPP::GetTemplateStructType(TTypePtr pBaseType_, const list<TTypePtr>& ParameterTypes_) const
		{
			return make_shared<_CTemplateStruct>(pBaseType_, ParameterTypes_);
		}
		void CProtoCPP::WriteHardCode(const wstring& Lang_, const wstring& Code_)
		{
			if (Lang_ != L"cpp")
				return;

			(*this) << Code_ << L"\n";
		}

		void CProtoCPP::WriteUsingNameSpace(const wstring& Name_)
		{
			(*this) << _Indentation;
			(*this) << L"using namespace " << Name_ << L";\n";
		}

		void CProtoCPP::WriteTypedef(TTypePtr pOriType_, const wstring& NewType_)
		{
			(*this) << (_Indentation + pOriType_->UsingStr(NewType_));
		}

		void CProtoCPP::WriteEnumName(bool HasClass_, const wstring& Str_)
		{
			if (HasClass_)
				(*this) << L"enum class " << Str_;
			else
				(*this) << L"enum " << Str_;
		}
		void CProtoCPP::WriteEnumData(const wstring& Str_)
		{
			(*this) << Str_ << L",\n";
		}
		void CProtoCPP::WriteEnumClose(void)
		{
			(*this) << L";";
		}

		void CProtoCPP::WriteInclude(void)
		{
			(*this) << L"#pragma once\n\n";
			(*this) << L"#include <string>\n";
			(*this) << L"#include <array>\n";
			(*this) << L"#include <list>\n";
			(*this) << L"#include <vector>\n";
			(*this) << L"#include <set>\n";
			(*this) << L"#include <map>\n";
			(*this) << L"#include <Rso/Core/Stream.h>\n";
			(*this) << L"#include <Rso/Json/Json.h>\n\n";
		}
		void CProtoCPP::WriteEnumType(const wstring& Name_)
		{
			(*this) << L" : " << Name_;
		}
		void CProtoCPP::WriteConst(TTypePtr Type_, const wstring& Name_, const wstring& Value_)
		{
			(*this) << _Indentation;
			(*this) << L"const " << Type_->Name() << L" " << Name_ << L" = " << Value_ << L";\n";
		}
		CProtoCPP::CProtoCPP(const wstring& IncludePath_, const wstring& SourceFile_, const wstring& LibraryName_) : CProtoBase(IncludePath_, SourceFile_, SourceFile_ + L".h", LibraryName_)
		{
			_NameSpace.NewType(L"bool", make_shared<_SType>(L"bool"));
			_NameSpace.NewType(L"int8", make_shared<_SType>(L"int8"));
			_NameSpace.NewType(L"uint8", make_shared<_SType>(L"uint8"));
			_NameSpace.NewType(L"int16", make_shared<_SType>(L"int16"));
			_NameSpace.NewType(L"uint16", make_shared<_SType>(L"uint16"));
			_NameSpace.NewType(L"int32", make_shared<_SType>(L"int32"));
			_NameSpace.NewType(L"uint32", make_shared<_SType>(L"uint32"));
			_NameSpace.NewType(L"int64", make_shared<_SType>(L"int64"));
			_NameSpace.NewType(L"uint64", make_shared<_SType>(L"uint64"));
			_NameSpace.NewType(L"float", make_shared<_SType>(L"float"));
			_NameSpace.NewType(L"double", make_shared<_SType>(L"double"));
			_NameSpace.NewType(L"string", make_shared<_SType>(L"string"));
			_NameSpace.NewType(L"wstring", make_shared<_SType>(L"wstring"));
			_NameSpace.NewType(L"u16string", make_shared<_SType>(L"u16string"));
			_NameSpace.NewType(L"u32string", make_shared<_SType>(L"u32string"));
			_NameSpace.NewType(L"time_point", make_shared<_SType>(L"system_clock::time_point"));
			_NameSpace.NewType(L"microseconds", make_shared<_SType>(L"microseconds"));
			_NameSpace.NewType(L"milliseconds", make_shared<_SType>(L"milliseconds"));
			_NameSpace.NewType(L"seconds", make_shared<_SType>(L"seconds"));
			_NameSpace.NewType(L"minutes", make_shared<_SType>(L"minutes"));
			_NameSpace.NewType(L"hours", make_shared<_SType>(L"hours"));
			_NameSpace.NewType(L"datetime", make_shared<_SType>(L"SDateTime"));
			_NameSpace.NewType(L"stream", make_shared<_SType>(L"CStream"));

			_NameSpace.NewContainer(L"array", [](TTypePtr FirstType_, TTypePtr SecondType_) { return make_shared<_CArray>(FirstType_, SecondType_); });
			_NameSpace.NewContainer(L"list", [](TTypePtr FirstType_, TTypePtr) { return make_shared<_CList>(FirstType_); });
			_NameSpace.NewContainer(L"vector", [](TTypePtr FirstType_, TTypePtr) { return make_shared<_CVector>(FirstType_); });
			_NameSpace.NewContainer(L"set", [](TTypePtr FirstType_, TTypePtr) { return make_shared<_CSet>(FirstType_); });
			_NameSpace.NewContainer(L"map", [](TTypePtr FirstType_, TTypePtr SecondType_) { return make_shared<_CMap>(FirstType_, SecondType_); });
			_NameSpace.NewContainer(L"multiset", [](TTypePtr FirstType_, TTypePtr) { return make_shared<_CMultiSet>(FirstType_); });
			_NameSpace.NewContainer(L"multimap", [](TTypePtr FirstType_, TTypePtr SecondType_) { return make_shared<_CMultiMap>(FirstType_, SecondType_); });
		}
	}
}
