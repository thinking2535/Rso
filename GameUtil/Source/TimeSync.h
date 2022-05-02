#pragma once

// ������ Ŭ���̾�Ʈ �� ��Ʈ��ũ ���� ���ο� ������� Ŭ���̾�Ʈ���� ������ �ؾ� �ϴ°�쿡 Ŭ���̾�Ʈ�� ������ ó������� ���ƾ� �ϹǷ� �������� ó���ð��� Ŭ�� �����ؾ� �ϴµ�
// �̷����� Time ���� ������ ���� ��ġ Ŭ������ 

#include "Base.h"

namespace rso::gameutil
{
	using namespace core;

	class CTimeSync
	{
		TTime _RemoteTime = steady_now();
		milliseconds _Error{}; // Ŭ���̾�Ʈ�� ���������� ����� ������ Ŭ���̾�Ʈ�� �ð��� ���� ���� ��쿡 ���� ��� ����

	public:
		CTimeSync(const milliseconds& Error_);
		bool Sync(const TTime& RemoteTime_);
	};
}