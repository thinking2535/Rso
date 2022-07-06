#pragma once

#include <type_traits>

namespace rso::base
{
	using namespace std;

	template<typename TData>
	struct SFullConstructDestruct
	{
		template<typename... TArgsEmplace>
		static void FirstConstruct(TData* /*pData_*/, TArgsEmplace&&... /*Args_*/)
		{
		}
		static void LastDestruct(TData* /*pData_*/)
		{
		}
		template<typename... TArgsEmplace>
		static void Construct(TData* pData_, TArgsEmplace&&... Args_)
		{
			new(pData_)TData(forward<TArgsEmplace>(Args_)...);
		}
		static void Destruct(TData* pData_)
		{
			pData_->~TData();
		}
	};
	template<typename TData, typename... TArgsEmplace>
	struct SLazyConstructDestruct
	{
		template<typename... TArgsEmplace>
		static void FirstConstruct(TData* pData_, TArgsEmplace&&... Args_)
		{
			new(pData_)TData(forward<TArgsEmplace>(Args_)...);
		}
		static void LastDestruct(TData* pData_)
		{
			pData_->~TData();
		}
		template<typename... TArgsEmplace>
		static void Construct(TData* /*pData_*/, TArgsEmplace&&... /*Args_*/)
		{
		}
		static void Destruct(TData* /*pData_*/)
		{
		}
	};
}