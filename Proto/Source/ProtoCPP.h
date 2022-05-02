#pragma once

#include "ProtoBase.h"
#include <sstream>

namespace rso
{
	namespace proto
	{
		using namespace std;

		class CProtoCPP : public CProtoBase
		{
			struct _SType : public SType
			{
				_SType(const wstring& Name_) :
					SType(Name_)
				{
				}
				wstring Name(void) const override
				{
					return _Name;
				}
				wstring DeclareNewStr(const wstring&) const override { return wstring(L"{}"); }
				wstring GetPopFromStreamStr(const wstring&, wstring&, const wstring&) const override { return wstring(); }
				wstring SetJsonStr(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_) const override
				{
					return Indentation_ + JsonName_ + L"[\"" + VarName_ + L"\"] >> " + VarName_ + L";\n";
				}
				wstring GetJsonStr(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_) const override
				{
					return Indentation_ + JsonName_ + L"[\"" + VarName_ + L"\"] = " + VarName_ + L";\n";
				}
				bool IsStruct(void) const override { return false; }
			};
			class _CEnum : public _SType
			{
				TTypePtr _BaseType = nullptr;

			public:
				_CEnum(const wstring& Name_, TTypePtr BasicType_) :
					_SType(Name_), _BaseType(BasicType_)
				{
				}
				bool IsEnum(void) const override { return true; }
			};

