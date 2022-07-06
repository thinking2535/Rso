#pragma once

// operator new 를 쓰는 이유는 ConsDesCall==false 모드에서 reserve 시에는 생성자가 호출되어선 안되기 때문
// 또한 ~_CList 에서 객체의 소멸자가 두번 호출되지 않도록

#include <Rso/Core/Core.h>
#include <new>
#include <vector>
#include "ConstructDestruct.h"

namespace rso::base
{
	using namespace core;
	using namespace std;

	template<typename TData, typename TConstructDestruct = SFullConstructDestruct<TData>>
	class _CList
	{
	public:
		using value_type = TData;

	protected:
		struct _SNode
		{
			size_t Index;
			_SNode* pNext;
			_SNode* pPrev;
			bool Newed; // for emplace_at
			TData Data;
		};

		using _TNodes = vector<_SNode*>;

		_TNodes _Nodes;
		_SNode* _pDeletedHead = nullptr;
		_SNode* _pDeletedTail = nullptr;
		_SNode* _pNewedHead = nullptr;
		_SNode* _pNewedTail = nullptr;
		size_t _Size = 0;

		void _AttachToNewed(_SNode* pNode_)
		{
			pNode_->pNext = nullptr;
			pNode_->Newed = true;

			if (_pNewedTail == nullptr)
			{
				pNode_->pPrev = nullptr;
				_pNewedHead = _pNewedTail = pNode_;
			}
			else
			{
				pNode_->pPrev = _pNewedTail;
				_pNewedTail->pNext = pNode_;
				_pNewedTail = pNode_;
			}
		}
		void _AttachToDeleted(_SNode* pNode_)
		{
			pNode_->pNext = nullptr;
			pNode_->Newed = false;

			if (_pDeletedTail == nullptr)
			{
				pNode_->pPrev = nullptr;
				_pDeletedHead = _pDeletedTail = pNode_;
			}
			else
			{
				pNode_->pPrev = _pDeletedTail;
				_pDeletedTail->pNext = pNode_;
				_pDeletedTail = pNode_;
			}
		}
		void _Detach(_SNode* pNode_)
		{
			if (pNode_->pPrev)
				pNode_->pPrev->pNext = pNode_->pNext;
			if (pNode_->pNext)
				pNode_->pNext->pPrev = pNode_->pPrev;
		}
		void _DetachFromDeleted(_SNode* pNode_)
		{
			_Detach(pNode_);

			if (!pNode_->pPrev)
				_pDeletedHead = pNode_->pNext;
			if (!pNode_->pNext)
				_pDeletedTail = pNode_->pPrev;
		}

	public:
		struct const_iterator
		{
		public:
			_SNode* pNode{ nullptr };

			const_iterator()
			{
			}
			const_iterator(_SNode* pNode_) :
				pNode(pNode_)
			{
			}
			inline explicit operator bool() const
			{
				return (pNode != nullptr && pNode->Newed);
			}
			inline const TData& operator*() const
			{
				return pNode->Data;
			}
			inline const TData* operator->() const
			{
				return &pNode->Data;
			}
			const_iterator& operator++()
			{
				pNode = pNode->pNext;
				return *this;
			}
			const_iterator operator++(int)
			{
				auto Ret = *this;
				++(*this);
				return Ret;
			}
			inline size_t Index(void) const
			{
				return pNode->Index;
			}
			inline bool operator == (const const_iterator& Iterator_) const
			{
				return (pNode == Iterator_.pNode);
			}
			inline bool operator != (const const_iterator& Iterator_) const
			{
				return !(*this == Iterator_);
			}
			inline bool operator < (const const_iterator& Iterator_) const
			{
				return (pNode->Index < Iterator_.pNode->Index);
			}
		};
		struct iterator : public const_iterator
		{
			using const_iterator::operator*;
			using const_iterator::operator->;
			using const_iterator::pNode;

