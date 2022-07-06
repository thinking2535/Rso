#pragma once

#include <Rso/Core/Core.h>
#include <atomic>
#include "ConstructDestruct.h"

namespace rso::base
{
	using namespace core;
	using namespace std;

	template<typename TData, typename TConstructDestruct = SFullConstructDestruct<TData>>
	class _CLFQueue
	{
	private:
		struct _SNode
		{
			atomic<bool> EndNode;
			TData Data;
			_SNode* pNext;
		};

		// 헤드와 테일 사이에 데이터가 있는 노드가 자리하게 됨. ( 헤드와 테일은 데이터 포인터가 0 )
		// 이런 구조를 가지는 이유는 모든 노드를 Pop 하더라도 헤드와 테일이 겹치지 않도록 하여 서로 다른 스레드가 헤드, 테일 모두를 액세스 하지 않도록 하기 위함

		_SNode* _pPushedHead = nullptr;
		_SNode* _pPushedTail = nullptr;
		_SNode* _pPoppedHead = nullptr;
		_SNode* _pPoppedTail = nullptr;

	public:
		_CLFQueue(_CLFQueue&& Var_) :
			_pPushedHead(Var_._pPushedHead),
			_pPushedTail(Var_._pPushedTail),
			_pPoppedHead(Var_._pPoppedHead),
			_pPoppedTail(Var_._pPoppedTail)
		{
			Var_._pPushedHead = nullptr;
			Var_._pPushedTail = nullptr;
			Var_._pPoppedHead = nullptr;
			Var_._pPoppedTail = nullptr;
		}
		_CLFQueue()
		{
			// LF 로 수행하기 위해 맨 처음과 맨 끝 임시 노드가 필요
			_pPushedHead = (_SNode*) operator new(sizeof(_SNode));

			try
			{
				_pPushedTail = (_SNode*) operator new(sizeof(_SNode));

				try
				{
					_pPoppedHead = (_SNode*) operator new(sizeof(_SNode));

					try
					{
						_pPoppedTail = (_SNode*) operator new(sizeof(_SNode));

						_pPushedHead->EndNode = true;
						_pPushedTail->EndNode = true;
						_pPoppedHead->EndNode = true;
						_pPoppedTail->EndNode = true;

						_pPushedHead->pNext = _pPushedTail;
						_pPushedTail->pNext = _pPoppedHead;
						_pPoppedHead->pNext = _pPoppedTail;
						_pPoppedTail->pNext = _pPushedHead;

						TConstructDestruct::FirstConstruct(&_pPushedHead->Data);
						TConstructDestruct::FirstConstruct(&_pPushedTail->Data);
						TConstructDestruct::FirstConstruct(&_pPoppedHead->Data);
						TConstructDestruct::FirstConstruct(&_pPoppedTail->Data);
					}
					catch (...)
					{
						delete _pPoppedHead;
						throw;
					}
				}
				catch (...)
				{
					delete _pPushedTail;
					throw;
				}
			}
			catch (...)
			{
				delete _pPushedHead;
				throw;
			}
		}
		virtual ~_CLFQueue()
		{
			clear(); // for call destructor

			auto pNode = _pPushedHead;

			do
			{
				auto pDelNode = pNode;
				pNode = pNode->pNext;
				TConstructDestruct::LastDestruct(&pDelNode->Data);
				operator delete(pDelNode);

			} while (pNode != _pPushedHead);
		}
		_CLFQueue& operator = (_CLFQueue&& Var_)
		{
			this->~_CLFQueue();
			new (this) _CLFQueue(std::move(Var_));
			return *this;
		}
		template<typename... TArgsEmplace>
		void emplace(TArgsEmplace&&... Args_)
		{
			TConstructDestruct::Construct(&_pPushedTail->Data, forward<TArgsEmplace>(Args_)...);
			push(forward<TArgsEmplace>(Args_)...);
		}
		TData* new_buf(void)
		{
			return &_pPushedTail->Data;
		}
		template<typename... TArgsEmplace>
		void push(TArgsEmplace&&... Args_)
		{
			if (_pPoppedHead->pNext->EndNode)
			{
				_pPushedTail->pNext = (_SNode*) operator new(sizeof(_SNode));
				_pPushedTail->pNext->EndNode = true;
				_pPushedTail->pNext->pNext = _pPoppedHead;
				TConstructDestruct::FirstConstruct(&_pPushedTail->pNext->Data, forward<TArgsEmplace>(Args_)...);
			}
			else
			{
				_pPoppedHead->pNext->EndNode = true;
				_pPoppedHead = _pPoppedHead->pNext;
			}

			_pPushedTail->EndNode = false;
			_pPushedTail = _pPushedTail->pNext;
		}
		TData* get(void)
		{
			if (_pPushedHead->pNext->EndNode)
				return nullptr;

			return &_pPushedHead->pNext->Data;
		}
		void pop(void)
		{
			if (_pPushedHead->pNext->EndNode)
				return;

			TConstructDestruct::Destruct(&_pPushedHead->pNext->Data);
			_pPushedHead->pNext->EndNode = true;
			_pPushedHead = _pPushedHead->pNext;
			_pPoppedTail->EndNode = false;
			_pPoppedTail = _pPoppedTail->pNext;
		}
		void clear(void)
		{
			for (auto pBuf = get(); pBuf; pBuf = get())
				pop();
		}
	};

	template<typename TData>
	class CLFQueue : public _CLFQueue<TData, SFullConstructDestruct<TData>>
	{
		TData* new_buf(void) = delete;
		void push(void) = delete;
	};

	template<typename TData>
	class CLFQueueB : public _CLFQueue<TData, SLazyConstructDestruct<TData>>
	{
		template<typename... TArgsEmplace>
		void emplace(TArgsEmplace&&... Args_) = delete;
	};
}