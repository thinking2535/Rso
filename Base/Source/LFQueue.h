#pragma once

#include <Rso/Core/Core.h>
#include <atomic>

namespace rso::base
{
	using namespace core;
	using namespace std;

	template< typename TData >
	class CLFQueue final
	{
	private:
		struct _SNode
		{
			atomic<bool> EndNode = true;
			unique_ptr<TData, void(*)(TData*)> pData{ static_cast<TData*>(operator new (sizeof(TData))), [](TData* pData_) { operator delete(pData_); } };
			_SNode* pNext = nullptr;

			_SNode()
			{
			}
			_SNode(_SNode* pNext_) :
				pNext(pNext_)
			{
			}
		};

		// ���� ���� ���̿� �����Ͱ� �ִ� ��尡 �ڸ��ϰ� ��. ( ���� ������ ������ �����Ͱ� 0 )
		// �̷� ������ ������ ������ ��� ��带 Pop �ϴ��� ���� ������ ��ġ�� �ʵ��� �Ͽ� ���� �ٸ� �����尡 ���, ���� ��θ� �׼��� ���� �ʵ��� �ϱ� ����

		_SNode* _pPushedHead = nullptr;
		_SNode* _pPushedTail = nullptr;
		_SNode* _pPoppedHead = nullptr;
		_SNode* _pPoppedTail = nullptr;

	public:
		CLFQueue(CLFQueue&& Var_) :
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
		CLFQueue()
		{
			// LF �� �����ϱ� ���� �� ó���� �� �� �ӽ� ��尡 �ʿ�
			_pPushedHead = new _SNode();

			try
			{
				_pPushedTail = new _SNode();

				try
				{
					_pPoppedHead = new _SNode();

					try
					{
						_pPoppedTail = new _SNode();
						_pPushedHead->pNext = _pPushedTail;
						_pPushedTail->pNext = _pPoppedHead;
						_pPoppedHead->pNext = _pPoppedTail;
						_pPoppedTail->pNext = _pPushedHead;
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
		virtual ~CLFQueue()
		{
			clear(); // for call destructor

			for (auto* pNode = _pPoppedHead->pNext;
				!pNode->EndNode;)
			{
				auto* pDelNode = pNode;
				pNode = pNode->pNext;
				delete pDelNode;
			}

			delete _pPoppedTail;
			delete _pPoppedHead;
			delete _pPushedTail;
			delete _pPushedHead;
		}
		CLFQueue& operator = (CLFQueue&& Var_)
		{
			this->~CLFQueue();
			new (this) CLFQueue(std::move(Var_));
			return *this;
		}
		template<typename... TArgsEmplace>
		void emplace(TArgsEmplace&&... Args_)
		{
			if (_pPoppedHead->pNext->EndNode)
			{
				_pPushedTail->pNext = new _SNode(_pPoppedHead);
			}
			else
			{
				_pPoppedHead->pNext->EndNode = true;
				_pPoppedHead = _pPoppedHead->pNext;
			}

			new(_pPushedTail->pData.operator->()) TData(forward<TArgsEmplace>(Args_)...);
			_pPushedTail->EndNode = false;
			_pPushedTail = _pPushedTail->pNext;
		}
		TData* get(void)
		{
			if (_pPushedHead->pNext->EndNode)
				return nullptr;

			return _pPushedHead->pNext->pData.operator->();
		}
		void pop(void)
		{
			if (_pPushedHead->pNext->EndNode)
				return;

			_pPushedHead->pNext->pData.operator->()->~TData();
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
}