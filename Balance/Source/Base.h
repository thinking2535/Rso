#pragma once

#include <Rso/Core/Base.h>

#pragma comment( lib, "Rso/Balance_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include "Protocol.h"

namespace rso
{
	namespace balance
	{
		struct SCapacityAvgClientCnt
		{
			TPeerCnt operator()(const SCapacity& Capacity)
			{
				return (Capacity.ServerCnt < 0 ? 0 : Capacity.ClientCnt / Capacity.ServerCnt);
			}
		};

		inline SCapacity operator += (SCapacity& Lhs_, const SCapacity& Rhs_)
		{
			Lhs_.ClientCnt += Rhs_.ClientCnt;
			Lhs_.ServerCnt += Rhs_.ServerCnt;
			return Lhs_;
		}
		inline SCapacity operator + (const SCapacity& Lhs_, const SCapacity& Rhs_)
		{
			SCapacity Ret = Lhs_;
			return Ret += Rhs_;
		}
		inline SCapacity operator -= (SCapacity& Lhs_, const SCapacity& Rhs_)
		{
			Lhs_.ClientCnt -= Rhs_.ClientCnt;
			Lhs_.ServerCnt -= Rhs_.ServerCnt;
			return Lhs_;
		}
		inline SCapacity operator - (const SCapacity& Lhs_, const SCapacity& Rhs_)
		{
			SCapacity Ret = Lhs_;
			return Ret -= Rhs_;
		}
		inline bool operator < (const SCapacity& Lhs_, const SCapacity& Rhs_)
		{
			return SCapacityAvgClientCnt()(Lhs_) < SCapacityAvgClientCnt()(Rhs_);
		}
	}
}
