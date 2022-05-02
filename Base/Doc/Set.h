#pragma once


#include <new>
#include <vector>


namespace rso
{
	namespace base
	{
		using namespace std;

		template<typename TKey, typename TData, bool Call_ = true>
		class CSet
		{
		private:
			struct _SNode
			{
				size_t		Index{ 0 };
				bool		Newed : 4;
				bool		Constructed : 4;
				TData		Data;
				_SNode*		pNext{ nullptr };
				_SNode*		pPrev{ nullptr };

				template<typename... TArgs>
				_SNode(TArgs&&... Args_) :
					Newed(false), Constructed(false),
					Data(forward<TArgs>(Args_)...)
				{
				}
			};

			using	_TNodes = vector<_SNode*>;

			_TNodes		_Nodes;
			_SNode*		_pDeletedHead{};
			_SNode*		_pDeletedTail{};
			_SNode*		_pNewedHead{};
			_SNode*		_pNewedTail{};
			size_t		_Size{};

			_SNode*		_New(void)
			{
				auto* Node = (_SNode*) operator new(sizeof(_SNode), nothrow);
				if (!Node)
					return nullptr;

				Node->Constructed = false;
				return Node;
			}
			template<typename... TArgsEmplace>
			void		_AttachToNewed(_SNode* pNode_, TArgsEmplace&&... Args_)
			{
				pNode_->pNext = 0;
				pNode_->Newed = true;

				if (_pNewedTail == 0)
				{
					pNode_->pPrev = 0;
					_pNewedHead = _pNewedTail = pNode_;
				}
				else
				{
					pNode_->pPrev = _pNewedTail;
					_pNewedTail->pNext = pNode_;
					_pNewedTail = pNode_;
				}

				if (!pNode_->Constructed)
				{
					pNode_->Constructed = true;
					new(&pNode_->Data)TData(forward<TArgsEmplace>(Args_)...);
				}
			}
			void		_AttachToDeleted(_SNode* pNode_)
			{
				pNode_->pNext = 0;
				pNode_->Newed = false;

				if (_pDeletedTail == 0)
				{
					pNode_->pPrev = 0;
					_pDeletedHead = _pDeletedTail = pNode_;
				}
				else
				{
					pNode_->pPrev = _pDeletedTail;
					_pDeletedTail->pNext = pNode_;
					_pDeletedTail = pNode_;
				}
			}
			void		_Detach(_SNode* pNode_)
			{
				if (pNode_->pPrev)
					pNode_->pPrev->pNext = pNode_->pNext;
				if (pNode_->pNext)
					pNode_->pNext->pPrev = pNode_->pPrev;
			}
			void		_DetachFromDeleted(_SNode* pNode_)
			{
				_Detach(pNode_);

				if (!pNode_->pPrev)
					_pDeletedHead = pNode_->pNext;
				if (!pNode_->pNext)
					_pDeletedTail = pNode_->pPrev;
			}

		public:
			struct SIterator
			{
			private:
				const _SNode*	_pNode{ 0 };

			public:
				SIterator() : _pNode() {}
				SIterator(const _SNode* pNode_) : _pNode(pNode_)
				{
				}
				TData&		operator*()
				{
					return (TData&)(_pNode->Data);
				}
				TData*		operator->()
				{
					return (TData*)(&_pNode->Data);
				}
				SIterator	operator++()
				{
					_pNode = _pNode->pNext;

					return *this;
				}
				SIterator	operator++(int)
				{
					auto Ret = *this;

					++(*this);

					return Ret;
				}
				inline _SNode*	Node(void)
				{
					return (_SNode*)(_pNode);
				}
				size_t	Index(void) const
				{
					return _pNode->Index;
				}
				bool	Valid(void) const
				{
					return (_pNode != 0);
				}
				bool	operator != (const SIterator& Iterator_)
				{
					return (_pNode != Iterator_._pNode);
				}
			};
			using SPairIB = pair<SIterator, bool>;

			CSet()
			{
			}
			CSet(const CSet& Var_)
			{
				for (auto it = Var_.begin(); it != Var_.end(); ++it)
					emplace_at(it.Index(), (*it));
			}
			~CSet()
			{
				for (auto& pNode : _Nodes)
				{
					if (pNode->Constructed)
						pNode->Data.~TData();
				}

				for (auto& pNode : _Nodes)
					operator delete(pNode, nothrow);
			}
			void	reserve(size_t Size_)
			{
				auto OldSize = _Nodes.size();

				if (Size_ <= OldSize)
					return;

				try
				{
					_Nodes.resize(Size_);
				}
				catch (...)
				{
					THROW();
				}

				for (size_t i = OldSize; i < Size_; ++i)
				{
					_Nodes[i] = _New();
					if (!_Nodes[i])
						THROW();

					_Nodes[i]->Index = i;
					_AttachToDeleted(_Nodes[i]);
				}
			}
			CSet	operator = (const CSet& Var_) = delete;
			CSet	operator = (CSet&& Var_) = delete;

