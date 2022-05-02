#pragma once


#include <new>
#include <vector>


namespace rso
{
	namespace base
	{
		using namespace std;

		template<typename TKey, typename TData, bool Call_ = true>
		class CMap
		{
		private:
			struct _SNode
			{
				size_t		Index;
				_SNode*		Next;
				_SNode*		Prev;
				_SNode*		Parent;
				_SNode*		Left;
				_SNode*		Right;
				__int32		Balance;
				bool		Newed;
				bool		Constructed;
				pair<TKey, TData>	Value;
			};
			using	_TNodes = vector<_SNode*>;

			_TNodes		_Nodes;
			_SNode*		_DeletedHead{ nullptr };
			_SNode*		_DeletedTail{ nullptr };
			_SNode*		_NewedHead{ nullptr };
			_SNode*		_NewedTail{ nullptr };
			_SNode*		_Root{ nullptr };
			size_t		_Size{ 0 };

		public:
			struct SIterator
			{
			private:
				_SNode*	_Node{ nullptr };

			public:
				SIterator() {}
				SIterator(_SNode* Node_) : _Node(Node_)
				{
				}
				pair<TKey, TData>&		operator*()
				{
					return _Node->Value;
				}
				pair<TKey, TData>*		operator->()
				{
					return &_Node->Value;
				}
				SIterator	operator++()
				{
					_Node = _Node->Next;

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
					return _Node;
				}
				size_t	Index(void) const
				{
					return _Node->Index;
				}
				bool	Valid(void) const
				{
					return (_Node != 0);
				}
				bool	operator != (const SIterator& Iterator_)
				{
					return (_Node != Iterator_._Node);
				}
			};
			using SPairIB = pair<SIterator, bool>;

