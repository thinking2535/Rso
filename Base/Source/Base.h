// TODO : use cuda

#pragma once

#include <Rso/Core/Core.h>

#pragma comment( lib, "Rso/Base_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <Rso/Core/DateTime.h>
#include <tchar.h>
#include <new>

namespace rso
{
    namespace base
    {
        using namespace std;
        using namespace core;

		using TData = int8;

        const int32 c_LogMsgLen_Max = 2048;

        // 입력 : BIT ( 1, 2, 4, 8, ... )
        template< typename _T >
        struct SBit
        {
            _T Value;

            SBit() : Value(0) {}

            _T operator += (const _T Value_) { return (Value |= Value_); }
            _T operator -= (const _T Value_) { return (Value &= ~Value_); }
        };

        // 입력 : 일반 서수 ( 1, 2, 3, 4, 5, ... )
        template< typename _T >
        struct SBitNum
        {
            _T Value;

            SBitNum() : Value(0) {}
            explicit SBitNum(_T Num_) : Value(1 << Num_) {}

            explicit operator bool() const
            {
                return (Value != 0);
            }

            SBitNum& operator |= (const SBitNum& Rhs_)
            {
                Value |= Rhs_.Value;
                return *this;
            }
            SBitNum& operator &= (const SBitNum& Rhs_)
            {
                Value &= Rhs_.Value;
                return *this;
            }
            SBitNum& operator += (const SBitNum& Rhs_)
            {
                Value |= Rhs_.Value;
                return *this;
            }
            SBitNum& operator -= (const SBitNum& Rhs_)
            {
                Value &= ~Rhs_.Value;
                return *this;
            }
            SBitNum operator | (const SBitNum& Rhs_) const
            {
                SBitNum Ret = *this;
                return (Ret |= Rhs_);
            }
            SBitNum operator & (const SBitNum& Rhs_) const
            {
                SBitNum Ret = *this;
                return (Ret &= Rhs_);
            }
            SBitNum operator + (const SBitNum& Rhs_) const
            {
                SBitNum Ret = *this;
                return (Ret += Rhs_);
            }
            SBitNum operator - (const SBitNum& Rhs_) const
            {
                SBitNum Ret = *this;
                return (Ret -= Rhs_);
            }
            bool operator == (const SBitNum& Rhs_) const
            {
                return (Value == Rhs_.Value);
            }
            bool operator != (const SBitNum& Rhs_) const
            {
                return (Value != Rhs_.Value);
            }

            SBitNum& operator |= (const _T Num_)
            {
                Value |= SBitNum(Num_);
                return *this;
            }
            SBitNum& operator &= (const _T Num_)
            {
                Value &= SBitNum(Num_);
                return *this;
            }
            SBitNum& operator += (const _T Num_)
            {
                Value |= (1 << Num_);
                return *this;
            }
            SBitNum& operator -= (const _T Num_)
            {
                Value &= SBitNum(Num_);
                return *this;
            }
            SBitNum operator | (const _T Num_) const
            {
                SBitNum Ret = *this;
                return (Ret |= SBitNum(Num_));
            }
            SBitNum operator & (const _T Num_) const
            {
                SBitNum Ret = *this;
                return (Ret &= SBitNum(Num_));
            }
            SBitNum operator + (const _T Num_) const
            {
                SBitNum Ret = *this;
                return (Ret += SBitNum(Num_));
            }
            SBitNum operator - (const _T Num_) const
            {
                SBitNum Ret = *this;
                return (Ret -= SBitNum(Num_));
            }

            friend SBitNum operator | (const _T Num_, const SBitNum& RValue_) { return (1 << Num_) | RValue_.Value; }
            friend SBitNum operator & (const _T Num_, const SBitNum& RValue_) { return (1 << Num_) & RValue_.Value; }
            friend bool operator == (const _T Num_, const SBitNum& RValue_) { return ((1 << Num_) == RValue_.Value); }
            friend bool operator != (const _T Num_, const SBitNum& RValue_) { return ((1 << Num_) != RValue_.Value); }
        };

		template< typename _TType >
		struct SNullable
		{
		private:
			unique_ptr<_TType> _Data;

		public:
			SNullable()
			{
			}
			SNullable(const _TType& Data_) :
				_Data(new _TType(Data_))
			{
			}
			SNullable(_TType&& Data_) :
				_Data(new _TType(std::move(Data_)))
			{
			}
			bool IsValid(void) const
			{
				return (bool)_Data;
			}
			void SetNull(void)
			{
				_Data.reset();
			}

