#pragma once

#include <Rso/Core/Core.h>
#include <vector>
#include <new>
#include "ConstructDestruct.h"

namespace rso::base
{
	using namespace core;
	using namespace std;

	template<typename TData, typename TConstructDestruct = SFullConstructDestruct<TData>>
	class _CQueue
	{
	private:
		using _TDatas = vector<TData*>;
		static const size_t c_Index_Null = size_t(~0);

		_TDatas _Nodes;
		size_t _Size = 0;
		size_t _Front = c_Index_Null;
		size_t _Back = c_Index_Null;
		size_t _Frag = c_Index_Null;

	public:
		class const_iterator
		{
		protected:
			_CQueue* _Queue{ nullptr };
			size_t _Index{ c_Index_Null };

		public:
			const_iterator(_CQueue* Queue_, size_t Index_) :
				_Queue(Queue_), _Index(Index_)
			{
			}
			inline const TData& operator*() const
			{
				return *_Queue->_Nodes[_Index];
			}
			inline const TData* operator->() const
			{
				return _Queue->_Nodes[_Index];
			}
			bool operator != (const const_iterator& Iterator_) const
			{
				if (_Queue != Iterator_._Queue ||
					_Index != Iterator_._Index)
					return true;

				return false;
			}
			const_iterator operator++()
			{
				if (_Queue->_Frag == c_Index_Null)
				{
					if (_Index == _Queue->_Back)
					{
						_Index = c_Index_Null;
					}
					else
					{
						++_Index;
						if (_Index >= _Queue->_Nodes.size())
							_Index = 0;
					}
				}
				else
				{
					if (_Index < _Queue->_Frag)
					{
						if (_Index == _Queue->_Back)
						{
							_Index = _Queue->_Frag;
						}
						else
						{
							++_Index;
							if (_Index >= _Queue->_Frag)
								_Index = 0;
						}
					}
					else
					{
						if (_Index + 1 == _Queue->_Nodes.size())
							_Index = c_Index_Null;
						else
							++_Index;
					}
				}

				return *this;
			}
			const_iterator operator++(int)
			{
				auto Ret = *this;
				++(*this);
				return Ret;
			}
		};
		class iterator : public const_iterator
		{
		public:
			iterator(_CQueue* Queue_, size_t Index_) :
				const_iterator(Queue_, Index_)
			{
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

		_CQueue()
		{
		}
		template<typename... TArgsEmplace>
		_CQueue(size_t Size_, TArgsEmplace&&... Args_)
		{
			resize(Size_, forward<TArgsEmplace>(Args_)...);
		}
		_CQueue(const _CQueue& Var_)
		{
			for (auto& it : Var_)
				emplace(it);
		}
		_CQueue(_CQueue&& Var_) :
			_Nodes(std::move(Var_._Nodes)),
			_Size(Var_._Size),
			_Front(Var_._Front),
			_Back(Var_._Back),
			_Frag(Var_._Frag)
		{
			Var_._Size = 0;
			Var_._Front = c_Index_Null;
			Var_._Back = c_Index_Null;
			Var_._Frag = c_Index_Null;
		}
		virtual ~_CQueue()
		{
			clear();

			for (auto& pNode : _Nodes)
			{
				TConstructDestruct::LastDestruct(pNode);
				operator delete(pNode);
			}
		}
		void reserve(size_t Size_)
		{
			if (_Size > 0)
				THROWEX();

			auto OldSize = _Nodes.size();

			if (Size_ <= OldSize)
				return;

			_Nodes.resize(Size_);

			for (size_t i = OldSize; i < Size_; ++i)
				_Nodes[i] = (TData*) operator new(sizeof(TData));
		}
		template<typename... TArgsEmplace>
		void resize(size_t Size_, TArgsEmplace&&... Args_)
		{
			if (_Size > 0)
				return;

			for (size_t i = 0; i < Size_; ++i)
			{
				if (!emplace(forward<TArgsEmplace>(Args_)...))
					THROWEX();
			}
		}
		_CQueue& operator = (const _CQueue& Var_)
		{
			this->~_CQueue();
			new (this) _CQueue(Var_);
		}
		_CQueue& operator = (_CQueue&& Var_)
		{
			this->~_CQueue();
			new (this) _CQueue(std::move(Var_));
		}
		inline size_t size(void) const
		{
			return _Size;
		}
		inline bool empty(void) const
		{
			return (_Front == c_Index_Null);
		}
		template<typename... TArgsEmplace>
		void emplace(TArgsEmplace&&... Args_)
		{
			auto NewBack = c_Index_Null;	// null : need to emplace

			if (!(_Frag != c_Index_Null ||
				_Nodes.size() == 0 ||
				(_Front != c_Index_Null && (_Back + 1) % _Nodes.size() == _Front)
				))
				NewBack = (_Back + 1) % _Nodes.size();

			if (NewBack == c_Index_Null)
			{
				_Nodes.emplace_back(nullptr);

				try
				{
					_Nodes.back() = (TData*)operator new(sizeof(TData));

					try
					{
						TConstructDestruct::FirstConstruct(_Nodes.back(), forward<TArgsEmplace>(Args_)...);
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

				if (_Front == c_Index_Null)
					_Front = _Back = 0;
				else if (_Frag == c_Index_Null)
				{
					if (_Front == 0)
						++_Back;
					else
						_Frag = _Nodes.size() - 1;
				}
			}
			else
			{
				TConstructDestruct::Construct(_Nodes[NewBack], forward<TArgsEmplace>(Args_)...);

				if (_Front == c_Index_Null)
					_Front = _Back = 0;
				else
					_Back = NewBack;
			}

			++_Size;
		}
		void pop(void)
		{
			if (_Front == c_Index_Null)
				return;

			TConstructDestruct::Destruct(_Nodes[_Front]);

			if (_Frag == c_Index_Null)
			{
				if (_Back < _Front)
				{
					++_Front;
					_Front %= _Nodes.size();
				}
				else if (_Front == _Back)
				{
					_Front = _Back = c_Index_Null;
				}
				else
				{
					++_Front;
				}
			}
			else
			{
				if (_Back < _Front)
				{
					++_Front;
					_Front %= _Frag;
				}
				else if (_Front == _Back)
				{
					_Front = _Frag;
					_Frag = c_Index_Null;
					_Back = _Nodes.size() - 1;
				}
				else
				{
					++_Front;
				}
			}

			--_Size;
		}
		inline TData& front(void)
		{
			return *_Nodes[_Front];
		}
		inline const TData& front(void) const
		{
			return *_Nodes[_Front];
		}
		TData& back(void)
		{
			if (_Frag == c_Index_Null)
				return *_Nodes[_Back];
			else
				return *_Nodes.back();
		}
		const TData& back(void) const
		{
			if (_Frag == c_Index_Null)
				return *_Nodes[_Back];
			else
				return *_Nodes.back();
		}
		inline const const_iterator begin(void) const
		{
			return const_iterator(this, _Front);
		}
		inline const const_iterator end(void) const
		{
			return const_iterator(this, c_Index_Null);
		}
		inline iterator begin(void)
		{
			return iterator(this, _Front);
		}
		inline iterator end(void)
		{
			return iterator(this, c_Index_Null);
		}
		TData& operator[](size_t Index_) // size() 에 맞지 않는 Index_ 는 외부 책임
		{
			if (_Frag == c_Index_Null)
			{
				if (_Front + Index_ < _Nodes.size())
					return *_Nodes[_Front + Index_];
				else
					return *_Nodes[_Front + Index_ - _Nodes.size()];
			}
			else
			{
				if (_Back < _Front)
				{
					if (_Front + Index_ < _Frag)
						return *_Nodes[_Front + Index_];
					else if (_Front + Index_ - _Frag < _Back + 1)
						return *_Nodes[_Front + Index_ - _Frag];
					else
						return *_Nodes[_Front + Index_ - _Back - 1];
				}
				else
				{
					if (_Front + Index_ < _Back + 1)
						return *_Nodes[_Front + Index_];
					else
						return *_Nodes[_Front + Index_ + _Frag - _Back - 1];
				}
			}
		}
		inline void clear(void)
		{
			for (; _Front != c_Index_Null; pop());
		}
	};

	template<typename TData>
	class CQueue : public _CQueue<TData, SFullConstructDestruct<TData>>
	{
	};

	template<typename TData>
	class CQueueB : public _CQueue<TData, SLazyConstructDestruct<TData>>
	{
	};
}