#pragma once

#include <Rso/Core/Core.h>
#include <functional>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <filesystem>

#pragma comment( lib, "Rso/Proto_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

// TODO : 이중 namespace 내부의 using namespace 는 파싱 에러남

namespace rso
{
    namespace proto
    {
        using namespace std;
		using namespace placeholders;
		using namespace filesystem;
        using namespace core;

        struct IType;
        struct SMember;
        using TTypePtr = shared_ptr<IType>;
        using TMemberPtr = shared_ptr<const SMember>;
        using TMembers = list<TMemberPtr>;
        struct IType
        {
            virtual ~IType() {}
            virtual wstring Name(void) const = 0;
            virtual wstring FullName(void) const
            {
                return Name();
            }
            virtual wstring DeclareStrInClass(wstring& Indentation_, const wstring& DefinedName_, const wstring& VarName_) const
            {
                return DeclareStr(Indentation_, DefinedName_, VarName_);
            }
            virtual wstring DeclareStr(wstring& Indentation_, const wstring& DefinedName_, const wstring& VarName_) const
            {
                return Indentation_ + (DefinedName_.empty() ? Name() : DefinedName_) + L" " + VarName_ + DeclareNewStr(DefinedName_) + L";\n";
            }
            virtual wstring DeclareNewStr(const wstring&) const = 0;
            virtual wstring GetStreamPopStr(const wstring&) const { return wstring(); }
            virtual wstring GetPopFromStreamStr(const wstring&, wstring&, const wstring&) const { return wstring(); }
            virtual wstring GetStreamPushStr(const wstring&, const wstring&) const { return wstring(); }
            virtual wstring GetPushToStreamStr(const wstring&, wstring&, const wstring&) const { return wstring(); }
            virtual wstring AssignStrFromMap(const wstring& ArgName_) const { return L" = " + ArgName_; }
            virtual wstring ExportStrToMap(const wstring& VarName_) const { return VarName_; }
            virtual wstring ElementNewStr(wstring&, const wstring&) const { return wstring(); }
            virtual wstring SetJsonStr(wstring&, const wstring&, const wstring&) const { return wstring(); }
            virtual wstring SetJsonStrWithName(wstring&, const wstring&, const wstring&, const wstring&) const { return wstring(); }
            virtual wstring GetJsonStr(wstring&, const wstring&, const wstring&) const { return wstring(); }
            virtual bool IsStruct(void) const = 0;
            virtual bool IsArray(void) const { return false; }
            virtual bool IsEnum(void) const { return false; }
            virtual wstring String(wstring&) const { return wstring(); }
            virtual wstring UsingStr(const wstring& NewName_) const
            {
                return L"using " + NewName_ + L" = " + FullName() + L";\n";
            }
            virtual bool HasSet(void) const { return false; }
            virtual bool HasMember(void) const { return false; }
            virtual bool HasSuperMember(void) const { return false; }
            virtual void AddMember(TMemberPtr) { THROWEX(); }
            virtual void SetSuper(TTypePtr) { THROWEX(); }
            virtual void CheckTemplateParameter(size_t TemplateParameterSize_) const
            {
                if (TemplateParameterSize_ > 0)
                    THROWEX();
            }
            virtual TTypePtr FindTemplateType(const wstring& /*TypeName_*/) const
            {
                return nullptr;
            }
            virtual wstring SqliteName(void) const { return wstring(); }
            virtual TMembers GetMembers(void) const { return list<TMemberPtr>(); }
            virtual TMembers GetMembersRecursive(void) const { return list<TMemberPtr>(); }
            virtual size_t GetValue(void) const { return 0; }
        };
        struct SType : public IType
        {
            wstring _Name;
            SType(const wstring& Name_) :
                _Name(Name_)
            {
            }
            wstring Name(void) const override { return _Name; }
		};
        struct SNumber : public SType
        {
            size_t Value;
            SNumber(const wstring& Name_) :
                SType(Name_),
                Value(stoi(Name_))
            {
            }
            wstring Name(void) const override { return _Name; }
            wstring DeclareNewStr(const wstring&) const override { return wstring(); }
            wstring GetPopFromStreamStr(const wstring&, wstring&, const wstring&) const override { return wstring(); }
            wstring GetPushToStreamStr(const wstring&, wstring&, const wstring&) const override { return wstring(); }
            bool IsStruct(void) const override { return false; }
            size_t GetValue(void) const override { return Value; }
        };
        struct SMember
        {
            TTypePtr Type;
            wstring VarName;