			template<typename TType>
			struct _STemplateStruct : public STemplateStruct<TType>
			{
				static wstring GetStructTemplateNames(TTypePtr pBaseType_, const list<TTypePtr>& ParameterTypes_)
				{
					wstring Str = pBaseType_->Name();

					auto it = ParameterTypes_.begin();
					if (it != ParameterTypes_.end())
					{
						Str += (L"<" + (*it)->Name());

						for (++it; it != ParameterTypes_.end(); ++it)
							Str += (L", " + (*it)->Name());

						Str += L">";
					}

					return Str;
				}
				_STemplateStruct(TTypePtr pBaseType_, const list<TTypePtr>& ParameterTypes_) :
					STemplateStruct(
						GetStructTemplateNames(pBaseType_, ParameterTypes_),
						pBaseType_,
						ParameterTypes_)
				{
				}
			};
			class _CStruct : public SStruct<_SType> // Unknown type Template Structure
			{
			public:
				_CStruct(const _STypeInfo& StructInfo_, const wstring& NameSpace_) :
					SStruct(StructInfo_, NameSpace_)
				{
				}
				wstring StdName(wstring& Indentation_) const
				{
					if (!HasSuperMember() && Members.empty())
						return L"L\"\"";

					wstring Str = (L'\n' + Indentation_);

					if (HasSuperMember())
						Str += (L"GetStdName(" + pSuper->Name() + L"())");

					for (auto it = Members.begin();
						it != Members.end();
						++it)
					{
						if (HasSuperMember() || it != Members.begin())
							Str += (L" + L\",\" + \n" + Indentation_);

						Str += (L"GetStdName(" + (*it)->Type->Name() + L"())");
					}

					return Str;
				}
				wstring MemberName(wstring& Indentation_) const
				{
					if (!HasSuperMember() && Members.empty())
						return L"L\"\"";

					wstring Str = (L'\n' + Indentation_);

					if (HasSuperMember())
						Str += (L"GetMemberName(" + pSuper->Name() + L"(), L\"\")");

					for (auto it = Members.begin();
						it != Members.end();
						++it)
					{
						if (HasSuperMember() || it != Members.begin())
							Str += (L" + L\",\" + \n" + Indentation_);

						Str += (L"GetMemberName(" + (*it)->Type->Name() + L"(), L\"" + (*it)->VarName + L"\")");
					}

					return Str;
				}
				wstring StructStr(wstring& Indentation_) const override
				{
					wstring Str;

					auto itParameter = StructInfo.ParameterNames.begin();
					if (itParameter != StructInfo.ParameterNames.end())
					{
						Str += Indentation_;
						Str += L"template<typename ";
						Str += *itParameter;
						for (++itParameter; itParameter != StructInfo.ParameterNames.end(); ++itParameter)
						{
							Str += L", typename ";
							Str += *itParameter;
						}
						Str += L">\n";
					}

					Str += Indentation_;
					Str += L"struct ";
					Str += _Name;
					Str += L" : public ";
					if (pSuper)
						Str += pSuper->Name() + L"\n";
					else
						Str += L"SProto\n";

					return Str;
				}
				wstring DefaultConstructorStr(wstring& Indentation_) const override
				{
					if (!HasMember())
						return wstring();

					wstring Str;

					Str += Indentation_ + _Name + L"()\n";
					Str += Indentation_ + L"{\n";
					Str += Indentation_ + L"}\n";

					return Str;
				}
				wstring ParamCopyConstructorStr(wstring& Indentation_) const override
				{
					if (!HasMember())
						return wstring();

					wstring Str;

					Str += Indentation_;
					Str += _Name + L"(";

					if (HasSuperMember())
						Str += L"const " + pSuper->Name() + L"& Super_";

					for (auto itMember = Members.begin();
						itMember != Members.end();
						++itMember)
					{
						if (!HasSuperMember())
						{
							if (itMember != Members.begin())
								Str += L", ";
						}
						else
						{
							Str += L", ";
						}

						Str += L"const " + (*itMember)->Type->Name() + L"& " + (*itMember)->VarName + L"_";
					}

					Str += L")";

					if (HasSuperMember())
						Str += L" : " + pSuper->Name() + L"(Super_)";

					bool bFirstMember = true;
					for (auto& Member : Members)
					{
						if (!HasSuperMember())
						{
							if (bFirstMember)
								Str += L" : ";
							else
								Str += L", ";
						}
						else
						{
							Str += L", ";
						}

						bFirstMember = false;

						Str += Member->VarName;
						Str += L"(" + Member->VarName + L"_)";
					}

					Str += L"\n";
					Str += Indentation_ + L"{\n";
					Str += Indentation_ + L"}\n";

					return Str;
				}
				wstring ParamMoveConstructorStr(wstring& Indentation_) const override
				{
					if (!HasMember())
						return wstring();

					wstring Str;

					Str += Indentation_;
					Str += _Name + L"(";

					if (HasSuperMember())
						Str += pSuper->Name() + L"&& Super_";

					for (auto itMember = Members.begin();
						itMember != Members.end();
						++itMember)
					{
						if (!HasSuperMember())
						{
							if (itMember != Members.begin())
								Str += L", ";
						}
						else
						{
							Str += L", ";
						}

						Str += (*itMember)->Type->Name() + L"&& " + (*itMember)->VarName + L"_";
					}

					Str += L")";

					if (HasSuperMember())
						Str += L" : " + pSuper->Name() + L"(std::move(Super_))";

					bool bFirstMember = true;
					for (auto& Member : Members)
					{
						if (!HasSuperMember())
						{
							if (bFirstMember)
								Str += L" : ";
							else
								Str += L", ";
						}
						else
						{
							Str += L", ";
						}

						bFirstMember = false;

						Str += Member->VarName;
						Str += L"(std::move(" + Member->VarName + L"_))";
					}

					Str += L"\n";
					Str += Indentation_ + L"{\n";
					Str += Indentation_ + L"}\n";

					return Str;
				}
				wstring SetOperatorStr(wstring& Indentation_) const override
				{
					wstring Str;

					Str += Indentation_;

					if (!HasMember())
						Str += L"void operator << (CStream&) override\n";
					else
						Str += L"void operator << (CStream& Stream_) override\n";

					Str += Indentation_ + L"{\n";

					Indentation_.push_back(L'\t');

					if (HasSuperMember())
					{
						Str += Indentation_;
						Str += pSuper->Name() + L"::operator << (Stream_);\n";
					}

					for (auto& Member : Members)
						Str += (Indentation_ + L"Stream_ >> " + Member->VarName + L";\n");

					Indentation_.pop_back();
					Str += Indentation_;
					Str += L"}\n";


					// Json
					Str += Indentation_;

					if (!HasMember())
						Str += L"void operator << (const Value&) override\n";
					else
						Str += L"void operator << (const Value& Value_) override\n";

					Str += Indentation_ + L"{\n";

					Indentation_.push_back(L'\t');

					if (HasSuperMember())
					{
						Str += Indentation_;
						Str += pSuper->Name() + L"::operator << (Value_);\n";
					}

					for (auto& Member : Members)
						Str += Member->SetJsonStr(Indentation_, L"Value_");

					Indentation_.pop_back();
					Str += Indentation_;
					Str += L"}\n";

					return Str;
				}
				wstring GetOperatorStr(wstring& Indentation_) const override
				{
					wstring Str;

					Str += Indentation_;

					if (!HasMember())
						Str += L"void operator >> (CStream&) const override\n";
					else
						Str += L"void operator >> (CStream& Stream_) const override\n";

					Str += Indentation_ + L"{\n";
					Indentation_.push_back(L'\t');

					if (HasSuperMember())
						Str += Indentation_ + pSuper->Name() + L"::operator >> (Stream_);\n";

					for (auto& Member : Members)
						Str += (Indentation_ + L"Stream_ << " + Member->VarName + L";\n");

					Indentation_.pop_back();
					Str += Indentation_ + L"}\n";


					// Json
					Str += Indentation_;

					if (!HasMember())
						Str += L"void operator >> (Value&) const override\n";
					else
						Str += L"void operator >> (Value& Value_) const override\n";

					Str += Indentation_ + L"{\n";
					Indentation_.push_back(L'\t');

					if (HasSuperMember())
						Str += Indentation_ + pSuper->Name() + L"::operator >> (Value_);\n";

					for (auto& Member : Members)
						Str += Member->GetJsonStr(Indentation_, L"Value_");

					Indentation_.pop_back();
					Str += Indentation_ + L"}\n";

					return Str;
				}
				wstring StdNameFunctionStr(wstring& Indentation_) const override
				{
					wstring Str;

					Str += Indentation_ + L"static wstring StdName(void)\n";
					Str += Indentation_ + L"{\n";
					Indentation_.push_back(L'\t');

					Str += Indentation_ + L"return ";
					Indentation_.push_back(L'\t');
					Str += StdName(Indentation_);
					Indentation_.pop_back();
					Str += L";\n";

					Indentation_.pop_back();
					Str += Indentation_;
					Str += L"}\n";

					return Str;
				}
				wstring MemberNameFunctionStr(wstring& Indentation_) const override
				{
					wstring Str;

					Str += Indentation_ + L"static wstring MemberName(void)\n";
					Str += Indentation_ + L"{\n";
					Indentation_.push_back(L'\t');

					Str += Indentation_ + L"return ";
					Indentation_.push_back(L'\t');
					Str += MemberName(Indentation_);
					Indentation_.pop_back();
					Str += L";\n";

					Indentation_.pop_back();
					Str += Indentation_;
					Str += L"}\n";

					return Str;
				}
				wstring StructCloseStr(void) const override
				{
					return L"};\n";
				}
			};
			class _CTemplateStruct : public _STemplateStruct<_SType> // Known type Template Structure
			{
			public:
				_CTemplateStruct(TTypePtr Type_, const list<TTypePtr>& ParameterTypes_) :
					_STemplateStruct(Type_, ParameterTypes_)
				{
				}
			};
			struct _SContainer : public _SType
			{
				TTypePtr KeyType = nullptr;
				TTypePtr Type = nullptr;

