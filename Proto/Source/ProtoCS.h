#pragma once


#include "ProtoBase.h"
#include <sstream>
#include <stack>


namespace rso
{
	namespace proto
	{
		using namespace std;

		class CProtoCS : public CProtoBase
		{
			struct _SType : public SType
			{
				_SType(const wstring& Name_) :
					SType(Name_)
				{
				}
				wstring DeclareNewStr(const wstring& NewName_) const override
				{
					return wstring(L" = default(" + (NewName_.empty() ? _Name : NewName_) + L")");
				}
				wstring GetPopFromStreamStr(const wstring&, wstring&, const wstring&) const override { return wstring(); }
				wstring FullName(void) const override
				{
					return L"System." + Name();
				}
				wstring SetJsonStr(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_) const override
				{
					return Indentation_ + JsonName_ + L".Pop(\"" + VarName_ + L"\", ref " + VarName_ + L");\n";
				}
				wstring SetJsonStrWithName(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_, const wstring& KeyName_) const override
				{
					return Indentation_ + JsonName_ + L".Pop(\"" + KeyName_ + L"\", ref " + VarName_ + L");\n";
				}
				wstring GetJsonStr(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_) const override
				{
					return Indentation_ + JsonName_ + L".Push(\"" + VarName_ + L"\", " + VarName_ + L");\n";
				}
				bool IsStruct(void) const override { return false; }
				wstring UsingStr(const wstring& NewName_) const override
				{
					return L"using " + NewName_ + L" = " + FullName() + L";\n";
				}
			};
			class _CBool : public _SType
			{
			public:
				_CBool() :
					_SType(L"Boolean")
				{
				}
			};
			class _CInt8 : public _SType
			{
			public:
				_CInt8() :
					_SType(L"SByte")
				{
				}
			};
			class _CUInt8 : public _SType
			{
			public:
				_CUInt8() :
					_SType(L"Byte")
				{
				}
			};
			class _CInt16 : public _SType
			{
			public:
				_CInt16() :
					_SType(L"Int16")
				{
				}
			};
			class _CUInt16 : public _SType
			{
			public:
				_CUInt16() :
					_SType(L"UInt16")
				{
				}
			};
			class _CInt32 : public _SType
			{
			public:
				_CInt32() :
					_SType(L"Int32")
				{
				}
			};
			class _CUInt32 : public _SType
			{
			public:
				_CUInt32() :
					_SType(L"UInt32")
				{
				}
			};
			class _CInt64 : public _SType
			{
			public:
				_CInt64() :
					_SType(L"Int64")
				{
				}
			};
			class _CUInt64 : public _SType
			{
			public:
				_CUInt64() :
					_SType(L"UInt64")
				{
				}
			};
			class _CFloat : public _SType
			{
			public:
				_CFloat() :
					_SType(L"Single")
				{
				}
			};
			class _CDouble : public _SType
			{
			public:
				_CDouble() :
					_SType(L"Double")
				{
				}
			};
			class _CString : public _SType
			{
			public:
				_CString() :
					_SType(L"String")
				{
				}
				wstring DeclareNewStr(const wstring&) const override { return wstring(L" = string.Empty"); }
			};
			class _CWString : public _SType
			{
			public:
				_CWString() :
					_SType(L"String")
				{
				}
				wstring DeclareNewStr(const wstring&) const override { return wstring(L" = string.Empty"); }
			};
			class _CTimePoint : public _SType
			{
			public:
				_CTimePoint() :
					_SType(L"TimePoint")
				{
				}
				wstring FullName(void) const { return L"rso.core." + Name(); }
			};
			class _CDateTime : public _SType
			{
			public:
				_CDateTime() :
					_SType(L"DateTime")
				{
				}
			};
			class _CStream : public _SType
			{
			public:
				_CStream() :
					_SType(L"CStream")
				{
				}
				wstring DeclareNewStr(const wstring& DefinedName_) const override { return L" = new " + (DefinedName_.empty() ? Name() : DefinedName_) + L"()"; }
				wstring FullName(void) const override { return L"rso.proto." + Name(); }
				bool HasSet(void) const override { return true; }
			};
			class _CEnum : public _SType
			{
				wstring _NameSpace;
				TTypePtr _BaseType = nullptr;

