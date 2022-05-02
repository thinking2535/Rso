// ConsoleApplication3.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>

//#include <Rso/Base/Num.h>
//using namespace rso;
//using namespace base;

using TValue = __int32;
using TCode = __int32;

// 아래 클래스는 Max 변수를 두어 자동으로 최대치를 넘지 않도록
// 하지만 스탯 클래스에서는 불필요

//class CValue
//{
//	static_assert(TValue(-1) < 0, "TValue must be signed");
//
//	TValue	_Value{ 0 };
//	TValue	_Max{ 0 };
//
//public:
//	explicit CValue() {}
//	explicit CValue(TValue Value_) :
//		_Value(Value_), _Max(Value_)
//	{}
//	explicit CValue(TValue Value_, TValue Max_) :
//		_Value(Value_), _Max(Max_)
//	{}
//
//	TValue	Value(void) const { return _Value; }
//	TValue&	Max(void) { return _Max; }
//
//	//operator TValue() const
//	//{
//	//	return _Value;
//	//}
//	void	SetValid(void)
//	{
//		if (_Max < 0)
//			_Max = 0;
//
//		if (_Value > _Max)
//			_Value = _Max;
//	}
//	void	SetMax(const TValue Max_)
//	{
//		if (_Value > Max_)
//			_Value = Max_;
//
//		if (Max_ < 0)
//			_Max = 0;
//		else
//			_Max = Max_;
//	}
//	//void	AddMax(const TValue Value_)
//	//{
//	//	if (_Max + Value_ < 0)
//	//		_Max = 0;
//	//	else if (_Max + Value_ < _Max)
//	//		_Max = CNum<TValue>::Max;
//	//	else
//	//		_Max += Value_
//	//}
//
//	void	operator += (const TValue Value_)
//	{
//		if (_Value + Value_ > _Max)
//			_Value = _Max;
//		else if (_Value + Value_ < 0)
//			_Value = 0;
//		else
//			_Value += Value_;
//	}
//};

enum class EValueType : __int8
{
	Add,
	Percent,
	Set,
	Max,
	Null
};

struct SAddedValue
{
	EValueType	Type{ EValueType::Null };
	TValue	Value{ 0 };

	explicit SAddedValue() {}
	explicit SAddedValue(EValueType Type_, TValue Value_) :
		Type(Type_), Value(Value_) {}


	friend void	operator += (TValue& Value_, const SAddedValue& AddedValue_)
	{
		if (AddedValue_.Type == EValueType::Add)
			Value_ += AddedValue_.Value;
		else if (AddedValue_.Type == EValueType::Percent)
			Value_ += (Value_*AddedValue_.Value / 100);
		else
			Value_ = AddedValue_.Value;
	}

	//friend void operator += (CValue& Value_, const SAddedValue& AddedValue_)
	//{
	//	if (AddedValue_.Type == EValueType::Add)
	//		Value_ += (AddedValue_.Value);
	//	else if (AddedValue_.Type == EValueType::Percent)
	//		Value_ += (Value_.Max()*AddedValue_.Value / 100);
	//	else
	//		Value_ = AddedValue_.Value;
	//}
};


#include <chrono>
#include <new>
#include <list>
#include <map>

using namespace std;
using namespace std::chrono;

using TTime = system_clock::time_point;
using TSpan = milliseconds;

enum EFactor
{
	HP,
	MP,
	Attack,
	Armor,
	Magic,
	Resistance,
	AttackSpeed,
	MagicSpeed,
	MoveSpeed,
	Critical,
	MagicCritical,
	AntiCritical,
	AntiMagicCritical,
	KnockBack,
	Aim,
	Range,
	Max,
	Null
};

enum class EDuration : __int8
{
	OneTime,
	OverTime,
	Passive,
	Max,
	Null
};

struct SEffect
{
	EFactor			Factor{ EFactor::Null };
	SAddedValue		AddedValue;

	SEffect(EFactor Factor_, const SAddedValue& AddedValue_) :
		Factor(Factor_), AddedValue(AddedValue_)
	{
	}
};

