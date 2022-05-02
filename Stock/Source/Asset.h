#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		class CAsset
		{
			TValue _Asset = 0;
			__int64 _OrderNo = 0;	// �ټ��� �ֹ���ȣ�� ���� �� �ֵ��� �����̳ʷ� �����. ����(Change)�� ��� OrgOrderNo ����������.
			TValue _OrderPrice = 0;	// �ټ��� �ֹ���ȣ�� ���� �� �ֵ��� �����̳ʷ� �����.
			TValue _OrderVolume = 0; // �ټ��� �ֹ���ȣ�� ���� �� �ֵ��� �����̳ʷ� �����.
			TValue _OpenInterest = 0;
			int _TradeCnt = 0;

		public:
			TValue OpenInterest(void) const
			{
				return _OpenInterest;
			}
			TValue	VirtualAsset(TValue CurPrice_) const
			{
				return (_Asset + (CurPrice_ * _OpenInterest));
			}
			__int64 OrderNo(void) const
			{
				return _OrderNo;
			}
			TValue OrderPrice(void) const
			{
				return _OrderPrice;
			}
			TValue OrderVolume(void) const
			{
				return _OrderVolume;
			}
			void Order(__int64 OrderNo_, TValue Price_, TValue Volume_);
			void Change(__int64 OrgOrderNo_, __int64 OrderNo_, TValue Price_, TValue Volume_);
			void Cancel(__int64 OrderNo_, TValue Volume_);
			void Trade(__int64 OrderNo_, TValue Price_, TValue Volume_);
			int TradeCnt(void) const;
			void ClearTradeCnt(void);
			void clear(void);
		};
	}
}