			public:
				_CEnum(const wstring& Name_, const wstring& NameSpace_, TTypePtr BaseType_) :
					_SType(Name_), _NameSpace(NameSpace_), _BaseType(BaseType_)
				{
				}
				wstring FullName(void) const override { return _NameSpace + Name(); }
				bool IsEnum(void) const override { return true; }
			};

			template<typename TType>
			struct _STemplateStruct : public STemplateStruct<TType>
			{
				static wstring GetStructTemplateNames(TTypePtr pBaseType_, const list<TTypePtr>& ParameterTypes_)
				{
					wstring Str = pBaseType_->FullName();

					auto it = ParameterTypes_.begin();
					if (it != ParameterTypes_.end())
					{
						Str += (L"<" + (*it)->FullName());

						for (++it; it != ParameterTypes_.end(); ++it)
							Str += (L", " + (*it)->FullName());

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
				wstring FullName(void) const override
				{
					return Name();
				}
			};
			class _CStruct : public SStruct<_SType>
			{
				wstring _GetDeclareName(void) const
				{
					wstring Str = _Name;

					auto it = StructInfo.ParameterNames.begin();
					if (it != StructInfo.ParameterNames.end())
					{
						Str += L'<';

						Str += *it;

						for (++it; it != StructInfo.ParameterNames.end(); ++it)
						{
							Str += L", ";
							Str += *it;
						}

						Str += L'>';
					}

					return Str;
				}

			public:
				_CStruct(const _STypeInfo& StructInfo_, const wstring& NameSpace_) :
					SStruct(StructInfo_, NameSpace_)
				{
				}
				wstring StdName(wstring& Indentation_) const
				{
					if (!HasSuperMember() && Members.empty())
						return L"\"\"";

					wstring Str = (L'\n' + Indentation_);

					if (HasSuperMember())
						Str += L"base.StdName()";

					for (auto it = Members.begin();
						it != Members.end();
						++it)
					{
						if (HasSuperMember() || it != Members.begin())
							Str += (L" + \",\" + \n" + Indentation_);

						if ((*it)->Type->IsEnum())
							Str += L"\"" + (*it)->Type->FullName() + L"\"";
						else
							Str += (L"SEnumChecker.GetStdName(" + (*it)->VarName + L")");
					}

					return Str;
				}
				wstring MemberName(wstring& Indentation_) const
				{
					if (!HasSuperMember() && Members.empty())
						return L"\"\"";

					wstring Str = (L'\n' + Indentation_);

					if (HasSuperMember())
						Str += L"base.MemberName()";

					for (auto it = Members.begin();
						it != Members.end();
						++it)
					{
						if (HasSuperMember() || it != Members.begin())
							Str += (L" + \",\" + \n" + Indentation_);

						Str += (L"SEnumChecker.GetMemberName(" + (*it)->VarName + L", \"" + (*it)->VarName + L"\")");
					}

					return Str;
				}
				wstring DeclareNewStr(const wstring& DefinedName_) const override
				{
					return L" = new " + (DefinedName_.empty() ? Name() : DefinedName_) + L"()";
				}
				wstring StructStr(wstring& Indentation_) const override
				{
					wstring Str;

					Str += Indentation_;
					Str += L"public class ";
					Str += StructInfo.GetFullName();
					Str += L" : ";
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

					Str += Indentation_;
					Str += L"public " + _Name + L"()\n";
					Str += Indentation_;
					Str += L"{\n";
					Indentation_.push_back(L'\t');

					for (auto& i : Members)
						Str += i->ElementNewStr(Indentation_, i->VarName);

					Indentation_.pop_back();
					Str += Indentation_ + L"}\n";

					return Str;
				}
				wstring CopyConstructorStr(wstring& Indentation_) const override
				{
					if (!HasMember())
						return wstring();

					wstring Str;

					Str += Indentation_ + L"public " + _Name + L"(" + _GetDeclareName() + L" Obj_)";

					if (HasSuperMember())
						Str += L" : base(Obj_)";

					Str += L"\n";
					Str += Indentation_ + L"{\n";

					Indentation_.push_back(L'\t');
					for (auto& Member : Members)
					{
						Str += Indentation_;
						Str += Member->VarName + L" = Obj_." + Member->VarName + L";\n";
					}
					Indentation_.pop_back();

					Str += Indentation_ + L"}\n";

					return Str;
				}
				wstring ParamCopyConstructorStr(wstring& Indentation_) const override
				{
					if (!HasMember())
						return wstring();

					wstring Str;
					Str += Indentation_;
					Str += L"public " + _Name + L"(";

					if (HasSuperMember())
						Str += pSuper->Name() + L" Super_";

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

						Str += (*itMember)->Type->Name() + L" ";
						Str += (*itMember)->VarName + L"_";
					}

					Str += L")";

					if (HasSuperMember())
						Str += L" : base(Super_)";

					Str += L"\n";
					Str += Indentation_ + L"{\n";

					Indentation_.push_back(L'\t');
					for (auto& Member : Members)
						Str += Indentation_ + Member->VarName + L" = " + Member->VarName + L"_;\n";

					Indentation_.pop_back();

					Str += Indentation_;
					Str += L"}\n";

					return Str;
				}
				wstring SetOperatorStr(wstring& Indentation_) const override
				{
					wstring Str;

					Str += Indentation_;
					Str += L"public override void Push(CStream Stream_)\n";
					Str += Indentation_ + L"{\n";
					Indentation_.push_back(L'\t');

					if (HasSuperMember())
						Str += Indentation_ + L"base.Push(Stream_);\n";

					for (auto& Member : Members)
						Str += (Indentation_ + L"Stream_.Pop(ref " + Member->VarName + L");\n");

					Indentation_.pop_back();
					Str += Indentation_ + L"}\n";


					// Json
					Str += Indentation_;
					Str += L"public override void Push(JsonDataObject Value_)\n";
					Str += Indentation_ + L"{\n";
					Indentation_.push_back(L'\t');

					if (HasSuperMember())
						Str += Indentation_ + L"base.Push(Value_);\n";

					for (auto& Member : Members)
						Str += Member->SetJsonStr(Indentation_, L"Value_");

					Indentation_.pop_back();
					Str += Indentation_ + L"}\n";


					return Str;
				}
				wstring GetOperatorStr(wstring& Indentation_) const override
				{
					wstring Str;

					Str += Indentation_;
					Str += L"public override void Pop(CStream Stream_)\n";
					Str += Indentation_ + L"{\n";
					Indentation_.push_back(L'\t');

					if (HasSuperMember())
						Str += Indentation_ + L"base.Pop(Stream_);\n";

					for (auto& Member : Members)
						Str += (Indentation_ + L"Stream_.Push(" + Member->VarName + L");\n");

					Indentation_.pop_back();
					Str += Indentation_ + L"}\n";


					// Json
					Str += Indentation_;
					Str += L"public override void Pop(JsonDataObject Value_)\n";
					Str += Indentation_ + L"{\n";
					Indentation_.push_back(L'\t');

					if (HasSuperMember())
						Str += Indentation_ + L"base.Pop(Value_);\n";

					for (auto& Member : Members)
						Str += Member->GetJsonStr(Indentation_, L"Value_");

					Indentation_.pop_back();
					Str += Indentation_ + L"}\n";


					return Str;
				}
				wstring SetFunctionStr(wstring& Indentation_) const override
				{
					if (!HasMember())
						return wstring();

					wstring Str;
					Str += Indentation_;
					Str += L"public void Set(" + _GetDeclareName() + L" Obj_)\n";
					Str += Indentation_;
					Str += L"{\n";
					Indentation_.push_back(L'\t');

					if (HasSuperMember())
						Str += Indentation_ + L"base.Set(Obj_);\n";

					for (auto& Member : Members)
					{
						Str += Indentation_;

						if (Member->Type->HasSet())
							Str += Member->VarName + L".Set(" + L"Obj_." + Member->VarName + L");\n";
						else
							Str += Member->VarName + L" = Obj_." + Member->VarName + L";\n";
					}
					Indentation_.pop_back();

					Str += Indentation_;
					Str += L"}\n";

					return Str;
				}
				wstring StdNameFunctionStr(wstring& Indentation_) const override
				{
					wstring Str;

					Str += Indentation_ + L"public override string StdName()\n";
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

					Str += Indentation_ + L"public override string MemberName()\n";
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
				}				wstring StructCloseStr(void) const override
				{
					return L"}\n";
				}
				wstring FullName(void) const override { return NameSpace + Name(); }
			};
			class _CTemplateStruct : public _STemplateStruct<_SType>
			{
			public:
				_CTemplateStruct(TTypePtr pType_, const list<TTypePtr>& ParameterTypes_) :
					_STemplateStruct(pType_, ParameterTypes_)
				{
				}
				wstring DeclareNewStr(const wstring& DefinedName_) const override
				{
					return L" = new " + (DefinedName_.empty() ? Name() : DefinedName_) + L"()";
				}
			};

