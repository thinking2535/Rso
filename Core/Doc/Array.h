#pragma once


#include "Base.h"
#include <array>


namespace rso
{
	namespace core
	{
		using namespace std;

		template<typename TType, size_t Size_>
		class CArray : public array<TType, Size_>
		{
		public:
			CArray operator += (const CArray& rhs_)
			{
                for (size_t i = 0; i < Size_; ++i)
                    this->operator[](i) += rhs_.operator[](i);

				return *this;
			}
			CArray operator + (const CArray& rhs_)
			{
				CArray ret(*this);
                return (ret += rhs_);
			}
            CArray operator -= (const CArray& rhs_)
            {
                for (size_t i = 0; i < Size_; ++i)
                    this->operator[](i) -= rhs_.operator[](i);

                return *this;
            }
            CArray operator - (const CArray& rhs_)
            {
                CArray ret(*this);
                return (ret -= rhs_);
            }
            CArray operator *= (const CArray& rhs_)
            {
                for (size_t i = 0; i < Size_; ++i)
                    this->operator[](i) *= rhs_.operator[](i);

                return *this;
            }
            CArray operator * (const CArray& rhs_)
            {
                CArray ret(*this);
                return (ret *= rhs_);
            }
            CArray operator /= (const CArray& rhs_)
            {
                for (size_t i = 0; i < Size_; ++i)
                    this->operator[](i) /= rhs_.operator[](i);

                return *this;
            }
            CArray operator / (const CArray& rhs_)
            {
                CArray ret(*this);
                return (ret /= rhs_);
            }

            CArray operator += (const TType& rhs_)
            {
                for (size_t i = 0; i < Size_; ++i)
                    this->operator[](i) += rhs_;

                return *this;
            }
            CArray operator + (const TType& rhs_)
            {
                CArray ret(*this);
                return (ret += rhs_);
            }
            CArray operator -= (const TType& rhs_)
            {
                for (size_t i = 0; i < Size_; ++i)
                    this->operator[](i) -= rhs_;

                return *this;
            }
            CArray operator - (const TType& rhs_)
            {
                CArray ret(*this);
                return (ret -= rhs_);
            }
            CArray operator *= (const TType& rhs_)
            {
                for (size_t i = 0; i < Size_; ++i)
                    this->operator[](i) *= rhs_;

                return *this;
            }
            CArray operator * (const TType& rhs_)
            {
                CArray ret(*this);
                return (ret *= rhs_);
            }
            CArray operator /= (const TType& rhs_)
            {
                for (size_t i = 0; i < Size_; ++i)
                    this->operator[](i) /= rhs_;

                return *this;
            }
            CArray operator / (const TType& rhs_)
            {
                CArray ret(*this);
                return (ret /= rhs_);
            }
        };
	}
}