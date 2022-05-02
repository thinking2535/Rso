#include "ProtoBase.h"

namespace rso
{
	namespace proto
	{
		_STypeInfo CProtoBase::ParseType(const wstring& TypeString_) const
		{
			const auto TemplateOpenIndex = TypeString_.find_first_of(L"<");
			if (TemplateOpenIndex != wstring::npos)
			{
				if (TypeString_.back() != L'>')
					THROWEX();

				// make fullname without space
				const auto TemplateName = TypeString_.substr(0, TemplateOpenIndex);
				const auto ParameterNames = Split(TypeString_.substr(TemplateOpenIndex + 1, TypeString_.size() - TemplateOpenIndex - 2), L", \t");
				return _STypeInfo(TemplateName, ParameterNames);
			}
			else
			{
				return _STypeInfo(TypeString_, list<wstring>());
			}
		}
		void CProtoBase::_EnumProc(const wstring& EnumName_)
		{
			(*this) << _Indentation;
			_EnumName = EnumName_;
			WriteEnumName(_HasEnumClass, EnumName_);
			_Mode = EMode::EnumOpen;
		}
		void CProtoBase::_ExEnumProc(const wstring& EnumName_)
		{
			InsertEnum(EnumName_, GetType(L"int32"));
			_Mode = EMode::NameSpace;
		}
		TTypePtr CProtoBase::FindType(const wstring& TypeName_) const
		{
			auto Type = _NameSpace.GetType(TypeName_);
			if (!Type && _pProtoStruct)
				Type = _pProtoStruct->FindTemplateType(TypeName_);

			return Type;
		}
		TTypePtr CProtoBase::InsertType(const wstring& TypeName_, TTypePtr pType_) const
		{
			return _NameSpace.NewType(TypeName_, pType_);
		}

		TTypePtr CProtoBase::GetTypeRecursive(const wstring& TypeString_, size_t& Index_)
		{
			if (TypeString_.empty())
				THROWEXA(L"Type is Empty String");

			bool Opened = false;
			wstring TypeName;
			list<TTypePtr> ParameterTypes;

			while (Index_ < TypeString_.size())
			{
				auto& c = TypeString_[Index_];

				if (c == L'<')
				{
					if (Index_ + 1 >= TypeString_.size())
						THROWEXA(L"TypeString_ is ended but got '<'");

					Opened = true;
					++Index_;

					ParameterTypes.emplace_back(GetTypeRecursive(TypeString_, Index_));
					continue;
				}
				else if (c == L'>')
				{
					if (Opened)
					{
						++Index_;
						Opened = false;
					}

					break;
				}
				else if (c == L',')
				{
					if (Opened)
					{
						++Index_;

						if (Index_ >= TypeString_.size())
							THROWEXA(L"TypeString_ is ended but got '<'");

						ParameterTypes.emplace_back(GetTypeRecursive(TypeString_, Index_));
						continue;
					}
					else
					{
						break;
					}
				}
				else
				{
					TypeName.push_back(c);
				}

				++Index_;
			}

			if (Opened)
				THROWEXA(L"GetTypeRecursive ended but '<' opened");

			if (TypeName.empty())
				THROWEXA(L"TypeName is empty");

			auto pType = FindType(TypeName);
			if (pType)
			{
				pType->CheckTemplateParameter(ParameterTypes.size());

				if (ParameterTypes.empty())
					return pType;
				else
					return GetTemplateStructType(pType, ParameterTypes);
			}
			else
			{
				try
				{
					return make_shared<SNumber>(TypeName);
				}
				catch (...)
				{
					return GetContainerType(TypeName, ParameterTypes); // todo need to delete (do not container when reusing)
				}
			}
		}
		TTypePtr CProtoBase::GetType(const wstring& TypeString_)
		{
			size_t Index = 0;
			auto pType = GetTypeRecursive(TypeString_, Index);

			if (Index != TypeString_.size())
				THROWEXA(L"GetType ended but Index != TypeString_.size()");

			return pType;
		}
		TTypePtr CProtoBase::GetContainerType(const wstring TypeName_, list<TTypePtr> ParameterTypes_)
		{
			auto it = _NameSpace.GetContainerInitializer(TypeName_);
			if (it == _NameSpace.GetContainerInitializerEnd())
				THROWEXA(L"Invalid Template Type Name[%s]", TypeName_);

			auto itType = ParameterTypes_.begin();
			if (itType == ParameterTypes_.end())
				THROWEX();

			auto FirstType = *itType;

			++itType;
			TTypePtr SecondType;

			if (itType != ParameterTypes_.end())
				SecondType = *itType;

			return it->second(FirstType, SecondType);
		}

