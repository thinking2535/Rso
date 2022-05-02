#pragma once

// 서버와 클라이언트 간 네트워크 연결 여부에 상관없이 클라이언트에서 진행을 해야 하는경우에 클라이언트와 서버간 처리결과가 같아야 하므로 프로토콜 처리시각을 클라가 결정해야 하는데
// 이로인한 Time 조작 방지를 위한 장치 클래스로 

#include "Base.h"

namespace rso::gameutil
{
	using namespace core;

	class CTimeSync
	{
		TTime _RemoteTime = steady_now();
		milliseconds _Error{}; // 클라이언트는 정상이지만 기계적 오차로 클라이언트의 시간이 빨리 가는 경우에 대한 허용 오차

	public:
		CTimeSync(const milliseconds& Error_);
		bool Sync(const TTime& RemoteTime_);
	};
}