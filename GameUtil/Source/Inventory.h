#pragma once

#include "Base.h"
#include "KeyCnt.h"

namespace rso
{
	namespace gameutil
	{
		template<typename _TIndex, typename _TCode, typename _TCnt, typename _TData>
		class CInventory
		{
		public:
			using TIndex = _TIndex;
			using TCode = _TCode;
			using TCnt = _TCnt;
			using TData = _TData;
			struct SIndexCnt
			{
				_TIndex Index{};
				_TCnt Cnt{};

				SIndexCnt()
				{
				}
				SIndexCnt(const _TIndex& Index_, const _TCnt& Cnt_) :
					Index(Index_), Cnt(Cnt_)
				{
				}
			};
			using TIndexCnts = list<SIndexCnt>;
			struct SItem
			{
				_TData Data{};
				_TIndex Index{};
				_TCode Code{};
				_TCnt Cnt{};

				SItem()
				{
				}
				SItem(const _TData& Data_, const _TIndex& Index_, const _TCode& Code_, const _TCnt& Cnt_) :
					Data(Data_), Index(Index_), Code(Code_), Cnt(Cnt_)
				{
				}
			};
			using TItems = list<SItem>;

		private:
			using _TKeyCnt = CKeyCnt<_TCode, _TCnt>;

		public:
			using TCodeCnt = typename _TKeyCnt::value_type;
			using TCodeCnts = map<_TCode, _TCnt>;

		private:
			struct _SDataIt;
			using _TItems = map<_TIndex, _SDataIt>;
			using _TCodes = multimap<_TCode, typename _TItems::iterator>;
			using _TCodesIt = typename _TCodes::iterator;
			struct _SDataIt
			{
				SItem Item;
				_TCodesIt CodesIt;

				_SDataIt(const SItem& Item_) :
					Item(Item_)
				{
				}
			};

			_TItems _Items;
			_TCodes _Codes;
			_TKeyCnt _KeyCnt;

		public:
			CInventory()
			{
			}
			CInventory(const CInventory& Var_)
			{
				for (auto& it : Var_._Items)
					Set(it.second.Data);
			}
			CInventory& operator = (const CInventory& Var_)
			{
				this->~CInventory();
				new (this) CInventory(Var_);
				return *this;
			}
			bool operator < (const TCodeCnt& rhs_) const
			{
				return (_KeyCnt < rhs_);
			}
			bool operator < (const TCodeCnts& rhs_) const
			{
				return (_KeyCnt < rhs_);
			}
			bool operator < (const _TKeyCnt& rhs_) const
			{
				return (_KeyCnt < rhs_);
			}
			const _TItems& Items(void) const
			{
				return _Items;
			}
			void Set(const SIndexCnt& Item_)
			{
				auto it = _Items.find(Item_.Index);
				if (it == _Items.end())
					return;

				if (Item_.Cnt <= 0)
				{
					_KeyCnt -= TCodeCnts::value_type(it->second.Item.Code, it->second.Item.Cnt);
					_Codes.erase(it->second.CodesIt);
					_Items.erase(it);
				}
				else
				{
					if (it->second.Item.Cnt > Item_.Cnt)
					{
						_KeyCnt -= TCodeCnts::value_type(it->second.Item.Code, it->second.Item.Cnt - Item_.Cnt);
					}
					else if (it->second.Item.Cnt < Item_.Cnt)
					{
						_KeyCnt += TCodeCnts::value_type(it->second.Item.Code, Item_.Cnt - it->second.Item.Cnt);
					}
					
					it->second.Item.Cnt = Item_.Cnt;
				}
			}
			void Set(const TIndexCnts& Items_)
			{
				for (auto& Item : Items_)
					Set(Item);
			}
			void Set(const SItem& Item_)
			{
				if (Item_.Cnt <= 0)
				{
					auto it = _Items.find(Item_.Index);
					if (it != _Items.end())
					{
						_KeyCnt -= TCodeCnts::value_type(it->second.Item.Code, it->second.Item.Cnt);
						_Codes.erase(it->second.CodesIt);
						_Items.erase(it);
					}
				}
				else
				{
					auto ibItems = _Items.emplace(Item_.Index, _SDataIt(Item_));
					if (ibItems.second)
					{
						try
						{
							ibItems.first->second.CodesIt = _Codes.emplace(Item_.Code, ibItems.first);
						}
						catch(...)
						{
							_Items.erase(ibItems.first);
							throw;
						}

						_KeyCnt += TCodeCnts::value_type(Item_.Code, Item_.Cnt);
					}
					else
					{
						if (ibItems.first->second.Item.Code == Item_.Code)
						{
							if (ibItems.first->second.Item.Cnt < Item_.Cnt)
								_KeyCnt += TCodeCnts::value_type(ibItems.first->second.Item.Code, Item_.Cnt - ibItems.first->second.Item.Cnt);
							else if (ibItems.first->second.Item.Cnt > Item_.Cnt)
								_KeyCnt -= TCodeCnts::value_type(ibItems.first->second.Item.Code, ibItems.first->second.Item.Cnt - Item_.Cnt);
						}
						else // 코드가 다르면 CodesIt 재 설정
						{
							auto itCodes = _Codes.emplace(Item_.Code, ibItems.first);

							_KeyCnt += TCodeCnts::value_type(Item_.Code, Item_.Cnt);
							_KeyCnt -= TCodeCnts::value_type(ibItems.first->second.Item.Code, ibItems.first->second.Item.Cnt);
							_Codes.erase(ibItems.first->second.CodesIt);
							ibItems.first->second.CodesIt = itCodes;
						}

						ibItems.first->second.Item = Item_;
					}
				}
			}
			void Set(const TItems& Items_)
			{
				for (auto& Item : Items_)
					Set(Item);
			}
			const SItem* Get(_TIndex Index_) const
			{
				auto it = _Items.find(Index_);
				if (it == _Items.end())
					return nullptr;

				return &it->second.Item;
			}
			bool Get(const TCodeCnts& Items_, TIndexCnts& IndexCnts_) const
			{
				for (auto& Item : Items_)
				{
					_TCnt Cnt = Item.second;
					auto fs = _Codes.equal_range(Item.first);
					for (auto it = fs.first; it != fs.second; ++it)
					{
						if (it->second->second.Item.Cnt < Cnt)
						{
							IndexCnts_.emplace_back(SIndexCnt(it->second->first, it->second->second.Item.Cnt));
							Cnt -= it->second->second.Item.Cnt;
						}
						else
						{
							IndexCnts_.emplace_back(SIndexCnt(it->second->first, Cnt));
							Cnt = 0;
							break;
						}
					}

					if (Cnt > 0) // 수량이 부족하면
						return false;
				}

				return true;
			}
			bool Get(const TCodeCnt& Item_, TIndexCnts& IndexCnts_) const
			{
				TCodeCnts Items;
				Items.emplace(Item_.Code, Item_.Cnt);
				return Get(Items, IndexCnts_);
			}
			void clear(void)
			{
				_Codes.clear();
				_Items.clear();
			}
		};
	}
}