#pragma once

#include "ProtoBase.h"
#include <sstream>

namespace rso
{
	namespace proto
	{
		using namespace std;

		class CProtoDart : public CProtoBase
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
				wstring DeclareStr(wstring& Indentation_, const wstring& DefinedName_, const wstring& VarName_) const override
				{
					return Indentation_ + L"late " + (DefinedName_.empty() ? Name() : DefinedName_) + L" " + VarName_ + L";\n";
				}
				wstring DeclareNewStr(const wstring&) const override { return wstring(L"{}"); }
				wstring SetJsonStr(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_) const override
				{
					return Indentation_ + JsonName_ + L"[\"" + VarName_ + L"\"] >> " + VarName_ + L";\n";
				}
				wstring GetJsonStr(wstring& Indentation_, const wstring& VarName_, const wstring& JsonName_) const override
				{
					return Indentation_ + JsonName_ + L"[\"" + VarName_ + L"\"] = " + VarName_ + L";\n";
				}
				bool IsStruct(void) const override { return false; }
				wstring SqliteName(void) const override { return wstring(); }
			};
			struct _STypeBool : public _SType
			{
				_STypeBool() :
					_SType(L"bool")
				{
				}
				wstring DeclareNewStr(const wstring&) const override
				{
					return L" = false";
				}
				wstring GetStreamPopStr(const wstring& StreamName_) const override { return StreamName_ + L".popBool()"; }
				wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + VarName_ + L" = " + GetStreamPopStr(StreamName_) + L";\n";
				}
				wstring GetStreamPushStr(const wstring& StreamName_, const wstring& VarName_) const override { return StreamName_ + L".pushBool(" + VarName_ + L")"; }
				wstring GetPushToStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + GetStreamPushStr(StreamName_, VarName_) + L";\n";
				}
				wstring AssignStrFromMap(const wstring& ArgName_) const override
				{
					return L" = " + ArgName_ + L" == 0 ? false : true";
				}
				wstring ExportStrToMap(const wstring& VarName_) const override
				{
					return VarName_ + L" ? 1 : 0";
				}
				wstring SqliteName(void) const override { return L"INTEGER"; }
			};
			struct _STypeInt : public _SType
			{
				_STypeInt() :
					_SType(L"int")
				{
				}
				wstring DeclareNewStr(const wstring&) const override
				{
					return L" = 0";
				}
				wstring GetStreamPopStr(const wstring& StreamName_) const override { return StreamName_ + L".popInt()"; }
				wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + VarName_ + L" = " + GetStreamPopStr(StreamName_) + L";\n";
				}
				wstring GetStreamPushStr(const wstring& StreamName_, const wstring& VarName_) const override { return StreamName_ + L".pushInt(" + VarName_ + L")"; }
				wstring GetPushToStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + GetStreamPushStr(StreamName_, VarName_) + L";\n";
				}
				wstring SqliteName(void) const override { return L"INTEGER"; }
			};
			struct _STypeDouble : public _SType
			{
				_STypeDouble() :
					_SType(L"double")
				{
				}
				wstring DeclareNewStr(const wstring&) const override
				{
					return L" = 0.0";
				}
				wstring GetStreamPopStr(const wstring& StreamName_) const override { return StreamName_ + L".popDouble()"; }
				wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + VarName_ + L" = " + GetStreamPopStr(StreamName_) + L";\n";
				}
				wstring GetStreamPushStr(const wstring& StreamName_, const wstring& VarName_) const override { return StreamName_ + L".pushDouble(" + VarName_ + L")"; }
				wstring GetPushToStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + GetStreamPushStr(StreamName_, VarName_) + L";\n";
				}
				wstring SqliteName(void) const override { return L"REAL"; }
			};
			struct _STypeString : public _SType
			{
				_STypeString() :
					_SType(L"String")
				{
				}
				wstring DeclareNewStr(const wstring&) const override
				{
					return L" = ''";
				}
				wstring GetStreamPopStr(const wstring& StreamName_) const override { return StreamName_ + L".popString()"; }
				wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + VarName_ + L" = " + GetStreamPopStr(StreamName_) + L";\n";
				}
				wstring GetStreamPushStr(const wstring& StreamName_, const wstring& VarName_) const override { return StreamName_ + L".pushString(" + VarName_ + L")"; }
				wstring GetPushToStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + GetStreamPushStr(StreamName_, VarName_) + L";\n";
				}
				wstring SqliteName(void) const override { return L"TEXT"; }
			};
			struct _STypeDateTime : public _SType
			{
				_STypeDateTime() :
					_SType(L"DateTime")
				{
				}
				wstring DeclareNewStr(const wstring&) const override
				{
					return L" = DateTime.zero";
				}
				wstring GetStreamPopStr(const wstring& StreamName_) const override { return StreamName_ + L".popDateTime()"; }
				wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + VarName_ + L" = " + GetStreamPopStr(StreamName_) + L";\n";
				}
				wstring GetStreamPushStr(const wstring& StreamName_, const wstring& VarName_) const override { return StreamName_ + L".pushDateTime(" + VarName_ + L")"; }
				wstring GetPushToStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + GetStreamPushStr(StreamName_, VarName_) + L";\n";
				}
				wstring SqliteName(void) const override { return L"INTEGER"; }
			};
			struct _STypeDuration : public _SType
			{
				_STypeDuration() :
					_SType(L"Duration")
				{
				}
				wstring DeclareNewStr(const wstring&) const override
				{
					return L" = Duration.zero";
				}
				wstring GetStreamPopStr(const wstring& StreamName_) const override { return StreamName_ + L".popDuration()"; }
				wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + VarName_ + L" = " + GetStreamPopStr(StreamName_) + L";\n";
				}
				wstring GetStreamPushStr(const wstring& StreamName_, const wstring& VarName_) const override { return StreamName_ + L".pushDuration(" + VarName_ + L")"; }
				wstring GetPushToStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + GetStreamPushStr(StreamName_, VarName_) + L";\n";
				}
				wstring SqliteName(void) const override { return L"INTEGER"; }
			};
			class _CEnum : public _SType
			{
				TTypePtr _BaseType = nullptr;

			public:
				_CEnum(const wstring& Name_, TTypePtr BasicType_) :
					_SType(Name_), _BaseType(BasicType_)
				{
				}
				wstring DeclareNewStr(const wstring&) const override
				{
					return L" = " + _Name + L".values[0]";
				}
				wstring GetStreamPopStr(const wstring& StreamName_) const override { return _Name + L".values[" + StreamName_ + L".popInt()]"; }
				wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + VarName_ + L" = " + GetStreamPopStr(StreamName_) + L";\n";
				}
				wstring GetStreamPushStr(const wstring& StreamName_, const wstring& VarName_) const override { return StreamName_ + L".pushInt(" + VarName_ + L".index)"; }
				wstring GetPushToStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + GetStreamPushStr(StreamName_, VarName_) + L";\n";
				}
				wstring AssignStrFromMap(const wstring& ArgName_) const override
				{
					return L" = " + _Name + L".values[" + ArgName_ + L"]";
				}
				wstring ExportStrToMap(const wstring& VarName_) const override
				{
					return VarName_ + L".index";
				}
				bool IsEnum(void) const override { return true; }
				wstring SqliteName(void) const override { return _BaseType->SqliteName(); }
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
				wstring StdName(wstring& /*Indentation_*/) const
				{
					return wstring();
				}
				wstring DeclareNewStr(const wstring&) const override { return L" = " + _Name + L"()"; }
				wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + VarName_ + L" = " + _Name + L".popFromSerial(" + StreamName_ + L");\n";
				}
				wstring GetPushToStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return Indentation_ + VarName_ + L".pushToSerial(" + StreamName_ + L");\n";
				}
				wstring StructStr(wstring& Indentation_) const override
				{
					wstring Str;

					Str += Indentation_;
					Str += L"class ";
					Str += StructInfo.GetFullName();
					if (pSuper)
						Str += L" extends " + pSuper->Name() + L"\n";
					else
						Str += L" implements Proto\n";

					return Str;
				}
				wstring DefaultConstructorStr(wstring& Indentation_) const override
				{
					if (!HasMember())
						return wstring();

					wstring Str;

					Str += Indentation_ + _Name + L"() :\n";

					Indentation_.push_back(L'\t');

					for (auto it = Members.begin(); it != Members.end(); ++it)
					{
						if (it != Members.begin())
							Str += L",\n" + Indentation_;
						
						Str += Indentation_ + (*it)->VarName + (*it)->Type->DeclareNewStr(Indentation_);
					}

					Str += L";\n";
					Indentation_.pop_back();

					return Str;
				}
				wstring ParamCopyConstructorStr(wstring& Indentation_) const override // 파라미터 있는 생성자
				{
					if (!HasMember())
						return wstring();

					wstring Str;

					Str += Indentation_;
					Str += _Name + L".fromMembers(";

					if (HasSuperMember())
					{
						auto superMembers = pSuper->GetMembers();
						for (auto it = superMembers.begin(); it != superMembers.end(); ++it)
						{
							if (it != superMembers.begin())
								Str += L", ";

							Str += (*it)->Type->Name() + L" " + (*it)->VarName;
						}
					}

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

						Str += L"this." + (*itMember)->VarName;
					}

					Str += L")";

					if (HasSuperMember())
					{
						Str += L" : super.fromMembers(";

						for (auto& m : pSuper->GetMembers())
							Str += m->VarName + L",";

						Str += L")";
					}

					Str += L";\n";

					return Str;
				}
				wstring ParamMoveConstructorStr(wstring& Indentation_) const override // fromMap 생성자
				{
					if (!HasMember())
						return wstring();

					wstring Str;

					Str += Indentation_ + _Name + L".fromMap(Map<String, dynamic> map)";

					if (pSuper != nullptr)
						Str += L" : super.fromMap(map)";

					Str += L" {\n";

					Indentation_.push_back('\t');

					for (auto itMember = Members.begin();
						itMember != Members.end();
						++itMember)
					{
						if ((*itMember)->Type->IsStruct())
						{
							Str += Indentation_ + (*itMember)->VarName + L" = " + (*itMember)->Type->Name() + L".fromMap(map['" + (*itMember)->VarName + L"']);\n";
						}
						else
						{
							Str += Indentation_ + L"{\n";
							Indentation_.push_back('\t');

							Str += Indentation_ + (*itMember)->VarName + (*itMember)->Type->AssignStrFromMap(L"map['" + (*itMember)->VarName + L"']") + L";\n";

							Indentation_.pop_back();
							Str += Indentation_ + L"}\n";
						}
					}

					Indentation_.pop_back();

					Str += Indentation_ + L"}\n";

					return Str;
				}
				wstring StreamConstructorStr(wstring& Indentation_) const override // popFromStream
				{
					if (!HasMember())
						return wstring();

					wstring Str;

					Str += Indentation_ + _Name + L".popFromSerial(Serial serial)";

					if (pSuper != nullptr)
						Str += L" : super.popFromSerial(serial)";

					Str += L" {\n";

					Indentation_.push_back('\t');

					for (auto itMember = Members.begin();
						itMember != Members.end();
						++itMember)
					{
						Str += (*itMember)->GetPopFromStreamStr(Indentation_, L"serial");
					}

					Indentation_.pop_back();

					Str += Indentation_ + L"}\n";

					return Str;
				}
				wstring SetOperatorStr(wstring& /*Indentation_*/) const override
				{
					return wstring();
				}
				wstring GetOperatorStr(wstring& Indentation_) const override 
				{
					wstring Str;

					Str += Indentation_ + L"@override\n";
					Str += Indentation_ + L"void pushToSerial(Serial serial) {\n";

					Indentation_.push_back(L'\t');

					if (pSuper != nullptr)
						Str += Indentation_ + L"super.pushToSerial(serial);\n";

					for (auto itMember = Members.begin();
						itMember != Members.end();
						++itMember)
					{
						Str += (*itMember)->GetPushToStreamStr(Indentation_, L"serial");
					}

					Indentation_.pop_back();

					Str += Indentation_ + L"}\n";

					return Str;
				}
				wstring GetMemberMapStr(wstring& Indentation_) const override
				{
					wstring Str;

					for (auto& Member : Members)
					{
						if (Member->Type->IsStruct())
							continue;

						Str += Indentation_ + L"Map<String, dynamic> get " + Member->VarName + L"Map {\n";
						Indentation_.push_back(L'\t');

						Str += Indentation_ + L"return {\n";
						Indentation_.push_back(L'\t');
						Str += Indentation_ + Member->ExportStrToMap() + L"\n";
						Indentation_.pop_back();
						Str += Indentation_ + L"};\n";

						Indentation_.pop_back();
						Str += Indentation_ + L"}\n";
					}

					return Str;
				}
				wstring GetToMapStr(wstring& Indentation_) const override
				{
					wstring Str;

					if (pSuper != nullptr)
						Str += Indentation_ + L"@override\n";

					Str += Indentation_ + L"Map<String, dynamic> toMap(){\n";

					Indentation_.push_back(L'\t');

					Str += Indentation_ + L"return {\n";
					Indentation_.push_back(L'\t');

					if (pSuper != nullptr)
						Str += Indentation_ + L"...super.toMap(),\n";

					for (auto& Member : Members)
					{
						Str += Indentation_ + L"...";

						if (Member->Type->IsStruct())
							Str += L"{'" + Member->VarName + L"': " + Member->VarName + L".toMap()},\n";
						else
							Str += Member->VarName + L"Map,\n";
					}

					Indentation_.pop_back();
					Str += Indentation_ + L"};\n";

					Indentation_.pop_back();
					Str += Indentation_ + L"}\n";

					return Str;
				}
				wstring ToStringStr(wstring& Indentation_) const override
				{
					wstring Str;

					Str += Indentation_ + L"@override\n";

					Str += Indentation_ + L"String toString() {\n";

					Indentation_.push_back(L'\t');
					Str += Indentation_ + L"return toMap().toString();\n";
					Indentation_.pop_back();

					Str += Indentation_ + L"}\n";

					return Str;
				}
				wstring StdNameFunctionStr(wstring& Indentation_) const override
				{
					wstring Str;

					// createString
					{
						Str += Indentation_ + L"static String createString()\n";
						Str += Indentation_ + L"{\n";
						Indentation_.push_back(L'\t');

						Str += Indentation_ + L"return '''\n";

						Str += (L"CREATE TABLE ${(" + _Name + L").toString()} (\n");

						auto MembersRecursive = GetMembersRecursive();

						for (auto it = MembersRecursive.begin(); it != MembersRecursive.end(); )
						{
							auto itCheck = it;
							++it; // 마지막 행 알기 위함.

							Str += (L"  " + (*itCheck)->VarName + L" " + (*itCheck)->Type->SqliteName());

							if (itCheck == MembersRecursive.begin())
								Str += L" PRIMARY KEY";
							else
								Str += L" NOT NULL";

							if (it == MembersRecursive.end())
								Str += L"\n";
							else
								Str += L",\n";
						}

						Str += L") WITHOUT ROWID;''';\n";

						Indentation_.pop_back();
						Str += Indentation_;
						Str += L"}\n";
					}

					// createRawString
					{
						Str += Indentation_ + L"static String createStringWithoutPrimaryKey()\n";
						Str += Indentation_ + L"{\n";
						Indentation_.push_back(L'\t');

						Str += Indentation_ + L"return '''\n";

						Str += (L"CREATE TABLE ${(" + _Name + L").toString()} (\n");

						auto MembersRecursive = GetMembersRecursive();

						for (auto it = MembersRecursive.begin(); it != MembersRecursive.end(); )
						{
							auto itCheck = it;
							++it; // 마지막 행 알기 위함.

							Str += (L"  " + (*itCheck)->VarName + L" " + (*itCheck)->Type->SqliteName());

							Str += L" NOT NULL";

							if (it == MembersRecursive.end())
								Str += L"\n";
							else
								Str += L",\n";
						}

						Str += L");''';\n";

						Indentation_.pop_back();
						Str += Indentation_;
						Str += L"}\n";
					}

					return Str;
				}
				wstring MemberNameFunctionStr(wstring& Indentation_) const override
				{
					wstring Str = Indentation_;

					Str += L"static List<String> getProperties() {\n";

					Indentation_.push_back('\t');

					Str += L"return ";

					if (pSuper)
						Str += pSuper->Name() + L".getProperties() + ";

					Str += L"[";

					for (auto it = Members.begin();
						it != Members.end();
						++it)
						Str += L"\'" + (*it)->VarName + L"\',";

					Str += L"];\n";

					Indentation_.pop_back();

					Str += L"}\n";

					return Str;
				}
				wstring StructCloseStr(void) const override
				{
					return L"}\n";
				}
			};
			class _CTemplateStruct : public _STemplateStruct<_SType> // Known type Template Structure
			{
			public:
				_CTemplateStruct(TTypePtr Type_, const list<TTypePtr>& ParameterTypes_) :
					_STemplateStruct(Type_, ParameterTypes_)
				{
				}
				wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return pBaseType->GetPopFromStreamStr(VarName_, Indentation_, StreamName_);
				}
				wstring GetPushToStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					return pBaseType->GetPushToStreamStr(VarName_, Indentation_, StreamName_);
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
			class _CList : public _SContainer
			{
			public:
				_CList(TTypePtr Type_) :
					_SContainer(L"List<" + Type_->Name() + L">", nullptr, Type_)
				{
				}
				wstring DeclareNewStr(const wstring&) const override
				{
					return L" = []";
				}
				wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					wstring Str;
					Str += Indentation_ + L"{\n";
					Indentation_.push_back(L'\t');

					Str += Indentation_ + L"final int length = " + StreamName_ + L".popInt();\n";
					Str += Indentation_ + VarName_ + L" = [for (int i = 0; i < length; ++i) " + Type->GetStreamPopStr(StreamName_) + L"]";

					Indentation_.pop_back();
					Str += Indentation_ + L"}\n";

					return Str;
				}
			};
			class _CSet : public _SContainer
			{
			public:
				_CSet(TTypePtr Type_) :
					_SContainer(L"Set<" + Type_->Name() + L">", Type_, nullptr)
				{
				}
				wstring DeclareNewStr(const wstring&) const override
				{
					return L" = {}";
				}
				wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					wstring Str;
					Str += Indentation_ + L"{\n";
					Indentation_.push_back(L'\t');

					Str += Indentation_ + L"final int length = " + StreamName_ + L".popInt();\n";
					Str += Indentation_ + VarName_ + L" = {for (int i = 0; i < length; ++i) " + KeyType->GetStreamPopStr(StreamName_) + L"}";

					Indentation_.pop_back();
					Str += Indentation_ + L"}\n";

					return Str;
				}
			};
			class _CMap : public _SContainer
			{
			public:
				_CMap(TTypePtr KeyType_, TTypePtr Type_) :
					_SContainer(L"Map<" + KeyType_->Name() + L"," + Type_->Name() + L">", KeyType_, Type_)
				{
				}
				wstring DeclareNewStr(const wstring&) const override
				{
					return L" = {}";
				}
				wstring GetPopFromStreamStr(const wstring& VarName_, wstring& Indentation_, const wstring& StreamName_) const override
				{
					wstring Str;
					Str += Indentation_ + L"{\n";
					Indentation_.push_back(L'\t');

					Str += Indentation_ + L"final int length = " + StreamName_ + L".popInt();\n";
					Str += Indentation_ + VarName_ + L" = {for (int i = 0; i < length; ++i) " + KeyType->GetStreamPopStr(StreamName_) + L": " + Type->GetStreamPopStr(StreamName_) + L"}";

					Indentation_.pop_back();
					Str += Indentation_ + L"}\n";

					return Str;
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
			CProtoDart(const wstring& IncludePath_, const wstring& SourceFile_, const wstring& LibraryName_);
		};
	}
}
