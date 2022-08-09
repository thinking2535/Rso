#pragma once

// �⺻������ �̸��� ��ũ �̹Ƿ� Key�� ������������ �����Ͽ��� �� ���� ó���� �̻��̸� ù ���� �����.
// ������������ �����Ͽ� Key�� ó���� �����̸� ù ���� ����Ϸ��� �����ڷ� greater<> �� �����.

#include "Base.h"

namespace rso::gameutil
{
	using namespace std;

	template<typename _TKey, typename _TValue, typename _TOp = less<_TKey>>
	class CClosedRank : public map<_TKey, _TValue, _TOp>
	{
	public:
		using typename map<_TKey, _TValue, _TOp>::const_iterator;
		using typename map<_TKey, _TValue, _TOp>::iterator;

		const_iterator get(_TKey Key_) const
		{
			auto it = lower_bound(Key_);
			if (it == end() && !empty())
				--it;

			return it;
		}
		iterator get(_TKey Key_)
		{
			auto it = lower_bound(Key_);
			if (it == end() && !empty())
				--it;

			return it;
		}
	};
}