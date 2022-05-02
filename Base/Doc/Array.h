#pragma once


#include <array>


namespace rso
{
	namespace base
	{
		using namespace std;

		template<typename _Ty, size_t _Size>
		class CArray : public array<_Ty, _Size>
		{
		public:
			using TBaseType = array<_Ty, _Size>;

			CArray()
			{
				for (auto& i : *this)
					i = _Ty{};
			}
			CArray(const TBaseType& Obj_) :
				TBaseType(Obj_)
			{
			}
			bool operator < (const TBaseType& Obj_) const
			{
				for (size_t i = 0; i < _Size; ++i)
				{
					if (operator[](i) >= Obj_[i])
						return false;
				}

				return true;
			}
			bool operator <= (const TBaseType& Obj_) const
			{
				for (size_t i = 0; i < _Size; ++i)
				{
					if (operator[](i) > Obj_[i])
						return false;
				}

				return true;
			}
			bool operator > (const TBaseType& Obj_) const
			{
				for (size_t i = 0; i < _Size; ++i)
				{
					if (operator[](i) <= Obj_[i])
						return false;
				}

				return true;
			}
			bool operator >= (const TBaseType& Obj_) const
			{
				for (size_t i = 0; i < _Size; ++i)
				{
					if (operator[](i) < Obj_[i])
						return false;
				}

				return true;
			}
			CArray operator += (const _Ty& Value_)
			{
				for (size_t i = 0; i < _Size; ++i)
					operator[](i) += Value_;

				return *this;
			}
			CArray operator + (const _Ty& Value_) const
			{
				auto Ret(*this);
				return (Ret += Value_);
			}
			CArray operator -= (const _Ty& Value_)
			{
				for (size_t i = 0; i < _Size; ++i)
					operator[](i) -= Value_;

				return *this;
			}
			CArray operator - (const _Ty& Value_) const
			{
				auto Ret(*this);
				return (Ret -= Value_);
			}
			CArray operator *= (const _Ty& Value_)
			{
				for (size_t i = 0; i < _Size; ++i)
					operator[](i) *= Value_;

				return *this;
			}
			CArray operator * (const _Ty& Value_) const
			{
				auto Ret(*this);
				return (Ret *= Value_);
			}
			CArray operator /= (const _Ty& Value_)
			{
				for (size_t i = 0; i < _Size; ++i)
					operator[](i) /= Value_;

				return *this;
			}
			CArray operator / (const _Ty& Value_) const
			{
				auto Ret(*this);
				return (Ret /= Value_);
			}

			CArray operator += (const TBaseType& Value_)
			{
				for (size_t i = 0; i < _Size; ++i)
					operator[](i) += Value_[i];

				return *this;
			}
			CArray operator + (const TBaseType& Value_) const
			{
				auto Ret(*this);
				return (Ret += Value_);
			}
			CArray operator -= (const TBaseType& Value_)
			{
				for (size_t i = 0; i < _Size; ++i)
					operator[](i) -= Value_[i];

				return *this;
			}
			CArray operator - (const TBaseType& Value_) const
			{
				auto Ret(*this);
				return (Ret -= Value_);
			}
			CArray operator *= (const TBaseType& Value_)
			{
				for (size_t i = 0; i < _Size; ++i)
					operator[](i) *= Value_[i];

				return *this;
			}
			CArray operator * (const TBaseType& Value_) const
			{
				auto Ret(*this);
				return (Ret *= Value_);
			}
			CArray operator /= (const TBaseType& Value_)
			{
				for (size_t i = 0; i < _Size; ++i)
					operator[](i) /= Value_[i];

				return *this;
			}
			CArray operator / (const TBaseType& Value_) const
			{
				auto Ret(*this);
				return (Ret /= Value_);
			}
		};
	}
}