			TData*		operator[](size_t Index_) const
			{
				if (Index_ < 0 ||
					Index_ >= _Nodes.size() ||
					!_Nodes[Index_]->Newed)
					return nullptr;

				return &_Nodes[Index_]->Data;
			}

			SIterator		find(size_t Index_) const
			{
				if (Index_ < 0 ||
					Index_ >= _Nodes.size() ||
					!_Nodes[Index_]->Newed)
					return SIterator();

				return SIterator(_Nodes[Index_]);
			}
			SIterator	begin(void) const	// for just iterate
			{
				return SIterator(_pNewedHead);
			}
			SIterator	end(void) const	// for just iterate
			{
				return SIterator();
			}

			size_t		size(void) const { return _Size; }

			template<typename... TArgsEmplace>
			SPairIB	emplace(TArgsEmplace&&... Args_)
			{
				_SNode* pNode = nullptr;

				// DetachFromDeleted ////////////////////////////////////
				if (_pDeletedHead)
				{
					pNode = _pDeletedHead;

					_DetachFromDeleted(pNode);
				}
				else
				{
					try
					{
						_Nodes.emplace_back((_SNode*)0);

						_Nodes.back() = _New();
						if (!_Nodes.back())
						{
							_Nodes.pop_back();
							return SPairIB(SIterator(), false);
						}
					}
					catch (...)
					{
						return SPairIB(SIterator(), false);
					}

					pNode = _Nodes.back();

					pNode->Index = _Nodes.size() - 1;
				}

				_AttachToNewed(pNode, forward<TArgsEmplace>(Args_)...);

				++_Size;

				return SPairIB(SIterator(pNode), true);	// 중복시 false 반환
			}

			template<typename... TArgsEmplace>
			SPairIB		emplace_at(size_t Index_, TArgsEmplace&&... Args_)
			{
				_SNode* pNode = nullptr;

				// _Nodes 범위 이내 이면
				if (Index_ < _Nodes.size())
				{
					if (_Nodes[Index_]->Newed)
						return SPairIB(SIterator(), false);

					pNode = _Nodes[Index_];

					_DetachFromDeleted(pNode);
				}
				else
				{
					auto LastSize = _Nodes.size();

					try
					{
						for (auto i = LastSize; i < Index_ + 1; ++i)
						{
							_Nodes.emplace_back((_SNode*)0);

							_Nodes.back() = _New();
							if (!_Nodes.back())
							{
								_Nodes.pop_back();
								return SPairIB(SIterator(), false);
							}

							_Nodes.back()->Index = _Nodes.size() - 1;

							if (i == Index_)
							{
								pNode = _Nodes.back();
								break;
							}

							_AttachToDeleted(_Nodes.back());
						}
					}
					catch (...)
					{
						return SPairIB(SIterator(), false);
					}
				}

				_AttachToNewed(pNode, forward<TArgsEmplace>(Args_)...);

				++_Size;

				return SPairIB(SIterator(pNode), true);	// 중복시 false 반환
			}

		private:
			void		_erase(_SNode* pNode_)
			{
				if (!pNode_->Newed)
					return;

				_Detach(pNode_);

				if (!pNode_->pPrev)
					_pNewedHead = pNode_->pNext;
				if (!pNode_->pNext)
					_pNewedTail = pNode_->pPrev;


				// AttachToDeleted ////////////////////
				_AttachToDeleted(pNode_);

				--_Size;

				if (Call_)
				{
					pNode_->Constructed = false;
					pNode_->Data.~TData();
				}
			}

		public:
			void		erase(SIterator& Iterator_)
			{
				_erase(Iterator_.Node());
			}
			void		erase(size_t Index_)
			{
				if (Index_ < 0 ||
					Index_ >= _Nodes.size())
					return;

				_erase(_Nodes[Index_]);
			}
			void		clear(void)	// no thread safe
			{
				for (auto it = begin();
					it.Valid();)
				{
					auto itDel = it;
					++it;

					erase(itDel);
				}
			}
		};
	}
}