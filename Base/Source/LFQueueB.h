#pragma once

#include <Rso/Core/Core.h>
#include <atomic>

namespace rso::base
{
	using namespace core;
	using namespace std;

	template< typename TData >
	class CLFQueueB final
	{
	private:
		struct _SNode
		{
			atomic<bool> EndNode = true;
			TData* pData = nullptr;
			_SNode* pNext = nullptr;
		};

		// ���� ���� ���̿� �����Ͱ� �ִ� ��尡 �ڸ��ϰ� ��. ( ���� ������ ������ �����Ͱ� 0 )
		// �̷� ������ ������ ������ ��� ��带 Pop �ϴ��� ���� ������ ��ġ�� �ʵ��� �Ͽ� ���� �ٸ� �����尡 ���, ���� ��θ� �׼��� ���� �ʵ��� �ϱ� ����

		_SNode* _pPoppedHead = nullptr;
		_SNode* _pPoppedTail = nullptr;
		_SNode* _pPushedHead = nullptr;
		_SNode* _pPushedTail = nullptr;
		_SNode* _pNewPushNode = nullptr;

	public:
		CLFQueueB(CLFQueueB&& Var_) :
			_pPoppedHead(Var_._pPoppedHead),
			_pPoppedTail(Var_._pPoppedTail),
			_pPushedHead(Var_._pPushedHead),
			_pPushedTail(Var_._pPushedTail),
			_pNewPushNode(Var_._pNewPushNode)
		{
			Var_._pPoppedHead = nullptr;
			Var_._pPoppedTail = nullptr;
			Var_._pPushedHead = nullptr;
			Var_._pPushedTail = nullptr;
			Var_._pNewPushNode = nullptr;
		}
		CLFQueueB()
		{
			try
			{
				// LF �� �������� ���� �� ó���� �� �� �ӽ� ��尡 �ʿ�
				_pPoppedHead = new _SNode();
				_pPoppedTail = new _SNode();
				_pPushedHead = new _SNode();
				_pPushedTail = new _SNode();
				_pPoppedHead->pNext = _pPoppedTail;
				_pPushedHead->pNext = _pPushedTail;
			}
			catch (...)
			{
				if (_pPoppedHead)
					delete _pPoppedHead;
				if (_pPoppedTail)
					delete _pPoppedTail;
				if (_pPushedHead)
					delete _pPushedHead;
				if (_pPushedTail)
					delete _pPushedTail;

				throw;
			}
		}
		virtual ~CLFQueueB()
		{
			if (!_pPoppedHead)
				return;

			clear();

			for (auto* pNode = _pPoppedHead->pNext;
				!pNode->EndNode;)
			{
				delete pNode->pData;

				auto* pDelNode = pNode;
				pNode = pNode->pNext;
				delete pDelNode;
			}

			delete _pPoppedHead;
			delete _pPoppedTail;
			delete _pPushedHead;
			delete _pPushedTail;
		}
		CLFQueueB& operator = (CLFQueueB&& Var_)
		{
			this->~CLFQueueB();
			new (this) CLFQueueB(std::move(Var_));
			return *this;
		}
		TData* new_buf(void)
		{
			if (_pNewPushNode)
				return _pNewPushNode->pData;

			if (!_pPoppedHead->pNext->EndNode)
			{
				_pNewPushNode = _pPoppedHead->pNext;
				_pPoppedHead->pNext = _pPoppedHead->pNext->pNext;
			}
			else
			{
				_pNewPushNode = new _SNode();

				try
				{
					_pNewPushNode->pData = new TData();
				}
				catch (...)
				{
					delete _pNewPushNode;
					_pNewPushNode = nullptr;
					throw;
				}
			}

			return _pNewPushNode->pData;
		}
		void push(void)
		{
			if (!_pNewPushNode)
				return;

			_pPushedTail->pData = _pNewPushNode->pData;
			_pPushedTail->pNext = _pNewPushNode;
			_pNewPushNode->EndNode = true;
			_pNewPushNode->pData = nullptr;
			_pNewPushNode->pNext = nullptr;
			_pPushedTail->EndNode = false;
			_pPushedTail = _pNewPushNode;
			_pNewPushNode = nullptr;
		}
		TData* get(void)
		{
			if (_pPushedHead->pNext->EndNode)
				return nullptr;

			return _pPushedHead->pNext->pData;
		}
		void pop(void)
		{
			if (_pPushedHead->pNext->EndNode)
				return;

			auto* pNewNode = _pPushedHead->pNext;
			_pPushedHead->pNext = _pPushedHead->pNext->pNext;
			_pPoppedTail->pData = pNewNode->pData;
			_pPoppedTail->pNext = pNewNode;
			pNewNode->EndNode = true;
			pNewNode->pData = nullptr;
			pNewNode->pNext = nullptr;
			_pPoppedTail->EndNode = false;
			_pPoppedTail = pNewNode;
		}
		void clear(void)
		{
			for (auto pBuf = get(); pBuf; pBuf = get())
				pop();
		}
	};
}