            SMember(TTypePtr Type_, const wstring& VarName_) :
                Type(Type_), VarName(VarName_)
            {
            }
            virtual ~SMember() {}
			wstring ElementNewStr(wstring& Indentation_, const wstring& VarName_) const
			{
				return Type->ElementNewStr(Indentation_, VarName_);
			}
            virtual wstring DeclareVarStr(wstring& Indentation_) const
            {
                return Type->DeclareStrInClass(Indentation_, L"", VarName);
            }
            wstring GetPopFromStreamStr(wstring& Indentation_, const wstring& StreamName_) const
            {
                return Type->GetPopFromStreamStr(VarName, Indentation_, StreamName_);
            }
            wstring GetPushToStreamStr(wstring& Indentation_, const wstring& StreamName_) const
            {
                return Type->GetPushToStreamStr(VarName, Indentation_, StreamName_);
            }
			wstring SetJsonStr(wstring& Indentation_, const wstring& JsonName_) const
			{
				return Type->SetJsonStr(Indentation_, VarName, JsonName_);
			}
			wstring GetJsonStr(wstring& Indentation_, const wstring& JsonName_) const
			{
				return Type->GetJsonStr(Indentation_, VarName, JsonName_);
			}
            wstring ExportStrToMap(void) const
            {
                return L"'" + VarName + L"' : " + Type->ExportStrToMap(VarName);
            }
            TMembers GetMembersRecursive(void) const
            {
                auto MemberRecursive = Type->GetMembersRecursive();
                if (MemberRecursive.empty())
                    return TMembers{ make_shared<SMember>(Type, VarName) };
                else
                    return MemberRecursive;
            }
		};
        using TMemberNames = set<wstring>;
        struct _STypeInfo
        {
            wstring Name;
            list<wstring> ParameterNames;

            _STypeInfo(const wstring& Name_, const list<wstring>& ParameterNames_) :
                Name(Name_), ParameterNames(ParameterNames_)
            {
            }
            wstring GetFullName(void) const
            {
                wstring Str = Name;

                auto it = ParameterNames.begin();
                if (it != ParameterNames.end())
                {
                    Str += (L"<" + *it);

                    for (++it; it != ParameterNames.end(); ++it)
                        Str += (L", " + *it);

                    Str += L">";
                }

                return Str;
            }
        };

        // prt 내부에서 사용되는 구조체
        template<typename TType>
        struct STemplateStruct : public TType
        {
            TTypePtr pBaseType = nullptr;
            list<TTypePtr> ParameterTypes; // 실제 타입

