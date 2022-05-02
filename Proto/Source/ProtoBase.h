#pragma once

#include "NameSpace.h"

namespace rso
{
	namespace proto
	{
		// TODO : 컨테이너의 컨테이너 가능하도록 할것.

		class CProtoBase
		{
		protected:
			enum class EMode
			{
				Using, // expect 'namespace' or ???
				UsingNameSpace, // expect NameSpaceName
				NameSpace, // expect 'struct' or 'namespace' or '{' or '}'
				NameSpaceName, // write namespace Name
				NameSpaceOpen, // expect '{'
				StructName, // write struct Name
				StructOpen, // expect '{' or ':'
				SuperName, // expect super class name
				Struct, // expect data types or '}'
				MemberName, // write member name
				EnumBegin, // expect enum class or enum name
				EnumName, // write enum name
				EnumType, // expect 'type'
				EnumOpen, // expect '{'
				Enum, // expect enum items or '}'
				ExEnumBegin, // expect enum class or enum name
				Typedef, // expect Typedef OriName
				TypedefName, // expect Typedef NewName
				IncludeName, // expect file path
				Const, // expect const type
				ConstName, // expect const name
				ConstValue, // expect const value
			};

			const char* c_TempFileSuffix = ".tmp";
			CNameSpace _NameSpace;
			path _IncludePath;
			path _LibraryName;
			int32 _IncludedCnt = 0;
			vector<path> _CurrentPaths;
			TTypePtr _pProtoStruct;
			wstring _EnumName;
			TTypePtr _pType;
			TTypePtr _EnumType;
			TTypePtr _pConstType;
			wstring _ConstName;
			bool _Inherited;
			const wstring _TargetFile;
			const wstring _TempFile;
			wofstream _TempFS;
			wifstream _SourceFS;
			EMode _Mode;
			bool _HasEnumClass = false;
			wstring _Indentation;
			set<wstring> _IncludedSet;

			_STypeInfo ParseType(const wstring& TypeString_) const;
			virtual void CreateStruct(const _STypeInfo& StructInfo_) = 0;
			void _EnumProc(const wstring& EnumName_);
			void _ExEnumProc(const wstring& EnumName_);
			virtual void Close(void) {}
			virtual void InsertMember(const wstring& VarName_) = 0;
			virtual TTypePtr InsertDefine(const wstring& Name_, const wstring& NameSpace_, TTypePtr Type_) = 0;
			virtual wstring CurrentFullNameSpace(void) const = 0;
			virtual void WriteInclude(const wstring& Str_) = 0;
			virtual void InsertEnum(const wstring& Name_, TTypePtr BaseType_) = 0;
			virtual void WriteNameSpaceOpen(void) = 0;
			virtual void WriteNameSpaceClose(void) = 0;
			TTypePtr FindType(const wstring& TypeName_) const;
			TTypePtr InsertType(const wstring& TypeName_, TTypePtr pType_) const;
			TTypePtr GetContainerType(const wstring TypeName_, list<TTypePtr> ParameterTypes_);
			TTypePtr GetTypeRecursive(const wstring& TypeString_, size_t& Index_);
			TTypePtr GetType(const wstring& TypeString_); // 컨테이너에 묶인 자료형의 문자열을 파싱, Core까지 파싱했는데 찾아지지 않으면 exception
			virtual TTypePtr GetTemplateStructType(TTypePtr pBaseType_, const list<TTypePtr>& ParameterTypes_) const = 0;
			virtual void WriteHardCode(const wstring& Lang_, const wstring& Code_) = 0;
			virtual void WriteUsingNameSpace(const wstring& Name_) = 0;
			virtual void WriteTypedef(TTypePtr OriType_, const wstring& NewType_) = 0;
			virtual void WriteEnumName(bool HasClass_, const wstring& Str_) = 0;
			virtual void WriteEnumData(const wstring& Str_) = 0;
			virtual void WriteEnumClose(void) = 0;
			virtual void WriteInclude(void) = 0;
			// Struct ////////
			//////////////////
			virtual void WriteEnumType(const wstring& Name_) = 0; // EnumType을 출력파일에 기록
			virtual void WriteConst(TTypePtr Type_, const wstring& Name_, const wstring& Value_) = 0;

		public:
			CProtoBase(const wstring& IncludePath_, const wstring& SourceFile_, const wstring& TargetFile_, const wstring& LibraryName_);
			virtual ~CProtoBase() {}
			void ExportCore(wifstream& fSource_);
			void Export(void);
			virtual CProtoBase& operator << (const wstring& Str_) = 0;
		};
	}
}