			operator _TType()
			{
				return *_Data;
			}
			bool operator == (const SNullable& Value_) const
			{
				if (!_Data || !Value_._Data)
					return false;

				return (*_Data == *Value_._Data);
			}
			bool operator != (const SNullable& Value_) const
			{
				return !(*this == Value_);
			}
			bool operator < (const SNullable& Value_) const
			{
				if (!_Data || !Value_._Data)
					return false;

				return (*_Data < *Value_._Data);
			}
			bool operator <= (const SNullable& Value_) const
			{
				if (!_Data || !Value_._Data)
					return false;

				return (*_Data <= *Value_._Data);
			}
			bool operator > (const SNullable& Value_) const
			{
				if (!_Data || !Value_._Data)
					return false;

				return (*_Data > *Value_._Data);
			}
			bool operator >= (const SNullable& Value_) const
			{
				if (!_Data || !Value_._Data)
					return false;

				return (*_Data >= *Value_._Data);
			}

			bool operator == (const _TType& Value_) const
			{
				if (!_Data)
					return false;

				return (*_Data == Value_);
			}
			bool operator != (const _TType& Value_) const
			{
				return !(*this == Value_);
			}
			bool operator < (const _TType& Value_) const
			{
				if (!_Data)
					return false;

				return (*_Data < Value_);
			}
			bool operator <= (const _TType& Value_) const
			{
				if (!_Data)
					return false;

				return (*_Data <= Value_);
			}
			bool operator > (const _TType& Value_) const
			{
				if (!_Data)
					return false;

				return (*_Data > Value_);
			}
			bool operator >= (const _TType& Value_) const
			{
				if (!_Data)
					return false;

				return (*_Data >= Value_);
			}
		};

		int32 GetAge(int32 Year_, int32 Month_, int32 Day_);
		int32 GetAgeWithSclNum(const string& SclNum_);

