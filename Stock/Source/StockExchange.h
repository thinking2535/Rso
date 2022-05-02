#pragma once


#include "Asset.h"


namespace rso
{
	namespace stock
	{
		class CStockExchange : private CAsset
		{
			enum class _EType
			{
				Order,
				Change,
				Cancel,
				Max,
				Null
			};

			TOrderCallback _OrderCallback;
			TChangeCallback _ChangeCallback;
			TCancelCallback _CancelCallback;
			TTradeCallback _TradeCallback;
			milliseconds _Delay;
			bool _Busy = false;
			__int64 _OrderCounter = 0;
			__int64 _OrderedNo = 0;
			__int64 _OrderType = EOrderType::Null;
			TRmns _Offers;
			TRmns _Bids;
			size_t _Offer0 = 5;
			size_t _Bid0 = 5;
			_EType _Type = _EType::Null;
			SOrder _PreOrder;
			TTime _OrderProcessTime;
			TTime _CurTime;
			TValue _PreRemain = 0; // 해당 호가의 내 잔량보다 이전에 있던 잔량
			TSqlUSmallInt _OldDay = 0;

			bool _EmptyOffer(void) const
			{
				return (_Offer0 == 5);
			}
			bool _EmptyBid(void) const
			{
				return (_Bid0 == 5);
			}
			bool _EmptyRemain(void) const
			{
				return (_EmptyOffer() || _EmptyBid());
			}
			void _Clear(void);
			void _Trade(__int64 OrderNo_, TValue Price_, TValue Volume_);
			void _Order(void);
			void _Change(void);
			void _Cancel(void);
			void _ResetPreRemain(void);

		public:
			CStockExchange(
				TOrderCallback OrderCallback_,
				TChangeCallback ChangeCallback_,
				TCancelCallback CancelCallback_,
				TTradeCallback TradeCallback_,
				milliseconds Delay_);
			TTime CurTime(void) const
			{
				return _CurTime;
			}
			const TRmns& Offers(void) const
			{
				return _Offers;
			}
			const TRmns& Bids(void) const
			{
				return _Bids;
			}
			const SRmn& Offers0(void) const
			{
				return _Offers[_Offer0];
			}
			const SRmn& Bids0(void) const
			{
				return _Bids[_Bid0];
			}
			void Set(const STick& Tick_);
			void SetRemain(const SRemain& Remain_);
			bool CanOrder(void) const;
			bool OrderBegin(const SOrder& Order_);
			bool ChangeBegin(__int64 OrgOrderNo_, const SOrder& Order_);
			bool CancelBegin(__int64 OrgOrderNo_, TValue Volume_);
			void Proc(void);
		};
	}
}
