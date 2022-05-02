#pragma once


#include <Rso/Core/Core.h>
#include <set>


namespace rso
{
	namespace base
	{
		using namespace std;
		using namespace core;

		template<typename TType>
		struct _SBaseSubComparer
		{
			bool operator()(const TType& mid_, const TType& lhs_, const TType& rhs_) const
			{
				return (mid_ - lhs_ < rhs_ - mid_);
			}
		};

		template<typename TDatas, typename TSubComparer = _SBaseSubComparer<typename TDatas::key_type>>
		class CBidirection : public TDatas
		{
		public:
			using key_type = typename TDatas::key_type;
			using value_type = typename TDatas::value_type;
			using const_iterator = typename TDatas::const_iterator;
			using iterator = typename TDatas::iterator;

			class bidirection_iterator
			{
				template<bool>
				inline typename const TDatas::key_type& _get_key(typename TDatas::const_iterator it_)
				{
					return *it_;
				}
				template<>
				inline typename const TDatas::key_type& _get_key<false>(typename TDatas::const_iterator it_)
				{
					return it_->first;
				}

				key_type _Key{};
				const TDatas* _Datas = nullptr;
				typename TDatas::const_iterator Up;
				typename TDatas::const_iterator Down;
				typename TDatas::const_iterator Cur;

			public:
				bidirection_iterator()
				{
				}
				bidirection_iterator(const_iterator it_, const TDatas* Datas_) :
					_Key(it_->first),
					_Datas(Datas_),
					Up(it_),
					Down(it_),
					Cur(it_)
				{
					if (Cur != Datas_->end())
					{
						++Down;

						if (Cur == Datas_->begin())
							Up = Datas_->end();
						else
							--Up;

						this->operator++();
					}
				}
				bidirection_iterator(const key_type& Key_, const TDatas* Datas_) :
					_Key(Key_),
					_Datas(Datas_)
				{
					Up = Datas_->lower_bound(Key_);
					if (Up == Datas_->end())
					{
						auto rb = Datas_->rbegin();
						if (rb == Datas_->rend())
							Down = Datas_->end();
						else
							Down = (++rb).base();
					}
					else
					{
						Down = Up;
						if (Down == Datas_->begin())
							Down = Datas_->end();
						else
							--Down;
					}

					this->operator++();
				}
				inline const value_type& operator*() const
				{
					return *Cur;
				}
				inline typename TDatas::const_iterator operator->() const
				{
					return Cur;
				}
				bidirection_iterator& operator++()
				{
					if (Up == _Datas->end())
					{
						if (Down == _Datas->end())
							Cur = _Datas->end();
						else
						{
							Cur = Down;
							if (Down == _Datas->begin())
								Down = _Datas->end();
							else
								--Down;
						}
					}
					else
					{
						if (Down == _Datas->end())
						{
							Cur = Up++;
						}
						else
						{
							if (TSubComparer()(_Key, _get_key<is_set<TDatas>::value>(Down), _get_key<is_set<TDatas>::value>(Up)))
							{
								Cur = Down;
								if (Down == _Datas->begin())
									Down = _Datas->end();
								else
									--Down;
							}
							else
							{
								Cur = Up++;
							}
						}
					}

					return *this;
				}
				bidirection_iterator operator++(int)
				{
					auto Ret = *this;
					++(*this);
					return Ret;
				}
				inline bool operator == (const_iterator Iterator_) const
				{
					return (Cur == Iterator_);
				}
				inline bool operator != (const_iterator Iterator_) const
				{
					return !(*this == Iterator_);
				}
			};
			inline bidirection_iterator nearest(const key_type& Key_) const
			{
				return bidirection_iterator(Key_, this);
			}
			inline bidirection_iterator nearest(const_iterator it_) const
			{
				return bidirection_iterator(it_, this);
			}
		};
	}
}