			iterator()
			{
			}
			iterator(_SNode* pNode_) :
				const_iterator(pNode_)
			{
			}
			inline TData& operator*()
			{
				return pNode->Data;
			}
			inline TData* operator->()
			{
				return &pNode->Data;
			}
			iterator& operator++()
			{
				const_iterator::operator++();
				return *this;
			}
			iterator operator++(int)
			{
				auto Ret = *this;
				++(*this);
				return Ret;
			}
		};
		_CList()
		{
		}
		template<typename... TArgsEmplace>
		_CList(size_t Size_, TArgsEmplace&&... Args_)
		{
			resize(Size_, forward<TArgsEmplace>(Args_)...);
		}
		_CList(const _CList& Var_)
		{
			for (auto it = Var_.begin(); it != Var_.end(); ++it)
				emplace_at(it.Index(), (*it));
		}
		_CList(_CList&& Var_) :
			_Nodes(std::move(Var_._Nodes)),
			_pDeletedHead(Var_._pDeletedHead),
			_pDeletedTail(Var_._pDeletedTail),
			_pNewedHead(Var_._pNewedHead),
			_pNewedTail(Var_._pNewedTail),
			_Size(Var_._Size)
		{
			Var_._pDeletedHead = nullptr;
			Var_._pDeletedTail = nullptr;
			Var_._pNewedHead = nullptr;
			Var_._pNewedTail = nullptr;
			Var_._Size = 0;
		}
		virtual ~_CList()
		{
			clear();

			for (auto& pNode : _Nodes)
			{
				TConstructDestruct::LastDestruct(&pNode->Data);
				operator delete(pNode);
			}
		}
		void reserve(size_t Size_)
		{
			auto OldSize = _Nodes.size();

			if (Size_ <= OldSize)
				return;

			_Nodes.resize(Size_);

			for (size_t i = OldSize; i < Size_; ++i)
			{
				_Nodes[i] = (_SNode*) operator new(sizeof(_SNode));
				_Nodes[i]->Index = i;
				_AttachToDeleted(_Nodes[i]);
			}
		}
		_CList& operator = (const _CList& Var_)
		{
			this->~_CList();
			new (this) _CList(Var_);
			return *this;
		}
		_CList& operator = (_CList&& Var_)
		{
			this->~_CList();
			new (this) _CList(std::move(Var_));
			return *this;
		}

