#pragma once


#include "Core.h"


namespace rso
{
	namespace core
	{
		template<typename _TType, wchar_t And_, wchar_t Or_, wchar_t Open_, wchar_t Close_>
		class CLogic
		{
		public:
			using TValue = _TType;
			using TChecker = function<bool(const _TType&)>;

		private:
			class _CValue;
			class _CValueAnd;
			class _CValueOr;
			using _TValue = unique_ptr<_CValue>;

			class _CValue
			{
			public:
				virtual bool check(TChecker Checker_) const = 0;
			};
			class _CValueOnly : public _CValue
			{
				_TType _Value{};

			public:
				_CValueOnly(const _TType& Value_) :
					_Value(Value_)
				{
				}
				bool check(TChecker Checker_) const override
				{
					return Checker_(_Value);
				}
			};
			class _CValueAnd : public _CValue
			{
				enum class _EState
				{
					Value,
					And,
					Or,
					Opened,
					Closed,
					Max,
					Null
				};

				list<_TValue> _Values;

			public:
				_CValueAnd(_TValue&& Value_, const wstring& Logic_, size_t& Index_)
				{
					_Values.emplace_back(std::move(Value_));

					_EState State = _EState::Null;
					wstring Value;

					for (; Index_ < Logic_.size(); )
					{
						auto Char = Logic_[Index_];

						switch (State)
						{
						case _EState::Null:
							switch (Char)
							{
							case Open_:
								++Index_;
								_Values.emplace_back(_TValue(new _CValueOr(Logic_, Index_)));
								State = _EState::Opened;
								continue;

							case Close_:
							case And_:
							case Or_:
								THROWEX();

							default:
								State = _EState::Value;
								Value += Char;
								break;
							}
							break;

						case _EState::Value:
							switch (Char)
							{
							case Open_:
								THROWEX();

							case Close_:
								_Values.emplace_back(_TValue(new _CValueOnly(ValueFromString<_TType>(Value))));
								return;

							case And_:
								_Values.emplace_back(_TValue(new _CValueOnly(ValueFromString<_TType>(Value))));
								Value.clear();
								State = _EState::And;
								break;

							case Or_:
								_Values.emplace_back(_TValue(new _CValueOnly(ValueFromString<_TType>(Value))));
								return;

							default:
								Value += Char;
								break;
							}
							break;

						case _EState::And:
							switch (Char)
							{
							case Open_:
								++Index_;
								_Values.emplace_back(_TValue(new _CValueOr(Logic_, Index_)));
								State = _EState::Opened;
								continue;

							case Close_:
							case And_:
							case Or_:
								THROWEX();

							default:
								State = _EState::Value;
								Value += Char;
								break;
							}
							break;

						case _EState::Opened:
							switch (Char)
							{
							case Close_:
								State = _EState::Closed;
								break;

							default:
								THROWEX();
							}
							break;

						case _EState::Closed:
							switch (Char)
							{
							case Open_:
								THROWEX();

							case Close_:
								return;

							case And_:
								State = _EState::And;
								break;

							case Or_:
								return;

							default:
								THROWEX();
							}
							break;
						}

						++Index_;
					}

					if (!Value.empty())
						_Values.emplace_back(_TValue(new _CValueOnly(ValueFromString<_TType>(Value))));
				}
				bool check(TChecker Checker_) const override
				{
					for (auto& v : _Values)
					{
						if (!v->check(Checker_))
							return false;
					}

					return true;
				}
			};
			class _CValueOr : public _CValue
			{
				enum class _EState
				{
					Value,
					And,
					Or,
					Opened,
					Closed,
					AndOpened,
					Max,
					Null
				};

				list<_TValue> _Values;

			public:
				_CValueOr(const wstring& Logic_, size_t& Index_)
				{
					_EState State = _EState::Null;
					wstring Value;
					_TValue ValuePtr;

					for (; Index_ < Logic_.size(); )
					{
						auto Char = Logic_[Index_];

						switch (State)
						{
						case _EState::Null:
							switch (Char)
							{
							case Open_:
								++Index_;
								ValuePtr.reset(new _CValueOr(Logic_, Index_));
								State = _EState::Opened;
								continue;

							case Close_:
							case And_:
							case Or_:
								THROWEX();

							default:
								State = _EState::Value;
								Value += Char;
								break;
							}
							break;

						case _EState::Value:
							switch (Char)
							{
							case Open_:
								THROWEX();

							case Close_:
								_Values.emplace_back(_TValue(new _CValueOnly(ValueFromString<_TType>(Value))));
								Value.clear();
								return;

							case And_:
								++Index_;
								_Values.emplace_back(_TValue(new _CValueAnd(_TValue(new _CValueOnly(ValueFromString<_TType>(Value))), Logic_, Index_)));
								Value.clear();
								State = _EState::AndOpened;
								continue;

							case Or_:
								_Values.emplace_back(_TValue(new _CValueOnly(ValueFromString<_TType>(Value))));
								Value.clear();
								State = _EState::Or;
								break;

							default:
								Value += Char;
								break;
							}
							break;

						case _EState::Or:
							switch (Char)
							{
							case Open_:
								++Index_;
								ValuePtr.reset(new _CValueOr(Logic_, Index_));
								State = _EState::Opened;
								continue;

							case Close_:
							case And_:
							case Or_:
								THROWEX();

							default:
								State = _EState::Value;
								Value += Char;
								break;
							}
							break;

						case _EState::Opened:
							switch (Char)
							{
							case Close_:
								State = _EState::Closed;
								break;

							default:
								THROWEX();
							}
							break;

						case _EState::Closed:
							switch (Char)
							{
							case Open_:
								THROWEX();

							case Close_:
								return;

							case And_:
								++Index_;
								_Values.emplace_back(_TValue(new _CValueAnd(std::move(ValuePtr), Logic_, Index_)));
								State = _EState::AndOpened;
								continue;

							case Or_:
								_Values.emplace_back(std::move(ValuePtr));
								State = _EState::Or;
								break;

							default:
								THROWEX();
							}
							break;

						case _EState::AndOpened:
							switch (Char)
							{
							case Open_:
								THROWEX();

							case Close_:
								return;

							case And_:
								THROWEX();

							case Or_:
								State = _EState::Or;
								break;

							default:
								THROWEX();
							}
							break;
						}

						++Index_;
					}

					if (!Value.empty())
						_Values.emplace_back(_TValue(new _CValueOnly(ValueFromString<_TType>(Value))));
					else if (ValuePtr)
						_Values.emplace_back(std::move(ValuePtr));

					switch (State)
					{
					case _EState::Null:
					case _EState::Value:
					case _EState::Closed:
					case _EState::AndOpened:
						break;

					default:
						THROWEX();
					}
				}
				bool check(TChecker Checker_) const override
				{
					if (_Values.empty())
						return true;

					for (auto& v : _Values)
					{
						if (v->check(Checker_))
							return true;
					}

					return false;
				}
			};

			_TValue _Value;

		public:
			CLogic()
			{
			}
			CLogic(const wstring& Logic_)
			{
				set_logic(Logic_);
			}
			void set_logic(const wstring& Logic_)
			{
				size_t Index = 0;

				_Value.reset(new _CValueOr(Logic_, Index));

				if (Index != Logic_.size() && Logic_[Index] == Close_)
					THROWEX();
			}
			bool check(TChecker Checker_) const
			{
				return _Value->check(Checker_);
			}
		};
	}
}