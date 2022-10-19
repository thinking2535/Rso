#include "ProtoDart.h"

namespace rso
{
	namespace proto
	{
		CProtoBase& CProtoDart::operator << (const wstring& Str_)
		{
			if (_IncludedCnt <= 0)
				_TempFS << Str_;

			return *this;
		}
		void CProtoDart::InsertMember(const wstring& VarName_)
		{
			_pProtoStruct->AddMember(make_shared<_SMember>(_pType, VarName_));
		}
		void CProtoDart::CreateStruct(const _STypeInfo& StructInfo_)
		{
			_pProtoStruct = make_shared<_CStruct>(StructInfo_, CurrentFullNameSpace());
		}
		TTypePtr CProtoDart::InsertDefine(const wstring& Name_, const wstring& NameSpace_, TTypePtr Type_)
		{
			return InsertType(Name_, make_shared<_CDefine>(NameSpace_, Name_, Type_));
		}
		wstring CProtoDart::CurrentFullNameSpace(void) const
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
		void CProtoDart::WriteInclude(const wstring& /*Str_*/)
		{
		}
		void CProtoDart::InsertEnum(const wstring& Name_, TTypePtr BaseType_)
		{
			InsertType(Name_, make_shared<_CEnum>(Name_, BaseType_));
		}
		void CProtoDart::WriteNameSpaceOpen(void)
		{
		}
		void CProtoDart::WriteNameSpaceClose(void)
		{
		}
		TTypePtr CProtoDart::GetTemplateStructType(TTypePtr pBaseType_, const list<TTypePtr>& ParameterTypes_) const
		{
			return make_shared<_CTemplateStruct>(pBaseType_, ParameterTypes_);
		}
		void CProtoDart::WriteHardCode(const wstring& /*Lang_*/, const wstring& /*Code_*/)
		{
		}
		void CProtoDart::WriteUsingNameSpace(const wstring& /*Name_*/)
		{
		}
		void CProtoDart::WriteTypedef(TTypePtr /*pOriType_*/, const wstring& /*NewType_*/)
		{
		}

		void CProtoDart::WriteEnumName(bool /*HasClass_*/, const wstring& Str_)
		{
			(*this) << L"enum " << Str_;
		}
		void CProtoDart::WriteEnumData(const wstring& Str_)
		{
			(*this) << Str_ << L",\n";
		}
		void CProtoDart::WriteEnumClose(void)
		{
		}

		void CProtoDart::WriteInclude(void)
		{
			if (!_LibraryName.empty())
				(*this) << L"part of " << _LibraryName << L"; \n\n";
		}
		void CProtoDart::WriteEnumType(const wstring& /*Name_*/)
		{
		}
		void CProtoDart::WriteConst(TTypePtr /*Type_*/, const wstring& /*Name_*/, const wstring& /*Value_*/)
		{
		}
		CProtoDart::CProtoDart(const wstring& IncludePath_, const wstring& SourceFile_, const wstring& LibraryName_) : CProtoBase(IncludePath_, SourceFile_, SourceFile_ + L".dart", LibraryName_)
		{
			_NameSpace.NewType(L"bool", make_shared<_STypeBool>());
			_NameSpace.NewType(L"int32", make_shared<_STypeInt>());
			_NameSpace.NewType(L"int", make_shared<_STypeInt>());
			_NameSpace.NewType(L"double", make_shared<_STypeDouble>());
			_NameSpace.NewType(L"string", make_shared<_STypeString>());
			_NameSpace.NewType(L"time_point", make_shared<_STypeDateTime>());
			_NameSpace.NewType(L"microseconds", make_shared<_STypeDuration>());
			_NameSpace.NewType(L"milliseconds", make_shared<_STypeDuration>());
			_NameSpace.NewType(L"seconds", make_shared<_STypeDuration>());
			_NameSpace.NewType(L"minutes", make_shared<_STypeDuration>());
			_NameSpace.NewType(L"hours", make_shared<_STypeDuration>());

			_NameSpace.NewContainer(L"list", [](TTypePtr FirstType_, TTypePtr) { return make_shared<_CList>(FirstType_); });
			_NameSpace.NewContainer(L"set", [](TTypePtr FirstType_, TTypePtr) { return make_shared<_CSet>(FirstType_); });
			_NameSpace.NewContainer(L"map", [](TTypePtr FirstType_, TTypePtr SecondType_) { return make_shared<_CMap>(FirstType_, SecondType_); });
		}
	}
}