			struct _SContainer : public _SType
			{
				TTypePtr KeyType = nullptr;
				TTypePtr Type = nullptr;

				wstring EmplaceStr(void) const { return wstring(L"Add"); }

				_SContainer(const wstring& Name_, TTypePtr KeyType_, TTypePtr Type_) :
					_SType(Name_), KeyType(KeyType_), Type(Type_)
				{
				}
				wstring DeclareNewStr(const wstring& DefinedName_) const override
				{
					return L" = new " + (DefinedName_.empty() ? Name() : DefinedName_) + L"()";
				}
			};
			class _CArray : public _SContainer
			{
				TTypePtr _SecondType;

			public:
				_CArray(TTypePtr Type_, TTypePtr SecondType_) :
					_SContainer(Type_->Name() + L"[]", nullptr, Type_), _SecondType(SecondType_)
				{
					if (SecondType_->GetValue() == 0)
						THROWEX();
				}
				bool IsArray(void) const override { return true; }
				wstring DeclareStrInClass(wstring& Indentation_, const wstring&, const wstring& VarName_) const override
				{
					return Indentation_ + Name() + L" " + VarName_ + L" = new " + Type->Name() + L"[" + _SecondType->Name() + L"];\n";
				}
				wstring DeclareStr(wstring& Indentation_, const wstring&, const wstring& VarName_) const override
				{
					return DeclareStrInClass(Indentation_, wstring(), VarName_) + ElementNewStr(Indentation_, VarName_);
				}
				wstring ElementNewStr(wstring& Indentation_, const wstring& VarName_) const override
				{
					wstring iName = L"i" + VarName_;
					wstring Str;
					Str += Indentation_ + L"for (int " + iName + L" = 0; " + iName + L" < " + VarName_ + L".Length; ++" + iName + L")\n";
					Indentation_.push_back(L'\t');
					Str += Indentation_ + VarName_ + L"[" + iName + L"]" + Type->DeclareNewStr(Type->Name()) + L";\n";
					Indentation_.pop_back();
					return Str;
				}
				wstring UsingStr(const wstring&) const override
				{
					return wstring();
				}
			};
			class _CList : public _SContainer
			{
			public:
				_CList(TTypePtr Type_) :
					_SContainer(L"List<" + Type_->Name() + L">", nullptr, Type_)
				{
				}
				wstring FullName(void) const override
				{
					return L"System.Collections.Generic.List<" + Type->FullName() + L">";
				}
			};
			class _CVector : public _SContainer
			{
			public:
				_CVector(TTypePtr Type_) :
					_SContainer(L"List<" + Type_->Name() + L">", nullptr, Type_)
				{
				}
				wstring FullName(void) const override
				{
					return L"System.Collections.Generic.List<" + Type->FullName() + L">";
				}
			};
			class _CSet : public _SContainer
			{
			public:
				_CSet(TTypePtr Type_) :
					_SContainer(L"HashSet<" + Type_->Name() + L">", Type_, nullptr)
				{
				}
				wstring FullName(void) const override
				{
					return L"System.Collections.Generic.HashSet<" + KeyType->FullName() + L">";
				}
			};
			class _CMap : public _SContainer
			{
			public:
				_CMap(TTypePtr KeyType_, TTypePtr Type_) :
					_SContainer(L"Dictionary<" + KeyType_->Name() + L"," + Type_->Name() + L">", KeyType_, Type_)
				{
				}
				wstring FullName(void) const override
				{
					return L"System.Collections.Generic.Dictionary<" + KeyType->FullName() + L"," + Type->FullName() + L">";
				}
			};
			class _CMultiSet : public _SContainer
			{
			public:
				_CMultiSet(TTypePtr Type_) :
					_SContainer(L"MultiSet<" + Type_->Name() + L">", Type_, nullptr)
				{
				}
				wstring FullName(void) const override
				{
					return L"rso.core.MultiSet<" + KeyType->FullName() + L">";
				}
			};
			class _CMultiMap : public _SContainer
			{
			public:
				_CMultiMap(TTypePtr KeyType_, TTypePtr Type_) :
					_SContainer(L"MultiMap<" + KeyType_->Name() + L"," + Type_->Name() + L">", KeyType_, Type_)
				{
				}
				wstring FullName(void) const override
				{
					return L"rso.core.MultiMap<" + KeyType->FullName() + L"," + Type->FullName() + L">";
				}
			};
			class _CDefine : public SDefine
			{
			public:
				_CDefine(const wstring& NameSpace_, const wstring& NewName_, TTypePtr Type_) :
					SDefine(NameSpace_, NewName_, Type_)
				{
				}
				wstring Name(void) const override
				{
					if (Type->IsArray())
						return Type->Name();
					else
						return SDefine::Name();
				}
				wstring FullName(void) const override
				{
					return Type->FullName();
				}
			};
			struct _SMember : public SMember
			{
				_SMember(TTypePtr pType_, const wstring& VarName_) :
					SMember(pType_, VarName_)
				{
				}
				wstring DeclareVarStr(wstring& Indentation_) const override
				{
					wstring EmptyIndentation;
					return Indentation_ + L"public " + Type->DeclareStrInClass(EmptyIndentation, L"", VarName);
				}
			};