            STemplateStruct(const wstring& Name_, TTypePtr pBaseType_, const list<TTypePtr>& ParameterTypes_) :
                TType(Name_),
                pBaseType(pBaseType_),
                ParameterTypes(ParameterTypes_)
            {
            }
            wstring DeclareStrInClass(wstring& Indentation_, const wstring& DefinedName_, const wstring& VarName_) const
            {
                return pBaseType->DeclareStrInClass(Indentation_, DefinedName_.empty() ? Name() : DefinedName_, VarName_);
            }
            wstring DeclareStr(wstring& Indentation_, const wstring& DefinedName_, const wstring& VarName_) const { return pBaseType->DeclareStr(Indentation_, DefinedName_.empty() ? Name() : DefinedName_, VarName_); }
            wstring DeclareNewStr(const wstring& DefinedName_) { return pBaseType->DeclareNewStr(DefinedName_.empty() ? Name() : DefinedName_); }
            wstring ElementNewStr(wstring& Indentation_, const wstring& VarName_) const { return pBaseType->ElementNewStr(Indentation_, VarName_); }
            wstring SetJsonStr(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_) const { return pBaseType->SetJsonStr(Indentation_, VarName_, JsonName_); }
            wstring SetJsonStrWithName(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_, const wstring& KeyName_) const { return pBaseType->SetJsonStrWithName(Indentation_, VarName_, JsonName_, KeyName_); }
            wstring GetJsonStr(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_) const { return pBaseType->GetJsonStr(Indentation_, VarName_, JsonName_); }
            bool IsStruct(void) const { return pBaseType->IsStruct(); }
            bool IsArray(void) const { return pBaseType->IsArray(); }
            bool IsEnum(void) const { return pBaseType->IsEnum(); }
            wstring String(wstring& Indentation_) const { return pBaseType->String(Indentation_); }
            bool HasSet(void) const { return pBaseType->HasSet(); }
            bool HasMember(void) const { return pBaseType->HasMember(); }
            bool HasSuperMember(void) const { return pBaseType->HasSuperMember(); }
            void AddMember(TMemberPtr pMember_) { pBaseType->AddMember(pMember_); }
            void SetSuper(TTypePtr pSuper_) { pBaseType->SetSuper(pSuper_); }
            void CheckTemplateParameter(size_t TemplateParameterSize_) const { pBaseType->CheckTemplateParameter(TemplateParameterSize_); }
        };
        // prt 내부에서 정의되는 구조체
        template<typename TType>
        struct SStruct : public TType
        {
            _STypeInfo StructInfo;
            wstring NameSpace;
            map<wstring, shared_ptr<TType>> TemplateTypes;
            TTypePtr pSuper = nullptr;
            TMemberNames MemberNames;
            TMembers Members;

            SStruct(const _STypeInfo& StructInfo_, const wstring& NameSpace_) :
                TType(StructInfo_.Name), StructInfo(StructInfo_), NameSpace(NameSpace_)
            {
                for (auto& i : StructInfo.ParameterNames)
                {
                    if (!TemplateTypes.emplace(i, make_unique<TType>(i)).second)
                        THROWEX();
                }
            }
			bool IsStruct(void) const override
            {
                return true;
            }
            wstring String(wstring& Indentation_) const override
            {
                wstring Str;

                Str += StructStr(Indentation_);
                Str += Indentation_;
                Str += L"{\n";
                Indentation_.push_back(L'\t');

                // Members
                for (auto& Member : Members)
                    Str += Member->DeclareVarStr(Indentation_);

                Str += DefaultConstructorStr(Indentation_);
                Str += CopyConstructorStr(Indentation_);
				Str += MoveConstructorStr(Indentation_);
                Str += ParamCopyConstructorStr(Indentation_);
                Str += ParamMoveConstructorStr(Indentation_);
                Str += StreamConstructorStr(Indentation_);
                Str += SetOperatorStr(Indentation_);
                Str += GetOperatorStr(Indentation_);
                Str += GetMemberMapStr(Indentation_);
                Str += GetToMapStr(Indentation_);
                Str += ToStringStr(Indentation_);
                Str += SetFunctionStr(Indentation_);
				Str += StdNameFunctionStr(Indentation_);
				Str += MemberNameFunctionStr(Indentation_);

				Indentation_.pop_back();
                Str += Indentation_ + StructCloseStr();

				return Str;
            }
            bool HasSet(void) const override { return true; }
            bool HasMember(void) const override { return (HasSuperMember() || Members.size() > 0); }
            bool HasSuperMember(void) const override { return (pSuper && pSuper->HasMember()); }
            void AddMember(TMemberPtr pMember_) override
            {
                if (!MemberNames.emplace(pMember_->VarName).second)
                    THROWEXA(L"AddMember Fail [%s]", pMember_->VarName);

                Members.emplace_back(pMember_);
            }
            void SetSuper(TTypePtr pSuper_) override { pSuper = pSuper_; }
            void CheckTemplateParameter(size_t TemplateParameterSize_) const override
            {
                if (TemplateParameterSize_ != StructInfo.ParameterNames.size())
                    THROWEX();
            }
            TTypePtr FindTemplateType(const wstring& TypeName_) const override
            {
                auto itTemplateType = TemplateTypes.find(TypeName_);
                if (itTemplateType == TemplateTypes.end())
                    return nullptr;

                return itTemplateType->second;
            }
            TMembers GetMembers(void) const override { return Members; }
            TMembers GetMembersRecursive(void) const override
            {
                TMembers MembersRecursive;

                if (pSuper != nullptr)
                {
                    auto m = pSuper->GetMembersRecursive();
                    MembersRecursive.insert(MembersRecursive.end(), m.begin(), m.end());
                }

                for (auto& i : Members)
                {
                    auto m = i->GetMembersRecursive();
                    MembersRecursive.insert(MembersRecursive.end(), m.begin(), m.end());
                }

                return MembersRecursive;
            }
            virtual wstring StructStr(wstring&) const = 0;
            virtual wstring StructCloseStr(void) const = 0;
            virtual wstring DefaultConstructorStr(wstring&) const { return wstring(); }
			virtual wstring CopyConstructorStr(wstring&) const { return wstring(); }
			virtual wstring MoveConstructorStr(wstring&) const { return wstring(); }
            virtual wstring ParamCopyConstructorStr(wstring&) const { return wstring(); }
            virtual wstring ParamMoveConstructorStr(wstring&) const { return wstring(); }
            virtual wstring StreamConstructorStr(wstring&) const { return wstring(); }
            virtual wstring SetOperatorStr(wstring&) const { return wstring(); }
            virtual wstring GetOperatorStr(wstring&) const { return wstring(); }
            virtual wstring GetMemberMapStr(wstring&) const { return wstring(); }
            virtual wstring GetToMapStr(wstring&) const { return wstring(); }
            virtual wstring ToStringStr(wstring&) const { return wstring(); }
            virtual wstring SetFunctionStr(wstring&) const { return wstring(); }
			virtual wstring StdNameFunctionStr(wstring&) const { return wstring(); }
			virtual wstring MemberNameFunctionStr(wstring&) const = 0;
        };
        struct SDefine : public IType
        {
            wstring NameSpace;
            wstring NewName;
            TTypePtr Type = nullptr;

