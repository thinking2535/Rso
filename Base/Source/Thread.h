#pragma once

#include "Base.h"
#include <new>
#include <functional>
#include <thread>

namespace rso
{
    namespace base
    {
        using namespace std;
        using namespace std::placeholders;

		class CThread
		{
		public:
			using TThreadFunc = function<void(const volatile bool* Exit_)>;

		private:
			unique_ptr<volatile bool> _Exit{ new bool(false) };
			thread _Thread;

		public:
			template<class _Fn, typename... _TParams>
			explicit CThread(_Fn&& _Fx, _TParams&&... Params_) :
				_Thread(_Fx, _Exit.get(), Params_...)
			{
			}
			// �Ϲ������� CThread �� ���� Ŭ������ ����Լ��� �Ķ���ͷ� �ޱ� ������ �ش� Ŭ������ �̵��� ���� CThread�� �̵���
			// CThread�� ���� ����Լ��� �ν��Ͻ��� ���ο� Ŭ������ ������ ���� ���ϱ� ������ �̵� ������, �����ڴ� ������� ����.
			CThread(const CThread& Var_) = delete;
			CThread(CThread&& Var_) = delete;
			virtual ~CThread()
			{
				*_Exit = true;
				_Thread.join();
			}
			void operator = (const CThread& Var_) = delete;
			void operator = (CThread&& Var_) = delete;
		};
	}
}