struct SEffectInfo
{
	TCode		Code{ 0 };
	wstring		Name;
	EDuration	Duration{ EDuration::Null };
	__int32		Count{ 0 };
	TSpan		Span;
	list<SEffect>	Effects;

	SEffectInfo(TCode Code_, const wstring& Name_, EDuration Duration_, __int32 Count_, TSpan Span_) :
		Code(Code_), Name(Name_), Duration(Duration_), Count(Count_), Span(Span_)
	{
	}

	void	operator += (const SEffect& Effect_)
	{
		try
		{
			Effects.emplace_back(Effect_);
		}
		catch (...)
		{
		}
	}
};



struct SEffectOld
{
	//	type		count	endtime
	//	onetime						1회

	//	overtime			1		도트 시간
	//	overtime	1				도트 회수
	//	overtime	1		1		도트 시간 회수

	//	passive		0		1		패시브 시간
	//	passive		1				패시브 회수

	EDuration	Type{ EDuration::OneTime };
	__int32		Count{ 0 };
	TSpan		Span;
	TCode		Code{ 0 };
	SAddedValue	HP;
	SAddedValue	MP;
	SAddedValue	Damage;
	SAddedValue	MoveSpeed;
	SAddedValue	AttackSpeed;
	SAddedValue	AttackRange;
	SAddedValue	SightRange;

	SEffectOld(EDuration Type_,
		__int32 Count_,
		TSpan Span_,
		TCode Code_,
		const SAddedValue& HP_,
		const SAddedValue& MP_,
		const SAddedValue& Damage_,
		const SAddedValue& MoveSpeed_,
		const SAddedValue& AttackSpeed_,
		const SAddedValue& AttackRange_,
		const SAddedValue& SightRange_
		) :
		Type(Type_),
		Count(Count_),
		Span(Span_),
		Code(Code_),
		HP(HP_),
		MP(MP_),
		Damage(Damage_),
		MoveSpeed(MoveSpeed_),
		AttackSpeed(AttackSpeed_),
		AttackRange(AttackRange_),
		SightRange(SightRange_) {}
};

class CState
{
	// 동일한 코드의 버프가 들어오면 기존 버프를 대체하기 위함
	struct _SEffect;
	using _TEffects = list<_SEffect>;
	using _TMap = map<TCode, _TEffects::const_iterator>;
	using _TMapIt = _TMap::const_iterator;

	struct _SEffect
	{
		__int32 Count{ 0 };
		TTime	EndTime;
		const SEffectInfo*	Effect{ 0 };
		_TMapIt	MapIt;

		_SEffect() {}
		_SEffect(TTime Now_, const SEffectInfo* Effect_, _TMapIt MapIt_) :
			Count(Effect_->Count), EndTime(Effect_->Span == TSpan() ? TTime() : Now_ + Effect_->Span), Effect(Effect_), MapIt(MapIt_){}
	};

	TValue	_HP{ 0 };
	TValue	_MP{ 0 };

	struct _SValue
	{
		TValue	Default{ 0 };
		TValue	Value{ 0 };

		explicit _SValue() {}
		explicit _SValue(TValue Default_, TValue Value_) :
			Default(Default_), Value(Value_) {}
		explicit _SValue(TValue Value_) :
			Default(Value_), Value(Value_) {}
	};

	_SValue		_Factors[EFactor::Max];
	_TMap		_OverTimeMap;
	_TMap		_PassiveMap;
	_TEffects	_OverTimeEffects;
	_TEffects	_PassiveEffects;

