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
			// 일반적으로 CThread 가 속한 클래스의 멤버함수를 파라미터로 받기 때문에 해당 클래스의 이동에 의한 CThread의 이동은
			// CThread가 받은 멤버함수의 인스턴스를 새로운 클래스의 것으로 받지 못하기 때문에 이동 생성자, 연산자는 사용하지 않음.
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
