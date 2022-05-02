#include "Core.h"


namespace rso
{
	namespace core
	{
		class CCalculator
		{
			static double _Do(size_t& Index_, const string& Formula_);
		public:
			static double Do(const string& Formula_);
		};
	}
}
