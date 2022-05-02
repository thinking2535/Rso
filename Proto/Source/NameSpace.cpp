#include "NameSpace.h"

namespace rso
{
	namespace proto
	{
		const set<wstring> CNameSpace::SystemNamespace = { L"std", L"rso", L"core", L"json" };

		CNameSpace::CNameSpace()
		{
			_Root = make_shared<_SNameSpace>(L"");
			_NameSpaces.emplace_back(_Root.get());
		}
		void CNameSpace::In(const wstring& Name_)
		{
			auto itNameSpace = _NameSpaces.back()->NameSpaces.emplace(Name_, make_shared<_SNameSpace>(Name_)).first;
			_NameSpaces.emplace_back(itNameSpace->second.get());
		}
		void CNameSpace::Out(void)
		{
			if (_NameSpaces.size() == 1)
				THROWEX();

			_NameSpaces.pop_back();
		}
		void CNameSpace::Using(const wstring& Name_)
		{
			for (auto it = _NameSpaces.rbegin(); it != _NameSpaces.rend(); ++it)
			{
				auto itFound = (*it)->NameSpaces.find(Name_);
				if (itFound != (*it)->NameSpaces.end())
				{
					_NameSpaces.back()->UsingNameSpaces.emplace(Name_, itFound->second);

					return;
				}

				for (auto& itNameSpace : (*it)->UsingNameSpaces)
				{
					itFound = itNameSpace.second->NameSpaces.find(Name_);
					if (itFound != itNameSpace.second->NameSpaces.end())
					{
						_NameSpaces.back()->UsingNameSpaces.emplace(Name_, itFound->second);

						return;
					}
				}
			}

			if (CNameSpace::SystemNamespace.find(Name_) == CNameSpace::SystemNamespace.end())
				THROWEXA(L"Using Fail NameSpace [%s]", Name_);
		}
		void CNameSpace::NewContainer(const wstring& Name_, _TContainerInitializer Initializer_)
		{
			ASSERTION(_ContainerInitializers.emplace(Name_, Initializer_).second);
		}
		TTypePtr CNameSpace::NewType(const wstring& Name_, TTypePtr pType_) const
		{
			// 중복 체크 안함.
			return _NameSpaces.back()->Types.emplace(Name_, pType_).first->second;
		}
		TTypePtr CNameSpace::GetType(const wstring& Name_) const
		{
			for (auto it = _NameSpaces.rbegin(); it != _NameSpaces.rend(); ++it)
			{
				auto itType = (*it)->Types.find(Name_);
				if (itType != (*it)->Types.end())
					return itType->second;

				for (auto& itNameSpace : (*it)->UsingNameSpaces)
				{
					itType = itNameSpace.second->Types.find(Name_);
					if (itType != itNameSpace.second->Types.end())
						return itType->second;
				}
			}

			return nullptr;
		}
		wstring CNameSpace::GetRelativeName(const wstring& Name_) const
		{
			return _NameSpaces.front()->GetRelativeName(Name_);
		}
		CNameSpace::_TContainerInitializersCIt CNameSpace::GetContainerInitializer(const wstring& Name_) const
		{
			return _ContainerInitializers.find(Name_);
		}
		CNameSpace::_TContainerInitializersCIt CNameSpace::GetContainerInitializerEnd(void) const
		{
			return _ContainerInitializers.end();
		}
		list<wstring> CNameSpace::CurrentFullName() const
		{
			list<wstring> Names;

			auto it = _NameSpaces.begin();
			++it;

			for (; it != _NameSpaces.end(); ++it)
				Names.emplace_back((*it)->Name);

			return Names;
		}
	}
}