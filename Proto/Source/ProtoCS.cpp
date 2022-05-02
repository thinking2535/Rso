#include "ProtoCS.h"


namespace rso
{
	namespace proto
	{
		CProtoBase& CProtoCS::operator << (const wstring& Str_)
		{
			if (_IncludedCnt <= 0)
				_NameSpaceStack.top().String += Str_;

			return *this;
		}
		void CProtoCS::Close(void)
		{
			_NameSpaceStack.top().Close(_Indentation);
			_TempFS << _NameSpaceStack.top().String;
			_NameSpaceStack.pop();
		}
		void CProtoCS::InsertMember(const wstring& VarName_)
		{
			_pProtoStruct->AddMember(make_shared<_SMember>(_pType, VarName_));
		}
		void CProtoCS::CreateStruct(const _STypeInfo& StructInfo_)
		{
			_pProtoStruct = make_shared<_CStruct>(StructInfo_, CurrentFullNameSpace());
		}
		TTypePtr CProtoCS::InsertDefine(const wstring& Name_, const wstring& NameSpace_, TTypePtr Type_)
		{
			return InsertType(Name_, make_shared<_CDefine>(NameSpace_, Name_, Type_));
		}
		wstring CProtoCS::CurrentFullNameSpace(void) const
		{
			auto Names = _NameSpace.CurrentFullName();

			wstring CurrentFullName;
			for (auto it = Names.begin();
				it != Names.end();
				++it)
			{
				CurrentFullName += (*it + L".");
			}

			return CurrentFullName;
		}
		void CProtoCS::WriteInclude(const wstring& /*Str_*/)
		{
		}
		void CProtoCS::InsertEnum(const wstring& Name_, TTypePtr BaseType_)
		{
			InsertType(Name_, make_shared<_CEnum>(Name_, CurrentFullNameSpace(), BaseType_));
		}
		void CProtoCS::WriteNameSpaceOpen(void)
		{
			if (_IncludedCnt > 0)
				return;

			_NameSpaceStack.emplace(_SNameSpace());

			// base 가 C# 에서는 키워드 이므로 변경
			auto NameSpace = _NameSpace.CurrentName();
			if (NameSpace == L"base")
				NameSpace = L"Base";

			_NameSpaceStack.top().String += (_Indentation + L"namespace " + NameSpace + L"\n");
			_NameSpaceStack.top().String += (_Indentation + L"{\n");

			_Indentation.push_back(L'\t');
		}
		void CProtoCS::WriteNameSpaceClose(void)
		{
			if (_IncludedCnt > 0)
				return;

			_NameSpaceStack.top().Close(_Indentation);
			_Indentation.pop_back();
			_NameSpaceStack.top().String += (_Indentation + L"}\n");

			auto Top = _NameSpaceStack.top();
			_NameSpaceStack.pop();

			_NameSpaceStack.top().String += Top.String;
		}
		TTypePtr CProtoCS::GetTemplateStructType(TTypePtr pBaseType_, const list<TTypePtr>& ParameterTypes_) const
		{
			return make_shared<_CTemplateStruct>(pBaseType_, ParameterTypes_);
		}
		void CProtoCS::WriteHardCode(const wstring& Lang_, const wstring& Code_)
		{
			if (Lang_ != L"cs")
				return;

			(*this) << Code_ << L"\n";
		}
		void CProtoCS::WriteUsingNameSpace(const wstring& Name_)
		{
			auto Name = Name_;

			ReplaceAll<wstring>(Name, L"::", L".");
			ReplaceAll<wstring>(Name, L"base", L"Base");

			auto Index = Name.find_last_of(L'.');
			if (Index == wstring::npos)
				Index = 0;
			else
				++Index;

			auto LastName = Name.substr(Index, Name.size() - Index);
			if (CNameSpace::SystemNamespace.find(LastName) != CNameSpace::SystemNamespace.end())
				return;

			// Full Name 으로 변경
			auto RelativeName = _NameSpace.GetRelativeName(Name_);
			ReplaceAll<wstring>(RelativeName, L"::", L".");
			ReplaceAll<wstring>(RelativeName, L"base", L"Base");

			if (RelativeName.empty())
				THROWEX();

			(*this) << _Indentation;
			(*this) << L"using " << RelativeName << L";\n";
		}
		void CProtoCS::WriteTypedef(TTypePtr pOriType_, const wstring& NewType_)
		{
			_TempFS << pOriType_->UsingStr(NewType_);
		}
		void CProtoCS::WriteEnumName(bool /*HasClass_*/, const wstring& Str_)
		{
			(*this) << L"public enum " << Str_;
		}
		void CProtoCS::WriteEnumData(const wstring& Str_)
		{
			wstring EnumName(Str_);
			ReplaceAll(EnumName, wstring(L"::"), wstring(L"."));

			(*this) << EnumName << L",\n";
		}
		void CProtoCS::WriteEnumClose(void)
		{
		}
		void CProtoCS::WriteInclude(void)
		{
			(*this) << L"using System;\n";
			(*this) << L"using System.Collections.Generic;\n";
			(*this) << L"using rso.core;\n";
			(*this) << L"\n\n";
		}
		void CProtoCS::WriteEnumType(const wstring& Name_)
		{
			(*this) << L" : " << Name_;
		}
		void CProtoCS::WriteConst(TTypePtr Type_, const wstring& Name_, const wstring& Value_)
		{
			if (_IncludedCnt > 0)
				return;

			auto ib = _NameSpaceStack.top().Consts.insert(_TConsts::value_type(Name_, _SConstInfo(Type_, Value_)));
			if (!ib.second)
				THROWEX();
		}
		CProtoCS::CProtoCS(const wstring& IncludePath_, const wstring& SourceFile_, const wstring& LibraryName_) : CProtoBase(IncludePath_, SourceFile_, SourceFile_ + L".cs", LibraryName_)
		{
			_NameSpaceStack.emplace(_SNameSpace());

			_NameSpace.NewType(L"bool", make_shared<_CBool>());
			_NameSpace.NewType(L"int8", make_shared<_CInt8>());
			_NameSpace.NewType(L"uint8", make_shared<_CUInt8>());
			_NameSpace.NewType(L"int16", make_shared<_CInt16>());
			_NameSpace.NewType(L"uint16", make_shared<_CUInt16>());
			_NameSpace.NewType(L"int32", make_shared<_CInt32>());
			_NameSpace.NewType(L"uint32", make_shared<_CUInt32>());
			_NameSpace.NewType(L"int64", make_shared<_CInt64>());
			_NameSpace.NewType(L"uint64", make_shared<_CUInt64>());
			_NameSpace.NewType(L"float", make_shared<_CFloat>());
			_NameSpace.NewType(L"double", make_shared<_CDouble>());
			_NameSpace.NewType(L"string", make_shared<_CString>());
			_NameSpace.NewType(L"wstring", make_shared<_CWString>());
			_NameSpace.NewType(L"u16string", make_shared<_CWString>());
			_NameSpace.NewType(L"time_point", make_shared<_CTimePoint>());
			_NameSpace.NewType(L"datetime", make_shared<_CDateTime>());
			_NameSpace.NewType(L"stream", make_shared<_CStream>());

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