				_SContainer(const wstring& Name_, TTypePtr KeyType_, TTypePtr Type_) :
					_SType(Name_), KeyType(KeyType_), Type(Type_)
				{
				}
			};
			class _CArray : public _SContainer
			{
			public:
				_CArray(TTypePtr FirstType_, TTypePtr SecondType_) :
					_SContainer(L"array<" + FirstType_->Name() + L"," + SecondType_->Name() + L">", nullptr, FirstType_)
				{
					if (SecondType_->GetValue() == 0)
						THROWEX();
				}
				bool IsArray(void) const override { return true; }
			};
			class _CList : public _SContainer
			{
			public:
				_CList(TTypePtr Type_) :
					_SContainer(L"list<" + Type_->Name() + L">", nullptr, Type_)
				{
				}
			};
			class _CVector : public _SContainer
			{
			public:
				_CVector(TTypePtr Type_) :
					_SContainer(L"vector<" + Type_->Name() + L">", nullptr, Type_)
				{
				}
			};
			class _CSet : public _SContainer
			{
			public:
				_CSet(TTypePtr Type_) :
					_SContainer(L"set<" + Type_->Name() + L">", Type_, nullptr)
				{
				}
			};
			class _CMap : public _SContainer
			{
			public:
				_CMap(TTypePtr KeyType_, TTypePtr Type_) :
					_SContainer(L"map<" + KeyType_->Name() + L"," + Type_->Name() + L">", KeyType_, Type_)
				{
				}
			};
			class _CMultiSet : public _SContainer
			{
			public:
				_CMultiSet(TTypePtr Type_) :
					_SContainer(L"multiset<" + Type_->Name() + L">", Type_, nullptr)
				{
				}
			};
			class _CMultiMap : public _SContainer
			{
			public:
				_CMultiMap(TTypePtr KeyType_, TTypePtr Type_) :
					_SContainer(L"multimap<" + KeyType_->Name() + L"," + Type_->Name() + L">", KeyType_, Type_)
				{
				}
			};
			class _CDefine : public SDefine
			{
			public:
				_CDefine(const wstring& NameSpace_, const wstring& NewName_, TTypePtr Type_) :
					SDefine(NameSpace_, NewName_, Type_)
				{
				}
			};
			struct _SMember : public SMember
			{
				_SMember(TTypePtr pType_, const wstring& VarName_) :
					SMember(pType_, VarName_)
				{
				}
			};

