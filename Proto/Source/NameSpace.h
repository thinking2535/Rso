#pragma once

#include "Base.h"

namespace rso
{
	namespace proto
	{
		class CNameSpace
		{
			struct _SNameSpace;
			using _TTypes = map<wstring, TTypePtr>;
			using _TNameSpace = shared_ptr<_SNameSpace>;
			using _TNameSpaces = map<wstring, _TNameSpace>;
			using _TContainerInitializer = function<TTypePtr(TTypePtr KeyType_, TTypePtr Type_)>;
			using _TContainerInitializers = map<const wstring, _TContainerInitializer>;
			using _TContainerInitializersCIt = typename _TContainerInitializers::const_iterator;

			struct _SNameSpace
			{
				wstring Name;
				_TTypes Types;
				_TNameSpaces NameSpaces;
				_TNameSpaces UsingNameSpaces;

				_SNameSpace(const wstring& Name_) :
					Name(Name_)
				{
				}
			private:
				wstring _GetRelativeName(const wstring& Name_) const
				{
					if (Name_ == Name)
						return Name;

					for (auto& i : NameSpaces)
					{
						auto RelativeName = i.second->_GetRelativeName(Name_);
						if (!RelativeName.empty())
						{
							if (Name.empty())
								return RelativeName;
							else
								return Name + L"::" + RelativeName;
						}
					}

					return wstring();
				}
			public:
				wstring GetRelativeName(const wstring& Name_) const
				{
					for (auto& i : NameSpaces)
					{
						auto RelativeName = i.second->_GetRelativeName(Name_);
						if (!RelativeName.empty())
							return RelativeName;
					}

					THROWEXA(L"Invalid Namespace [%s]", Name_);
				}
			};

		public:
			static const set<wstring> SystemNamespace;

		private:
			map<wstring, TTypePtr> _TemplateTypes;
			_TContainerInitializers _ContainerInitializers;
			_TNameSpace _Root;
			vector<_SNameSpace*> _NameSpaces;

		public:
			CNameSpace();
			void In(const wstring& Name_);
			void Out(void);
			void Using(const wstring& Name_);
			void NewContainer(const wstring& Name_, _TContainerInitializer Initializer_);
			TTypePtr NewType(const wstring& Name_, TTypePtr pType_) const;
			TTypePtr GetType(const wstring& Name_) const;
			wstring GetRelativeName(const wstring& Name_) const;
			_TContainerInitializersCIt GetContainerInitializer(const wstring& Name_) const;
			_TContainerInitializersCIt GetContainerInitializerEnd(void) const;
			list<wstring> CurrentFullName() const;
			inline const wstring& CurrentName(void) const
			{
				return _NameSpaces.back()->Name;
			}
		};
	}
}