			struct _SConstInfo
			{
				TTypePtr pType;
				wstring Value;

				_SConstInfo(TTypePtr pType_, const wstring& Value_) :
					pType(pType_), Value(Value_)
				{
				}
			};
			using _TConsts = map<wstring, _SConstInfo>;
			struct _SNameSpace
			{
				wstring String;
				_TConsts Consts;

				void Close(wstring& Indentation_)
				{
					if (Consts.size() > 0)
					{
						String += (Indentation_ + L"public partial class global\n");
						String += (Indentation_ + L"{\n");

						Indentation_.push_back(L'\t');
						for (auto& Const : Consts)
							String += (Indentation_ + L"public const " + Const.second.pType->Name() + L" " + Const.first + L" = " + Const.second.Value + L";\n");
						Indentation_.pop_back();

						String += (Indentation_ + L"}\n");
					}
				}
			};

			stack<_SNameSpace> _NameSpaceStack;

			CProtoBase& operator << (const wstring& Str_) override;
			void Close(void) override;
			void InsertMember(const wstring& VarName_) override;
			void CreateStruct(const _STypeInfo& StructInfo_) override;
			TTypePtr InsertDefine(const wstring& Name_, const wstring& NameSpace_, TTypePtr Type_) override;
			wstring CurrentFullNameSpace(void) const override;
			void WriteInclude(const wstring& Str_) override;
			void InsertEnum(const wstring& Name_, TTypePtr Type_) override;

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
			CProtoCS(const wstring& IncludePath_, const wstring& SourceFile_, const wstring& LibraryName_);
		};
	}
}