			CMap()
			{
			}
			CMap(const CMap& Var_)
			{
				for (auto it = Var_.begin(); it != Var_.end(); ++it)
					emplace_at(it.Index(), (*it));
			}
			~CMap()
			{
				for (auto& Node : _Nodes)
				{
					if (Node->Constructed)
						Node->Value.second.~TData();
				}

				for (auto& Node : _Nodes)
					operator delete(Node, nothrow);
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
			CMap	operator = (const CMap& Var_) = delete;
			CMap	operator = (CMap&& Var_) = delete;

			pair<TKey, TData>*		operator[](size_t Index_) const
			{
				if (Index_ < 0 ||
					Index_ >= _Nodes.size() ||
					!_Nodes[Index_]->Newed)
					return nullptr;

				return &_Nodes[Index_]->Value;
			}

			SIterator		get(size_t Index_) const
			{
				if (Index_ < 0 ||
					Index_ >= _Nodes.size() ||
					!_Nodes[Index_]->Newed)
					return SIterator();

				return SIterator(_Nodes[Index_]);
			}
		public:
			__inline SIterator	find(const TKey& Key_) const
			{
				auto* Tree = _Root;
				while (Tree)
				{
					if (Key_ == Tree->Value.first) return SIterator(Tree);
					else if (Key_ < Tree->Value.first) Tree = Tree->Left;
					else Tree = Tree->Right;
				}

				return SIterator(nullptr);
			}
			SIterator	begin(void) const	// for just iterate
			{
				return SIterator(_NewedHead);
			}
			SIterator	end(void) const	// for just iterate
			{
				return SIterator();
			}

			size_t		size(void) const { return _Size; }

		private:
			_SNode*		_New(void)
			{
				auto* Node = (_SNode*) operator new(sizeof(_SNode), nothrow);
				if (!Node)
					return nullptr;

				Node->Constructed = false;
				
				return Node;
			}
			void	LLRot(_SNode* Node_)
			{
				auto* TempLeft = Node_->Left;
				Node_->Left = TempLeft->Right;

				if (TempLeft->Right)
					TempLeft->Right->Parent = Node_;

				auto* TempParent = Node_->Parent;
				TempLeft->Right = Node_;
				Node_->Parent = TempLeft;

				TempLeft->Parent = TempParent;
				if (TempParent)
				{
					if (TempParent->Left == Node_)
						TempParent->Left = TempLeft;
					else
						TempParent->Right = TempLeft;
				}

				if (_Root == Node_)
					_Root = TempLeft;

				if (TempLeft->Balance == 0)
					Node_->Balance = -1;
				else
					Node_->Balance = 0;

				TempLeft->Balance += 1;
			}
			void	RRRot(_SNode* Node_)
			{
				auto* TempRight = Node_->Right;
				Node_->Right = TempRight->Left;

				if (TempRight->Left)
					TempRight->Left->Parent = Node_;

				auto* TempParent = Node_->Parent;
				TempRight->Left = Node_;
				Node_->Parent = TempRight;

				TempRight->Parent = TempParent;
				if (TempParent)
				{
					if (TempParent->Left == Node_)
						TempParent->Left = TempRight;
					else
						TempParent->Right = TempRight;
				}

				if (_Root == Node_)
					_Root = TempRight;

				if (TempRight->Balance == 0)
					Node_->Balance = 1;
				else
					Node_->Balance = 0;

				TempRight->Balance -= 1;
			}
			void	LRRot(_SNode* Node_)
			{
				auto* TempLeft = Node_->Left;
				auto* TempParent = Node_->Parent;
				auto* TmpLeftRight = Node_->Left->Right;

				TempLeft->Right = TmpLeftRight->Left;
				if (TmpLeftRight->Left)
					TmpLeftRight->Left->Parent = TempLeft;

				Node_->Left = TmpLeftRight->Right;
				if (TmpLeftRight->Right)
					TmpLeftRight->Right->Parent = Node_;

				TmpLeftRight->Left = TempLeft;
				TempLeft->Parent = TmpLeftRight;

				TmpLeftRight->Right = Node_;
				Node_->Parent = TmpLeftRight;

				TmpLeftRight->Parent = TempParent;
				if (TempParent)
				{
					if (TempParent->Left == Node_)
						TempParent->Left = TmpLeftRight;
					else
						TempParent->Right = TmpLeftRight;
				}

				if (_Root == Node_)
					_Root = TmpLeftRight;

				if (TmpLeftRight->Balance == 1)
				{
					Node_->Balance = 0;
					TempLeft->Balance = -1;
				}
				else if (TmpLeftRight->Balance == -1)
				{
					Node_->Balance = 1;
					TempLeft->Balance = 0;
				}
				else
				{
					Node_->Balance = 0;
					TempLeft->Balance = 0;
				}
				TmpLeftRight->Balance = 0;
			}
			void	RLRot(_SNode* Node_)
			{
				auto* TempRight = Node_->Right;
				auto* TempParent = Node_->Parent;
				auto* TmpRightLeft = Node_->Right->Left;

				TempRight->Left = TmpRightLeft->Right;
				if (TmpRightLeft->Right)
					TmpRightLeft->Right->Parent = TempRight;

				Node_->Right = TmpRightLeft->Left;
				if (TmpRightLeft->Left)
					TmpRightLeft->Left->Parent = Node_;

				TmpRightLeft->Right = TempRight;
				TempRight->Parent = TmpRightLeft;

				TmpRightLeft->Left = Node_;
				Node_->Parent = TmpRightLeft;

				TmpRightLeft->Parent = TempParent;
				if (TempParent)
				{
					if (TempParent->Right == Node_)
						TempParent->Right = TmpRightLeft;
					else
						TempParent->Left = TmpRightLeft;
				}

				if (_Root == Node_)
					_Root = TmpRightLeft;

				if (TmpRightLeft->Balance == -1)
				{
					Node_->Balance = 0;
					TempRight->Balance = 1;
				}
				else if (TmpRightLeft->Balance == 1)
				{
					Node_->Balance = -1;
					TempRight->Balance = 0;
				}
				else
				{
					Node_->Balance = 0;
					TempRight->Balance = 0;
				}
				TmpRightLeft->Balance = 0;
			}
			template<typename... TArgsEmplace>
			SPairIB		_AttachToNewed(const TKey& Key_, _SNode* Node_, TArgsEmplace&&... Args_)
			{
				// Tree /////////////////////////////////////////////////////
				if (_Root)
				{
					auto* Tree = _Root;
					for (;;)
					{
						if (Key_ == Tree->Value.first)
							return SPairIB(SIterator(Tree), false);

						if (Key_ < Tree->Value.first)
						{
							if (!Tree->Left)
							{
								Tree->Left = Node_;
								if (Tree->Prev)
								{
									Tree->Prev->Next = Node_;
									Node_->Prev = Tree->Prev;
								}
								else
								{
									Node_->Prev = nullptr;
									_NewedHead = Node_;
								}
								Tree->Prev = Node_;
								Node_->Next = Tree;

								break;
							}

							Tree = Tree->Left;
						}
						else
						{
							if (!Tree->Right)
							{
								Tree->Right = Node_;
								if (Tree->Next)
								{
									Tree->Next->Prev = Node_;
									Node_->Next = Tree->Next;
								}
								else
								{
									Node_->Next = nullptr;
									_NewedTail = Node_;
								}
								Tree->Next = Node_;
								Node_->Prev = Tree;

								break;
							}

							Tree = Tree->Right;
						}
					}

					Node_->Parent = Tree;
					Node_->Left = nullptr;
					Node_->Right = nullptr;
					Node_->Balance = 0;
					Node_->Newed = true;
					Node_->Value.first = Key_;

					// Balance ///////////////////////////////////
					int Balances[2] = {};
					auto* ChildNode = Node_;
					do
					{
						auto OldBalance = Tree->Balance;

						if (Tree->Left == ChildNode)
						{
							Balances[0] = -1;
							Tree->Balance -= 1;

							if (Tree->Balance < -1)
							{
								if (Balances[1] == -1)
									LLRot(Tree);
								else
									LRRot(Tree);

								break;
							}
						}
						else
						{
							Balances[0] = 1;
							Tree->Balance += 1;

							if (Tree->Balance > 1)
							{
								if (Balances[1] == -1)
									RLRot(Tree);
								else
									RRRot(Tree);

								break;
							}
						}

						if (OldBalance == Tree->Balance || Tree->Balance == 0)
							break;

						Balances[1] = Balances[0];

						ChildNode = Tree;
						Tree = Tree->Parent;
					} while (Tree);
				}
				else
				{
					_Root = _NewedHead = _NewedTail = Node_;
					Node_->Parent = nullptr;
					Node_->Next = nullptr;
					Node_->Prev = nullptr;
					Node_->Left = nullptr;
					Node_->Right = nullptr;
					Node_->Balance = 0;
					Node_->Newed = true;
					Node_->Value.first = Key_;
				}

				// Construct /////////////////////////////////
				if (!Node_->Constructed)
				{
					Node_->Constructed = true;
					new(&Node_->Value.second)TData(forward<TArgsEmplace>(Args_)...);
				}

				++_Size;

				return SPairIB(SIterator(Node_), true);
			}
			void		_AttachToDeleted(_SNode* Node_)
			{
				Node_->Next = nullptr;
				Node_->Newed = false;

				if (_DeletedTail == 0)
				{
					Node_->Prev = nullptr;
					_DeletedHead = _DeletedTail = Node_;
				}
				else
				{
					Node_->Prev = _DeletedTail;
					_DeletedTail->Next = Node_;
					_DeletedTail = Node_;
				}
			}
			void		_Detach(_SNode* Node_)
			{
				if (Node_->Prev)
					Node_->Prev->Next = Node_->Next;
				if (Node_->Next)
					Node_->Next->Prev = Node_->Prev;
			}
			void		_DetachFromDeleted(_SNode* Node_)
			{
				_Detach(Node_);

				if (!Node_->Prev)
					_DeletedHead = Node_->Next;
				if (!Node_->Next)
					_DeletedTail = Node_->Prev;
			}





			void 	_CleanTree(_SNode* Node_)
			{
				Node_->Parent = nullptr;
				Node_->Left = nullptr;
				Node_->Right = nullptr;
				Node_->Balance = 0;
			}
			void 	_AttachTreeLeft(_SNode* Node_, _SNode* ToNode_)
			{
				Node_->Parent = ToNode_;
				ToNode_->Left = Node_;
			}
			void 	_AttachTreeRight(_SNode* Node_, _SNode* ToNode_)
			{
				Node_->Parent = ToNode_;
				ToNode_->Right = Node_;
			}
			void 	_AttachTree(_SNode* Node_, _SNode* ToNode_)
			{
				if (ToNode_->Parent)
				{
					if (ToNode_->Parent->Left == ToNode_)
						_AttachTreeLeft(Node_, ToNode_->Parent);
					else
						_AttachTreeRight(Node_, ToNode_->Parent);
				}

				if (ToNode_->Left)
					_AttachTreeLeft(ToNode_->Left, Node_);
				if (ToNode_->Right)
					_AttachTreeRight(ToNode_->Right, Node_);

				Node_->Balance = ToNode_->Balance;
			}
			void 	_DetachTree(_SNode* Node_)
			{
				if (Node_->Parent)
				{
					if (Node_->Parent->Left == Node_)
						Node_->Parent->Left = nullptr;
					else
						Node_->Parent->Right = nullptr;
					Node_->Parent = nullptr;
				}

				if (Node_->Left)
				{
					Node_->Left->Parent = nullptr;
					Node_->Left = nullptr;
				}

				if (Node_->Right)
				{
					Node_->Right->Parent = nullptr;
					Node_->Right = nullptr;
				}

				Node_->Balance = 0;
			}
			void 	_ReplaceTree(_SNode* Node_, _SNode* ToNode_)
			{
				_DetachTree(Node_);
				_AttachTree(Node_, ToNode_);
			}










		public:
			template<typename... TArgsEmplace>
			SPairIB	emplace(const TKey& Key_, TArgsEmplace&&... Args_)
			{
				_SNode* Node = nullptr;

				// DetachFromDeleted ////////////////////////////////////
				if (_DeletedHead)
				{
					Node = _DeletedHead;

					_DetachFromDeleted(Node);
				}
				else
				{
					try
					{
						_Nodes.emplace_back(nullptr);

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

					Node = _Nodes.back();

					Node->Index = _Nodes.size() - 1;
				}

				return _AttachToNewed(Key_, Node, forward<TArgsEmplace>(Args_)...);
			}

			template<typename... TArgsEmplace>
			SPairIB		emplace_at(const TKey& Key_, size_t Index_, TArgsEmplace&&... Args_)
			{
				_SNode* Node = nullptr;

				// _Nodes 범위 이내 이면
				if (Index_ < _Nodes.size())
				{
					if (_Nodes[Index_]->Newed)
						return SPairIB(SIterator(), false);

					Node = _Nodes[Index_];

					_DetachFromDeleted(Node);
				}
				else
				{
					auto LastSize = _Nodes.size();

					try
					{
						for (auto i = LastSize; i < Index_ + 1; ++i)
						{
							_Nodes.emplace_back(nullptr);

							_Nodes.back() = _New();
							if (!_Nodes.back())
							{
								_Nodes.pop_back();
								return SPairIB(SIterator(), false);
							}

							_Nodes.back()->Index = _Nodes.size() - 1;

							if (i == Index_)
							{
								Node = _Nodes.back();
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

				return _AttachToNewed(Key_, Node, forward<TArgsEmplace>(Args_)...);
			}

		private:
			void		_erase(_SNode* Node_)
			{
				if (!Node_->Newed)
					return;

				_SNode* RepNode = nullptr;
				_SNode* ScanNode = nullptr;

				if (Node_->Balance > 0)
				{
					RepNode = Node_->Right;
					if (RepNode->Left)
					{
						while (RepNode->Left)
							RepNode = RepNode->Left;

						if (RepNode->Right)
						{
							auto* RepRight = RepNode->Right;

							_ReplaceTree(RepRight, RepNode);
							_CleanTree(RepNode);
							_AttachTree(RepNode, Node_);
							_AttachTreeRight(Node_, RepRight);
						}
						else
						{
							auto* RepParent = RepNode->Parent;

							__int32	iDir = 0;
							if (RepParent->Left == RepNode)
								iDir = -1;
							else
								iDir = 1;

							_ReplaceTree(RepNode, Node_);
							if (iDir == -1)
								_AttachTreeLeft(Node_, RepParent);
							else
								_AttachTreeRight(Node_, RepParent);
						}
					}
					else
					{
						if (RepNode->Right)
						{
							auto* RepRight = RepNode->Right;

							_ReplaceTree(RepRight, RepNode);
							_CleanTree(RepNode);
							_AttachTree(RepNode, Node_);
							_AttachTreeRight(Node_, RepRight);
						}
						else
						{
							auto* RepParent = RepNode->Parent;

							__int32	iDir = 0;
							if (RepParent->Left == RepNode)
								iDir = -1;
							else
								iDir = 1;

							_ReplaceTree(RepNode, Node_);
							if (iDir == -1)
								_AttachTreeLeft(Node_, RepNode);
							else
								_AttachTreeRight(Node_, RepNode);
						}
					}
				}
				else
				{
					if (Node_->Left)
					{
						RepNode = Node_->Left;
						if (RepNode->Right)
						{
							while (RepNode->Right)
								RepNode = RepNode->Right;

							if (RepNode->Left)
							{
								auto* RepLeft = RepNode->Left;

								_ReplaceTree(RepLeft, RepNode);
								_CleanTree(RepNode);
								_AttachTree(RepNode, Node_);
								_AttachTreeLeft(Node_, RepLeft);
							}
							else
							{
								auto* RepParent = RepNode->Parent;

								__int32	iDir = 0;
								if (RepParent->Left == RepNode)
									iDir = -1;
								else
									iDir = 1;

								_ReplaceTree(RepNode, Node_);
								if (iDir == -1)
									_AttachTreeLeft(Node_, RepParent);
								else
									_AttachTreeRight(Node_, RepParent);
							}
						}
						else
						{
							if (RepNode->Left)
							{
								auto* RepLeft = RepNode->Left;

								_ReplaceTree(RepLeft, RepNode);
								_CleanTree(RepNode);
								_AttachTree(RepNode, Node_);
								_AttachTreeLeft(Node_, RepLeft);
							}
							else
							{
								auto* RepParent = RepNode->Parent;

								__int32	iDir = 0;
								if (RepParent->Left == RepNode)
									iDir = -1;
								else
									iDir = 1;

								_ReplaceTree(RepNode, Node_);
								if (iDir == -1)
									_AttachTreeLeft(Node_, RepNode);
								else
									_AttachTreeRight(Node_, RepNode);
							}
						}
					}
				}

				ScanNode = Node_->Parent;


				__int32 iDir = 0;

				if (ScanNode)
				{
					if (ScanNode->Left == Node_)
					{
						ScanNode->Left = nullptr;
						iDir = -1;
					}
					else
					{
						ScanNode->Right = nullptr;
						iDir = 1;
					}
				}

				Node_->Parent = nullptr;
				Node_->Left = nullptr;
				Node_->Right = nullptr;
				Node_->Balance = 0;

				if (Node_ == _Root)
				{
					if (RepNode == 0)
						_Root = nullptr;
					else
						_Root = RepNode;
				}

				while (ScanNode)
				{
					ScanNode->Balance -= iDir;

					if (ScanNode->Balance == -2)
					{
						if (ScanNode->Left)
						{
							switch (ScanNode->Left->Balance)
							{
							case -1:
								LLRot(ScanNode);
								ScanNode = ScanNode->Parent;
								break;

							case 0:
								LLRot(ScanNode);
								goto LABEL_EXITSCAN;

							case 1:
								LRRot(ScanNode);
								ScanNode = ScanNode->Parent;
								break;
							}
						}
					}
					else if (ScanNode->Balance == 2)
					{
						if (ScanNode->Right)
						{
							switch (ScanNode->Right->Balance)
							{
							case 1:
								RRRot(ScanNode);
								ScanNode = ScanNode->Parent;
								break;

							case 0:
								RRRot(ScanNode);
								goto LABEL_EXITSCAN;

							case -1:
								RLRot(ScanNode);
								ScanNode = ScanNode->Parent;
								break;
							}
						}
					}
					else if (ScanNode->Balance == 0)
					{
					}
					else
					{
						goto LABEL_EXITSCAN;
					}

					if (ScanNode->Parent)
					{
						if (ScanNode->Parent->Left == ScanNode)
							iDir = -1;
						else
							iDir = 1;
					}
					ScanNode = ScanNode->Parent;
				}

			LABEL_EXITSCAN:

				_Detach(Node_);

				if (!Node_->Prev)
					_NewedHead = Node_->Next;
				if (!Node_->Next)
					_NewedTail = Node_->Prev;


				// AttachToDeleted ////////////////////
				_AttachToDeleted(Node_);

				--_Size;

				if (Call_)
				{
					Node_->Constructed = false;
					Node_->Value.second.~TData();
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
			void		erase(const TKey& Key_)
			{
				auto itDel = find(Key_);
				if (!itDel.Valid())
					return;

				_erase(itDel.Node());
			}
			void		clear(void)
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