		inline TData& operator[](size_t Index_)
		{
			return _Nodes[Index_]->Data;
		}
		inline const TData& operator[](size_t Index_) const
		{
			return _Nodes[Index_]->Data;
		}
		const_iterator get(size_t Index_) const
		{
			if (Index_ < 0 ||
				Index_ >= _Nodes.size() ||
				!_Nodes[Index_]->Newed)
				return const_iterator();

			return const_iterator(_Nodes[Index_]);
		}
		iterator get(size_t Index_)
		{
			if (Index_ < 0 ||
				Index_ >= _Nodes.size() ||
				!_Nodes[Index_]->Newed)
				return iterator();

			return iterator(_Nodes[Index_]);
		}
		inline const_iterator begin(void) const // for just iterate
		{
			return const_iterator(_pNewedHead);
		}
		inline const_iterator end(void) const // for just iterate
		{
			return const_iterator();
		}
		inline const_iterator last(void) const
		{
			return const_iterator(_pNewedTail);
		}
		inline iterator begin(void)
		{
			return iterator(_pNewedHead);
		}
		inline iterator end(void)
		{
			return iterator();
		}
		inline iterator last(void)
		{
			return iterator(_pNewedTail);
		}
		inline const TData& front(void) const
		{
			return _pNewedHead->Data;
		}
		inline TData& front(void)
		{
			return _pNewedHead->Data;
		}
		inline const TData& back(void) const
		{
			return _pNewedTail->Data;
		}
		inline TData& back(void)
		{
			return _pNewedTail->Data;
		}
		inline size_t capacity(void) const { return _Nodes.size(); }
		inline size_t size(void) const { return _Size; }
		inline bool empty(void) const { return (_Size <= 0); }
		size_t new_index(void) const
		{
			if (_pDeletedHead)
				return _pDeletedHead->Index;
			else
				return _Nodes.size();
		}
		template<typename... TArgsEmplace>
		void resize(size_t Size_, TArgsEmplace&&... Args_)
		{
			if (_Size > 0)
				return;

			for (size_t i = 0; i < Size_; ++i)
				emplace(forward<TArgsEmplace>(Args_)...);
		}
		template<typename... TArgsEmplace>
		iterator emplace(TArgsEmplace&&... Args_)
		{
			_SNode* pNode;

			// DetachFromDeleted ////////////////////////////////////
			if (_pDeletedHead)
			{
				pNode = _pDeletedHead;
				_DetachFromDeleted(pNode);
			}
			else
			{
				_Nodes.emplace_back(nullptr);

				try
				{
					_Nodes.back() = (_SNode*) operator new(sizeof(_SNode));

					try
					{
						TConstructDestruct::FirstConstruct(&_Nodes.back()->Data, forward<TArgsEmplace>(Args_)...);
					}
					catch (...)
					{
						operator delete(_Nodes.back());
						throw;
					}
				}
				catch (...)
				{
					_Nodes.pop_back();
					throw;
				}

				pNode = _Nodes.back();
				pNode->Index = _Nodes.size() - 1;
			}

			_AttachToNewed(pNode);

			++_Size;

			TConstructDestruct::Construct(&pNode->Data, forward<TArgsEmplace>(Args_)...);

			return iterator(pNode);
		}
		template<typename... TArgsEmplace>
		iterator emplace_at(size_t Index_, TArgsEmplace&&... Args_)
		{
			_SNode* pNode;

			// _Nodes 범위 이내 이면
			if (Index_ < _Nodes.size())
			{
				if (_Nodes[Index_]->Newed)
					THROWEX();

				pNode = _Nodes[Index_];
			}
			else
			{
				auto LastSize = _Nodes.size();

				for (auto i = LastSize; i < Index_ + 1; ++i)
				{
					_Nodes.emplace_back(nullptr);

					try
					{
						_Nodes.back() = (_SNode*) operator new(sizeof(_SNode));

						try
						{
							TConstructDestruct::FirstConstruct(&_Nodes.back()->Data, forward<TArgsEmplace>(Args_)...);
						}
						catch (...)
						{
							operator delete(_Nodes.back());
							throw;
						}
					}
					catch (...)
					{
						_Nodes.pop_back();
						throw;
					}

					_Nodes.back()->Index = _Nodes.size() - 1;
					_AttachToDeleted(_Nodes.back());
				}

				pNode = _Nodes.back();
			}

			_DetachFromDeleted(pNode);
			_AttachToNewed(pNode);

			++_Size;

			TConstructDestruct::Construct(&pNode->Data, forward<TArgsEmplace>(Args_)...);

			return iterator(pNode);
		}
	private:
		bool _erase(_SNode* pNode_)
		{
			if (!pNode_->Newed)
				return false;

			_Detach(pNode_);

			if (!pNode_->pPrev)
				_pNewedHead = pNode_->pNext;
			if (!pNode_->pNext)
				_pNewedTail = pNode_->pPrev;


			// AttachToDeleted ////////////////////
			_AttachToDeleted(pNode_);
			--_Size;

			TConstructDestruct::Destruct(&pNode_->Data);

			return true;
		}
	public:
		inline bool erase(const_iterator Iterator_)
		{
			return _erase(Iterator_.pNode);
		}
		bool erase(size_t Index_)
		{
			if (Index_ < 0 ||
				Index_ >= _Nodes.size())
				return false;

			return _erase(_Nodes[Index_]);
		}
		inline void pop_front(void)
		{
			_erase(_pNewedHead);
		}
		inline void pop_back(void)
		{
			_erase(_pNewedTail);
		}
		void clear(void)
		{
			for (auto it = begin();
				it;)
			{
				auto itDel = it;
				++it;

				erase(itDel);
			}
		}
	};

	template<typename TData>
	class CList : public _CList<TData, SFullConstructDestruct<TData>>
	{
	};

	template<typename TData>
	class CListB : public _CList<TData, SLazyConstructDestruct<TData>>
	{
	};
}