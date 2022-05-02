#include "Base.h"


namespace rso
{
    namespace math
    {
        bool IsPrime(unsigned __int64 Num_)
        {
            if (Num_ % 2 == 0)
                return false;

            for (unsigned __int64 i = 3; i * i <= Num_; i += 2)
                if (Num_%i == 0)
                    return false;

            return true;
        }
        double NormalDistribution(double X_, double M_, double S_)
        {
            return (1.0 / (S_ * sqrt(2 * c_PI))) * exp(-0.5 * pow((X_ - M_) / S_, 2.0));
        }
        double SimpleNormalDistribution(double X_, double A_, double B_)
        {
            return (1.0 / 1.0 + pow(A_ * (X_ - B_), 2.0));
        }
        double Sigmoid(double X_)
        {
            return (1.0 / (1.0 + exp(-X_)));
        }
		double DerivativeSigmoid(double X_)
		{
			auto f = Sigmoid(X_);
			return f * (1.0 - f);
		}
    }
}
