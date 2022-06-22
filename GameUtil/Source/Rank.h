#pragma once

// �⺻������ �̸��� ��ũ �̹Ƿ� Key�� ������������ �����Ͽ��� �� ���� ó���� �̻��̸� ù ���� �����.
// ������������ �����Ͽ� Key�� ó���� �����̸� ù ���� ����Ϸ��� �����ڷ� greater<> �� �����.

#include "Base.h"

namespace rso
{
	namespace gameutil
	{
		using namespace std;

		template<typename _TKey, typename _TValue, typename _TOp = less<_TKey>>
		class CRank : public map<_TKey, _TValue, _TOp>
		{
		public:
			using typename map<_TKey, _TValue, _TOp>::const_iterator;
			using typename map<_TKey, _TValue, _TOp>::iterator;

			const_iterator get(_TKey Key_) const
			{
				auto it = upper_bound(Key_); // �Ǽ��� Ű�� ���� ��� 0 ���� �����ϱ� ������ 1.0 �ϰ�� 0 <= Key < 1 �� �Ǿ���ϹǷ�, upper_bound �� ó��
				if (it == end() && !empty())
					--it;

				return it;
			}
			iterator get(_TKey Key_)
			{
				auto it = upper_bound(Key_); // �Ǽ��� Ű�� ���� ��� 0 ���� �����ϱ� ������ 1.0 �ϰ�� 0 <= Key < 1 �� �Ǿ���ϹǷ�, upper_bound �� ó��
				if (it == end() && !empty())
					--it;

				return it;
			}
		};
	}
}