	void			_Add(const SEffectInfo* Effect_)
	{
		for (auto& Factor : Effect_->Effects)
		{
			if (Factor.Factor == EFactor::HP)
			{
				_HP += Factor.AddedValue;
				if (_HP > _Factors[Factor.Factor].Value)
					_HP = _Factors[Factor.Factor].Value;
			}
			else if (Factor.Factor == EFactor::MP)
			{
				_MP += Factor.AddedValue;
				if (_MP > _Factors[Factor.Factor].Value)
					_MP = _Factors[Factor.Factor].Value;
			}
			else
			{
				_Factors[Factor.Factor].Value += Factor.AddedValue;
			}
		}
	}
	void			_RefreshPassive(void)
	{
		for (int i = 0; i < EFactor::Max; ++i)
			_Factors[i].Value = _Factors[i].Default;

		TValue Multiples[EFactor::Max] = {};		// Percent 타입의 값은 따로 저장 ( Add 타입 먼저 계산하기 위함 )
		pair<bool, TValue> SetValues[EFactor::Max] = {};	// Set 타입은 마지막에 설정하기 위함

		// 순회하며 Add 타입 먼저 처리, Percent , Set 등은 따로 처리
		for (auto it = _PassiveEffects.begin(); it != _PassiveEffects.end(); ++it)
		{
			for (auto& Effect : it->Effect->Effects)
			{
				if (Effect.AddedValue.Type == EValueType::Add)
					_Factors[Effect.Factor].Value += Effect.AddedValue.Value;
				else if (Effect.AddedValue.Type == EValueType::Percent)
					Multiples[Effect.Factor] += Effect.AddedValue.Value;
				else
				{
					SetValues[Effect.Factor].first = true;
					if (SetValues[Effect.Factor].second > Effect.AddedValue.Value)	// Set 타입의 값은 가장 작은 수가 우선되도록
						SetValues[Effect.Factor].second = Effect.AddedValue.Value;
				}
			}
		}

		// 저장해둔 Percent 타입 처리
		for (int i = 0; i < EFactor::Max; ++i)
			_Factors[i].Value += (_Factors[i].Default*Multiples[i] / 100);


		// 음수인 속성에 대해서는 0으로 재설정
		for (int i = 0; i < EFactor::Max; ++i)
		{
			if (_Factors[i].Value < 0)
				_Factors[i].Value = 0;
		}


		// Set 타입의 값을 마지막에 저장( 가장 작은 수)
		for (int i = 0; i < EFactor::Max; ++i)
		{
			if (SetValues[i].first)
				_Factors[i].Value = SetValues[i].second;
		}

		if (_HP > _Factors[EFactor::HP].Value)
			_HP = _Factors[EFactor::HP].Value;
		if (_MP > _Factors[EFactor::MP].Value)
			_MP = _Factors[EFactor::MP].Value;
	}

public:
	CState(TValue Values_[EFactor::Max])
	{
		for (int i = 0; i < EFactor::Max; ++i)
		{
			if (i == EFactor::HP)
				_HP = Values_[i];
			else if (i == EFactor::MP)
				_MP = Values_[i];

			new(&_Factors[i])_SValue(Values_[i]);
		}
	}

	TValue Get(EFactor Factor_) const
	{
		if (Factor_ == EFactor::HP)
			return _HP;
		else if (Factor_ == EFactor::MP)
			return _MP;

		return _Factors[Factor_].Value;
	}

	void operator += (const SEffectInfo* Effect_)
	{
		try
		{
			if (Effect_->Duration == EDuration::OneTime)	// One Time
			{
				_Add(Effect_);
				return;
			}

			if (Effect_->Duration == EDuration::Passive)		// Passive
			{
				auto ib = _PassiveMap.emplace(Effect_->Code, _TEffects::const_iterator());
				try
				{
					_PassiveEffects.emplace_back(_SEffect(system_clock::now(), Effect_, ib.first));
					if (!ib.second)
						_PassiveEffects.erase(ib.first->second);

					ib.first->second = --_PassiveEffects.end();
					_RefreshPassive();
				}
				catch (...)
				{
					if (ib.second)
						_PassiveMap.erase(ib.first);
				}
			}
			else	// Over Time
			{
				auto ib = _OverTimeMap.emplace(Effect_->Code, _TEffects::const_iterator());
				try
				{
					_OverTimeEffects.emplace_back(_SEffect(system_clock::now(), Effect_, ib.first));
					if (!ib.second)
						_OverTimeEffects.erase(ib.first->second);

					ib.first->second = --_OverTimeEffects.end();
				}
				catch (...)
				{
					if (!ib.second)
						_OverTimeMap.erase(ib.first);
				}
			}
		}
		catch (...)
		{
		}
	}