			CProtoBase& operator << (const wstring& Str_) override;
			void InsertMember(const wstring& VarName_) override;
			void CreateStruct(const _STypeInfo& StructInfo_) override;
			TTypePtr InsertDefine(const wstring& Name_, const wstring& NameSpace_, TTypePtr Type_) override;
			wstring CurrentFullNameSpace(void) const override;
			void WriteInclude(const wstring& Str_) override;
			void InsertEnum(const wstring& Name_, TTypePtr BaseType_) override;

			void WriteNameSpaceOpen(void) override;
			void WriteNameSpaceClose(void) override;
			TTypePtr GetTemplateStructType(TTypePtr pBaseType_, const list<TTypePtr>& ParameterTypes_) const override;
			void WriteHardCode(const wstring& Lang_, const wstring& Code_) override;
			void WriteUsingNameSpace(const wstring& Name_) override;
			void WriteTypedef(TTypePtr pOriType_, const wstring& NewType_) override;
			void WriteEnumName(bool HasClass_, const wstring& Str_) override;
			void WriteEnumData(const wstring& Str_) override;
			void WriteEnumClose(void) override;
			void WriteInclude(void) override;
			void WriteEnumType(const wstring& Name_) override;
			void WriteConst(TTypePtr Type_, const wstring& Name_, const wstring& Value_) override;

		public:
			CProtoCPP(const wstring& IncludePath_, const wstring& SourceFile_, const wstring& LibraryName_);
		};
	}
}
