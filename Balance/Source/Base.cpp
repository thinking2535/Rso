#include "Base.h"

namespace rso
{
	namespace balance
	{
		SCapacity operator += (SCapacity& Lhs_, const SCapacity& Rhs_)
		{
			Lhs_.ClientCnt += Rhs_.ClientCnt;
			Lhs_.ServerCnt += Rhs_.ServerCnt;
			return Lhs_;
		}
		SCapacity operator + (const SCapacity& Lhs_, const SCapacity& Rhs_)
		{
			SCapacity Ret = Lhs_;
			return Ret += Rhs_;
		}
		SCapacity operator -= (SCapacity& Lhs_, const SCapacity& Rhs_)
		{
			Lhs_.ClientCnt -= Rhs_.ClientCnt;
			Lhs_.ServerCnt -= Rhs_.ServerCnt;
			return Lhs_;
		}
		SCapacity operator - (const SCapacity& Lhs_, const SCapacity& Rhs_)
		{
			SCapacity Ret = Lhs_;
			return Ret -= Rhs_;
		}
	}
}