		CProtoBase::CProtoBase(const wstring& IncludePath_, const wstring& SourceFile_, const wstring& TargetFile_, const wstring& LibraryName_) :
			_IncludePath(IncludePath_),
			_LibraryName(LibraryName_),
			_Inherited(false),
			_TargetFile(TargetFile_),
			_TempFile(TargetFile_ + L".tmp"),
			_TempFS(_TempFile, wofstream::trunc),
			_SourceFS(SourceFile_ + L".prt", ios::in),
			_Mode(EMode::NameSpace)
		{
			if (_TempFS.fail())
				THROWEXA(L"Temp File Open fail %s", _TempFile);

			_IncludePath = absolute(_IncludePath);

			if (_SourceFS.fail())
				THROWEXA(L"Prt File Open fail %s", SourceFile_);

			_CurrentPaths.emplace_back(current_path().append(path(SourceFile_).parent_path().string()));
		}
		void CProtoBase::ExportCore(wifstream& fSource_)
		{
			wstring Line;
			while (!fSource_.eof())
			{
				std::getline(fSource_, Line);
				Line = Trim(Line);

				for (auto& Str : Split(Line, L" \t\n\r\v"))
				{
					// 주석 관련 코드 ( 상속된 클래스나, C# 의 typedef 등은 문제 발생 )
					//if (Line.size() >= 2)
					//{
					// if (Line[0] == L'/' &&
					// Line[1] == L'/')
					// {
					// (*this) << Line;

					// Line.clear();
					// continue;
					// }
					//}

					if (_Mode == EMode::NameSpace)
					{
						if (Str == L"using")
						{
							_Mode = EMode::Using;
						}
						else if (Str == L"namespace")
						{
							_Mode = EMode::NameSpaceName;
						}
						else if (Str == L"struct")
						{
							_Mode = EMode::StructName;
						}
						else if (Str == L"enum")
						{
							_Mode = EMode::EnumBegin;
							_HasEnumClass = false;
						}
						else if (Str == L"exenum")
						{
							_Mode = EMode::ExEnumBegin;
						}
						else if (Str == L"typedef")
						{
							_Mode = EMode::Typedef;
						}
						else if (Str == L"cs" || Str == L"cpp")
						{
							WriteHardCode(Str, Line);
							Line.clear();
						}
						else if (Str == L"{")
						{
							(*this) << _Indentation;
							(*this) << Str << L"\n";
							_Indentation.push_back(L'\t');
						}
						else if (Str == L"}")
						{
							WriteNameSpaceClose();
							_NameSpace.Out();
						}
						else if (Str == L"include")
						{
							_Mode = EMode::IncludeName;
						}
						else if (Str == L"const")
						{
							_Mode = EMode::Const;
						}
						else
						{
							THROWEXA(L"Invalid Token[%s]", Str);
						}
					}
					else if (_Mode == EMode::Using)
					{
						_Mode = EMode::UsingNameSpace;
					}
					else if (_Mode == EMode::UsingNameSpace)
					{
						_NameSpace.Using(Str);
						WriteUsingNameSpace(Str);
						_Mode = EMode::NameSpace;
					}
					else if (_Mode == EMode::NameSpaceName)
					{
						_NameSpace.In(Str);
						_Mode = EMode::NameSpaceOpen;
					}
					else if (_Mode == EMode::NameSpaceOpen)
					{
						if (Str != L"{")
							THROWEX();

						WriteNameSpaceOpen();

						_Mode = EMode::NameSpace;
					}
					else if (_Mode == EMode::StructName)
					{
						CreateStruct(ParseType(Str));
						_Mode = EMode::StructOpen;
					}
					else if (_Mode == EMode::StructOpen)
					{
						if (Str == L"{")
						{
							_Inherited = false;
							_Mode = EMode::Struct;
						}
						else if (Str == L":")
						{
							if (_Inherited)
								THROWEXA(L"Aleardy Interited");

							_Inherited = true;
							_Mode = EMode::SuperName;
						}
						else
						{
							THROWEXA(L"Expact {  or :  but [%s]", Str);
						}
					}
					else if (_Mode == EMode::SuperName)
					{
						if (Str.empty())
							THROWEXA(L"Empty SuperName");

						auto pSuperType = GetType(Str);
						if (!pSuperType || !pSuperType->IsStruct())
							THROWEXA(L"Invalid SuperName[%s]", Str);

						_pProtoStruct->SetSuper(pSuperType);

						_Mode = EMode::StructOpen;
					}
					else if (_Mode == EMode::Struct)
					{
						if (Str == L"}")
						{
							(*this) << InsertType(_pProtoStruct->Name(), _pProtoStruct)->String(_Indentation);
							_pProtoStruct = nullptr;
							_Mode = EMode::NameSpace;
						}
						else
						{
							_pType = GetType(Str);
							if (_pType == nullptr)
								THROWEXA(L"Member Type [%s] is null", Str);

							_Mode = EMode::MemberName;
						}
					}
					else if (_Mode == EMode::MemberName)
					{
						InsertMember(Str);
						_Mode = EMode::Struct;
					}
					else if (_Mode == EMode::EnumBegin)
					{
						if (Str == L"class")
						{
							_HasEnumClass = true;
							_Mode = EMode::EnumName;
						}
						else
						{
							_EnumProc(Str);
						}
					}
					else if (_Mode == EMode::EnumName)
					{
						_EnumProc(Str);
					}
					else if (_Mode == EMode::EnumOpen)
					{
						if (Str == L"{")
						{
							(*this) << L"\n" << _Indentation << Str << L"\n";
							_Indentation.push_back(L'\t');
							_Mode = EMode::Enum;
						}
						else if (Str == L":")
						{
							_Mode = EMode::EnumType;
						}
						else
						{
							THROWEX();
						}
					}
					else if (_Mode == EMode::EnumType)
					{
						_EnumType = _NameSpace.GetType(Str);
						if (!_EnumType)
							THROWEXA(L"Invalid EnumType[%s]", Str);

						WriteEnumType(_EnumType->Name());
						_Mode = EMode::EnumOpen;
					}
					else if (_Mode == EMode::Enum)
					{
						if (Str == L"}")
						{
							_Indentation.pop_back();
							(*this) << _Indentation;
							(*this) << Str;
							WriteEnumClose();
							(*this) << L"\n";

							if (!_EnumType)
								_EnumType = GetType(L"int32");

							InsertEnum(_EnumName, _EnumType);

							_EnumName.clear();
							_EnumType = 0;
							_Mode = EMode::NameSpace;
						}
						else
						{
							(*this) << _Indentation;
							WriteEnumData(Str);
						}
					}
					else if (_Mode == EMode::ExEnumBegin)
					{
						_ExEnumProc(Str);
					}
					else if (_Mode == EMode::Typedef)
					{
						_pType = GetType(Str);
						if (!_pType)
							THROWEXA(Str);

						_Mode = EMode::TypedefName;
					}
					else if (_Mode == EMode::TypedefName)
					{
						InsertDefine(Str, CurrentFullNameSpace(), _pType);
						WriteTypedef(_pType, Str);

						_Mode = EMode::NameSpace;
					}
					else if (_Mode == EMode::IncludeName)
					{
						wifstream fSource;

						if (Str.size() == 0)
							THROWEX();

						wstring IncludedName; // 현재 포함하려하는 파일의 상대 또는 절대 경로이름.
						path CurPath; // 현재 포함파일이 있는 경로
						path FilePath; // 현재 포함하려하는 파일의 절대 경로이름.
						if (Str[0] == L'<') // 포함폴더에서 검색
						{
							if (Str.size() < 3)
								THROWEX();

							if (Str[Str.size() - 1] != L'>')
								THROWEX();

							IncludedName = Trim(Str.substr(1, Str.size() - 2));

							FilePath = path(_IncludePath).append(IncludedName).concat(L".prt");
							wcout << StringFormat(L"Try To FileOpen[%s]", FilePath) << endl;

							fSource.open(FilePath, ios::in);
							if (!fSource.is_open())
								THROWEXA(L"FileOpen Fail[%s]", FilePath);

							CurPath = _IncludePath;
							WriteInclude(L"#include <" + IncludedName + L".h>\n");
						}
						else
						{
							// 현재 폴더 먼저 검사
							FilePath = path(_CurrentPaths.back()).append(Str).concat(L".prt");

							fSource.open(FilePath, ios::in);
							if (fSource.is_open())
							{
								CurPath = _CurrentPaths.back();
							}
							else
							{
								// 없으면 작업폴더 검사
								FilePath = path(_CurrentPaths.front()).append(Str).concat(L".prt");

								fSource.open(FilePath, ios::in);
								if (!fSource.is_open())
									THROWEXA(L"Local FileOpen Fail Str[%s] FilePath[%s]", Str, FilePath);

								CurPath = _CurrentPaths.front();
							}

							WriteInclude(L"#include \"" + Str + L".h\"\n");
						}

						_Mode = EMode::NameSpace;

						if (_IncludedSet.emplace(to_std_path(FilePath.wstring())).second)
						{
							if (path(IncludedName).is_absolute())
								_CurrentPaths.emplace_back(path(IncludedName).parent_path());
							else
								_CurrentPaths.emplace_back(CurPath.append(path(IncludedName).parent_path().string()));

							++_IncludedCnt;
							ExportCore(fSource);
							--_IncludedCnt;

							_CurrentPaths.pop_back();
						}
					}
					else if (_Mode == EMode::Const)
					{
						_pConstType = _NameSpace.GetType(Str);
						if (!_pConstType)
							THROWEX();

						_Mode = EMode::ConstName;
					}
					else if (_Mode == EMode::ConstName)
					{
						_ConstName = Str;

						_Mode = EMode::ConstValue;
					}
					else if (_Mode == EMode::ConstValue)
					{
						WriteConst(_pConstType, _ConstName, Str);

						_Mode = EMode::NameSpace;
					}
				}
			}
		}
		void CProtoBase::Export(void)
		{
			WriteInclude();

			ExportCore(_SourceFS);

			Close();
			_TempFS.close();

			{
				std::wifstream fTemp(_TempFile, wifstream::ate);
				std::wifstream fTarget(_TargetFile, wifstream::ate);
				if (!fTarget.fail() && fTemp.tellg() == fTarget.tellg())
				{
					fTemp.seekg(0, std::wifstream::beg);
					fTarget.seekg(0, std::wifstream::beg);

					if (std::equal(std::istreambuf_iterator<wchar_t>(fTemp.rdbuf()),
						std::istreambuf_iterator<wchar_t>(),
						std::istreambuf_iterator<wchar_t>(fTarget.rdbuf())))
					{
						fTemp.close();
						remove(_TempFile);
						return;
					}
				}
			}

			rename(_TempFile, _TargetFile);
		}
	}
}
