#pragma once


#include "Base.h"
#include <tuple>


namespace rso
{
	namespace core
	{
		using namespace std;

		template<typename... TTypes>
		class CTuple : public tuple<TTypes...>
		{
			template<size_t Index_>
			void operator += (const CTuple& rhs_)
			{
				get<Index_>(*this) += get<Index_>(rhs_);
				operator +=<Index_ + 1>(rhs_);
			}
			template<>
			void operator +=<tuple_size<tuple<TTypes...>>::value - 1> (const CTuple& rhs_)
			{
				get<tuple_size<tuple<TTypes...>>::value - 1>(*this) += get<tuple_size<tuple<TTypes...>>::value - 1>(rhs_);
			}
			template<size_t Index_>
			void operator -= (const CTuple& rhs_)
			{
				get<Index_>(*this) -= get<Index_>(rhs_);
				operator -=<Index_ + 1>(rhs_);
			}
			template<>
			void operator -=<tuple_size<tuple<TTypes...>>::value - 1> (const CTuple& rhs_)
			{
				get<tuple_size<tuple<TTypes...>>::value - 1>(*this) -= get<tuple_size<tuple<TTypes...>>::value - 1>(rhs_);
			}
			template<size_t Index_>
			void operator *= (const CTuple& rhs_)
			{
				get<Index_>(*this) *= get<Index_>(rhs_);
				operator *=<Index_ + 1>(rhs_);
			}
			template<>
			void operator *=<tuple_size<tuple<TTypes...>>::value - 1> (const CTuple& rhs_)
			{
				get<tuple_size<tuple<TTypes...>>::value - 1>(*this) *= get<tuple_size<tuple<TTypes...>>::value - 1>(rhs_);
			}
			template<size_t Index_>
			void operator /= (const CTuple& rhs_)
			{
				get<Index_>(*this) /= get<Index_>(rhs_);
				operator /=<Index_ + 1>(rhs_);
			}
			template<>
			void operator /=<tuple_size<tuple<TTypes...>>::value - 1> (const CTuple& rhs_)
			{
				get<tuple_size<tuple<TTypes...>>::value - 1>(*this) /= get<tuple_size<tuple<TTypes...>>::value - 1>(rhs_);
			}


		public:
			CTuple operator += (const CTuple& rhs_)
			{
				operator += <0>(rhs_);
				return *this;
			}
			CTuple operator + (const CTuple& rhs_)
			{
				CTuple ret(*this);
				ret.operator += <0>(rhs_);
				return ret;
			}
			CTuple operator -= (const CTuple& rhs_)
			{
				operator -= <0>(rhs_);
				return *this;
			}
			CTuple operator - (const CTuple& rhs_)
			{
				CTuple ret(*this);
				ret.operator -= <0>(rhs_);
				return ret;
			}
			CTuple operator *= (const CTuple& rhs_)
			{
				operator *= <0>(rhs_);
				return *this;
			}
			CTuple operator * (const CTuple& rhs_)
			{
				CTuple ret(*this);
				ret.operator *= <0>(rhs_);
				return ret;
			}
			CTuple operator /= (const CTuple& rhs_)
			{
				operator /= <0>(rhs_);
				return *this;
			}
			CTuple operator / (const CTuple& rhs_)
			{
				CTuple ret(*this);
				ret.operator /= <0>(rhs_);
				return ret;
			}
		};
	}
}