	void	Proc(void)
	{
		auto Now = system_clock::now();

		// OverTimeEffect Proc
		for (auto it = _OverTimeEffects.begin(); it != _OverTimeEffects.end();)
		{
			// 시간 기반인가? ( 시간이 이미 지난경우는 Effect처리하지 않고 삭제 하기 위해 시간을 먼저 비교 )
			if (it->EndTime != TTime())
			{
				if (it->EndTime < system_clock::now())
				{
					_OverTimeMap.erase(it->MapIt);
					_OverTimeEffects.erase(it++);
					continue;
				}
			}

			_Add(it->Effect);

			// 회수 기반인가?
			if (it->Count > 0)
			{
				if (--(it->Count) == 0)
				{
					_OverTimeMap.erase(it->MapIt);
					_OverTimeEffects.erase(it++);
					continue;
				}
			}

			++it;
		}

		// PassiveEffect Proc
		bool NeedToRefresh{ false };
		for (auto it = _PassiveEffects.begin(); it != _PassiveEffects.end();)
		{
			// 시간 기반인가? ( 시간이 이미 지난경우는 Effect처리하지 않고 삭제 하기 위해 시간을 먼저 비교 )
			if (it->EndTime != TTime())
			{
				if (it->EndTime < system_clock::now())
				{
					_PassiveMap.erase(it->MapIt);
					_PassiveEffects.erase(it++);
					NeedToRefresh = true;
					continue;
				}
			}

			// 회수 기반인가?
			if (it->Count > 0)
			{
				if (--(it->Count) == 0)
				{
					_PassiveMap.erase(it->MapIt);
					_PassiveEffects.erase(it++);
					NeedToRefresh = true;
					continue;
				}
			}

			++it;
		}

		if (NeedToRefresh)
			_RefreshPassive();
	}
};

#include <iostream>
void Print(const CState& State_)
{
	for (int i = 0; i < EFactor::Max; ++i)
		cout << State_.Get(EFactor(i)) << endl;
}


#include <thread>

int _tmain(int argc, _TCHAR* argv[])
{
	//HP,
	//MP,
	//Attack,
	//Armor,
	//Magic,
	//Resistance,
	//AttackSpeed,
	//MagicSpeed,
	//MoveSpeed,
	//Critical,
	//MagicCritical,
	//AntiCritical,
	//AntiMagicCritical,
	//KnockBack,
	//Aim,
	//Range,

	TValue Values[EFactor::Max] = {
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
	};

	CState State(Values);
	SEffectInfo Heal(0, L"Heal", EDuration::OneTime, 0, TSpan());
	Heal += SEffect(EFactor::HP, SAddedValue(EValueType::Add, 10));

	SEffectInfo ManaPotion(0, L"ManaPotion", EDuration::OneTime, 0, TSpan());
	ManaPotion += SEffect(EFactor::MP, SAddedValue(EValueType::Add, -10));

	SEffectInfo EnduranceAura(0, L"EnduranceAura", EDuration::Passive, 5, TSpan(10));
	EnduranceAura += SEffect(EFactor::Armor, SAddedValue(EValueType::Add, -10));

	SEffectInfo Stun(0, L"Stun", EDuration::Passive, 2, TSpan());
	Stun += SEffect(EFactor::MoveSpeed, SAddedValue(EValueType::Set, 0));
	Stun += SEffect(EFactor::AttackSpeed, SAddedValue(EValueType::Set, 0));
	Stun += SEffect(EFactor::MagicSpeed, SAddedValue(EValueType::Set, 0));

	//State += &Heal;
	//State += &ManaPotion;
	//State += &EnduranceAura;
	State += &Stun;

	cout << "\nState" << endl;
	Print(State);
	State.Proc();

	cout << "\nState" << endl;
	Print(State);
	State.Proc();

	cout << "\nState" << endl;
	Print(State);
	State.Proc();

	cout << "\nState" << endl;
	Print(State);
	State.Proc();


	return 0;
}