            SDefine(const wstring& NameSpace_, const wstring& NewName_, TTypePtr Type_) :
                NameSpace(NameSpace_), NewName(NewName_), Type(Type_)
            {
            }
            wstring Name(void) const override
            {
                return NewName;
            }
            wstring FullName(void) const override { return NameSpace + Name(); }
			wstring DeclareStrInClass(wstring& Indentation_, const wstring&, const wstring& VarName_) const override
            {
                return Type->DeclareStrInClass(Indentation_, NewName, VarName_);
            }
			wstring DeclareStr(wstring& Indentation_, const wstring&, const wstring& VarName_) const override { return Type->DeclareStr(Indentation_, NewName, VarName_); }
            wstring DeclareNewStr(const wstring&) const override { return Type->DeclareNewStr(NewName); }
            wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override { return Type->GetPopFromStreamStr(VarName_, Indentation_, StreamName_); }
            wstring GetPushToStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override { return Type->GetPushToStreamStr(VarName_, Indentation_, StreamName_); }
			wstring ElementNewStr(wstring& Indentation_, const wstring& VarName_) const { return Type->ElementNewStr(Indentation_, VarName_); }
			wstring SetJsonStr(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_) const override
			{
				return Type->SetJsonStr(Indentation_, VarName_, JsonName_);
			}
			wstring SetJsonStrWithName(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_, const wstring& KeyName_) const override
			{
				return Type->SetJsonStrWithName(Indentation_, VarName_, JsonName_, KeyName_);
			}
			wstring GetJsonStr(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_) const override
			{
				return Type->GetJsonStr(Indentation_, VarName_, JsonName_);
			}
			bool IsStruct(void) const override { return Type->IsStruct(); }
            bool IsArray(void) const override { return Type->IsArray(); }
            wstring String(wstring& Indentation_) const override { return Type->String(Indentation_); }
            wstring UsingStr(const wstring& NewName_) const override { return Type->UsingStr(NewName_); }
            bool HasSet(void) const override { return Type->HasSet(); }
            bool HasMember(void) const override { return Type->HasMember(); }
			bool HasSuperMember(void) const override { return Type->HasSuperMember(); }
		};
	}
}