		template<typename _Ty, size_t _Sz>
		array<_Ty, _Sz> operator - (const array<_Ty, _Sz>& Value_)
		{
			array<_Ty, _Sz> ret;

			for (size_t i = 0; i < _Sz; ++i)
				ret[i] = -Value_[i];

			return ret;
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<_Ty, _Sz> operator += (array<_Ty, _Sz>& lhs_, const _Ty2& Value_)
		{
			for (size_t i = 0; i < _Sz; ++i)
				lhs_[i] += Value_;

			return lhs_;
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<_Ty, _Sz> operator -= (array<_Ty, _Sz>& lhs_, const _Ty2& Value_)
		{
			for (size_t i = 0; i < _Sz; ++i)
				lhs_[i] -= Value_;

			return lhs_;
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<_Ty, _Sz> operator *= (array<_Ty, _Sz>& lhs_, const _Ty2& Value_)
		{
			for (size_t i = 0; i < _Sz; ++i)
				lhs_[i] *= Value_;

			return lhs_;
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<_Ty, _Sz> operator /= (array<_Ty, _Sz>& lhs_, const _Ty2& Value_)
		{
			for (size_t i = 0; i < _Sz; ++i)
				lhs_[i] /= Value_;

			return lhs_;
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<_Ty, _Sz> operator %= (array<_Ty, _Sz>& lhs_, const _Ty2& Value_)
		{
			for (size_t i = 0; i < _Sz; ++i)
				lhs_[i] %= Value_;

			return lhs_;
		}

		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<decltype(_Ty{} +_Ty2{}), _Sz> operator + (const array<_Ty, _Sz>& lhs_, const _Ty2& Value_)
		{
			array<decltype(_Ty{} +_Ty2{}), _Sz> Ret;
			for (size_t i = 0; i < _Sz; ++i)
				Ret[i] = lhs_[i];

			return (Ret += Value_);
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<decltype(_Ty{} +_Ty2{}), _Sz> operator - (const array<_Ty, _Sz>& lhs_, const _Ty2& Value_)
		{
			array<decltype(_Ty{} +_Ty2{}), _Sz> Ret;
			for (size_t i = 0; i < _Sz; ++i)
				Ret[i] = lhs_[i];

			return (Ret -= Value_);
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<decltype(_Ty{} +_Ty2{}), _Sz> operator * (const array<_Ty, _Sz>& lhs_, const _Ty2& Value_)
		{
			array<decltype(_Ty{} +_Ty2{}), _Sz> Ret;
			for (size_t i = 0; i < _Sz; ++i)
				Ret[i] = lhs_[i];

			return (Ret *= Value_);
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<decltype(_Ty{} +_Ty2{}), _Sz> operator / (const array<_Ty, _Sz>& lhs_, const _Ty2& Value_)
		{
			array<decltype(_Ty{} +_Ty2{}), _Sz> Ret;
			for (size_t i = 0; i < _Sz; ++i)
				Ret[i] = lhs_[i];

			return (Ret /= Value_);
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<decltype(_Ty{} +_Ty2{}), _Sz> operator % (const array<_Ty, _Sz>& lhs_, const _Ty2& Value_)
		{
			array<decltype(_Ty{} +_Ty2{}), _Sz> Ret;
			for (size_t i = 0; i < _Sz; ++i)
				Ret[i] = lhs_[i];

			return (Ret %= Value_);
		}

		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<_Ty, _Sz> operator += (array<_Ty, _Sz>& lhs_, const array<_Ty2, _Sz>& rhs_)
		{
			for (size_t i = 0; i < _Sz; ++i)
				lhs_[i] += rhs_[i];

			return lhs_;
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<_Ty, _Sz> operator -= (array<_Ty, _Sz>& lhs_, const array<_Ty2, _Sz>& rhs_)
		{
			for (size_t i = 0; i < _Sz; ++i)
				lhs_[i] -= rhs_[i];

			return lhs_;
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<_Ty, _Sz> operator *= (array<_Ty, _Sz>& lhs_, const array<_Ty2, _Sz>& rhs_)
		{
			for (size_t i = 0; i < _Sz; ++i)
				lhs_[i] *= rhs_[i];

			return lhs_;
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<_Ty, _Sz> operator /= (array<_Ty, _Sz>& lhs_, const array<_Ty2, _Sz>& rhs_)
		{
			for (size_t i = 0; i < _Sz; ++i)
				lhs_[i] /= rhs_[i];

			return lhs_;
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<_Ty, _Sz> operator %= (array<_Ty, _Sz>& lhs_, const array<_Ty2, _Sz>& rhs_)
		{
			for (size_t i = 0; i < _Sz; ++i)
				lhs_[i] %= rhs_[i];

			return lhs_;
		}

		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<decltype(_Ty{} +_Ty2{}), _Sz> operator + (const array<_Ty, _Sz>& lhs_, const array<_Ty2, _Sz>& rhs_)
		{
			array<decltype(_Ty{} +_Ty2{}), _Sz> Ret;
			for (size_t i = 0; i < _Sz; ++i)
				Ret[i] = lhs_[i];

			return (Ret += rhs_);
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<decltype(_Ty{} +_Ty2{}), _Sz> operator - (const array<_Ty, _Sz>& lhs_, const array<_Ty2, _Sz>& rhs_)
		{
			array<decltype(_Ty{} +_Ty2{}), _Sz> Ret;
			for (size_t i = 0; i < _Sz; ++i)
				Ret[i] = lhs_[i];

			return (Ret -= rhs_);
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<decltype(_Ty{} +_Ty2{}), _Sz> operator * (const array<_Ty, _Sz>& lhs_, const array<_Ty2, _Sz>& rhs_)
		{
			array<decltype(_Ty{} +_Ty2{}), _Sz> Ret;
			for (size_t i = 0; i < _Sz; ++i)
				Ret[i] = lhs_[i];

			return (Ret *= rhs_);
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<decltype(_Ty{} +_Ty2{}), _Sz> operator / (const array<_Ty, _Sz>& lhs_, const array<_Ty2, _Sz>& rhs_)
		{
			array<decltype(_Ty{} +_Ty2{}), _Sz> Ret;
			for (size_t i = 0; i < _Sz; ++i)
				Ret[i] = lhs_[i];

			return (Ret /= rhs_);
		}
		template<typename _Ty, typename _Ty2, size_t _Sz>
		array<decltype(_Ty{} +_Ty2{}), _Sz> operator % (const array<_Ty, _Sz>& lhs_, const array<_Ty2, _Sz>& rhs_)
		{
			array<decltype(_Ty{} +_Ty2{}), _Sz> Ret;
			for (size_t i = 0; i < _Sz; ++i)
				Ret[i] = lhs_[i];

			return (Ret %= rhs_);
		}

		template<typename _Ty, size_t _Sz>
		bool array_greater(const array<_Ty, _Sz>& have_, const array<_Ty, _Sz>& cost_)
		{
			for (size_t i = 0; i < _Sz; ++i)
			{
				if (have_[i] < cost_[i])
					return true;
			}

			return false;
		}
    }
}