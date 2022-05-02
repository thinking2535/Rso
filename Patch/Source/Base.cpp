#include "Base.h"

namespace rso
{
	namespace patch
	{
		bool operator == (const SVersion& Lhs_, const SVersion& Rhs_)
		{
			return (Lhs_.Main == Rhs_.Main && Lhs_.Sub == Rhs_.Sub);
		}
	}
}