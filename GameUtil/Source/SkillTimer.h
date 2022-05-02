#pragma once

#include "Base.h"
#include <functional>
#include <map>
#include <chrono>
#include <Rso/Base/List.h>
//#include <Rso/Physics/ObjectStraight.h>

namespace rso::gameskill
{
	using namespace std;
	using namespace core;
	using namespace base;
	//using namespace physics;

	template<typename _TSkill, typename _TTarget>
	class CSkillTimer
	{
	public:
		const size_t c_Index_Null = MaxValue<size_t>();

		struct SEffect
		{
			_TTarget Target{};
			const _TSkill Skill{};

			SEffect(const _TTarget& Target_, const _TSkill& Skill_) : Target(Target_), Skill(Skill_) {}
		};
		struct SSkill : public SEffect
		{
			TTime EndTime;
			milliseconds Period{};

			SSkill() {}
			SSkill(const SEffect& Effect_, const TTime& EndTime_, const milliseconds& Period_) : SEffect(Effect_), EndTime(EndTime_), Period(Period_) {}
		};

		//	EndTime		Period		Type
		//	0			0			1회성
		//	0			+			1회성
		//	+			0			지속성
		//	+			+			주기성
		using TCallback = function<void(const TTime& Time_, size_t PlayerIndex_, const SEffect& Effect_, bool End_)>;
		class CPlayer : public SObjectStraight
		{
			using _TSkills = multimap<TTime, SSkill>;

			TCallback _Callback;
			size_t _Index = c_Index_Null;
			_TSkills _Casts; // 자신이 시전하는 주문에 대해서는 시전중 취소 가능하므로 _Buffs에 포함시키지 않음.
			_TSkills _Buffs;

		public:
			CPlayer(TCallback Callback_, size_t Index_) : _Callback(Callback_), _Index(Index_) {}
			inline void Cast(const TTime& StartTime_, const SSkill& Skill_) { _Casts.emplace(StartTime_, Skill_); }
			inline void Buff(const TTime& StartTime_, const SSkill& Skill_) { _Buffs.emplace(StartTime_, Skill_); }
			inline void ClearCast(void) { _Casts.clear(); }
		private:
			void _Proc(const TTime& Time_, _TSkills& Skills_)
			{
				for (auto it = Skills_.begin(); it != Skills_.end() && it->first <= Time_; it = Skills_.begin())
				{
					auto End = it->first == it->second.EndTime;
					if (it->second.EndTime != TTime() && !End) // 1회성이 아니면
					{
						// 익셉션 날 수 있으므로 emplace 가장 먼저
						if (it->second.Period.count() > 0 && it->first + it->second.Period < it->second.EndTime) // 주기성이고, 다음 Time 이 EndTime 보다 작으면
							Skills_.emplace(it->first + it->second.Period, it->second);
						else
							Skills_.emplace(it->second.EndTime, it->second);
					}

					try
					{
						_Callback(it->first, _Index, it->second, End);
					}
					catch (...)
					{
					}

					Skills_.erase(it);
				}
			}
		public:
			void Proc(const TTime& Time_)
			{
				_Proc(Time_, _Casts);
				_Proc(Time_, _Buffs);
			}
		};

	private:
		TCallback _Callback;
		CList<CPlayer> _Players;
		inline double _TimePointToDouble(const TTime& Time_) { return Time_.time_since_epoch().count() * 0.0000001; }

	public:
		CSkillTimer(TCallback Callback_) : _Callback(Callback_) {}
		inline void AddPlayerAt(size_t PlayerIndex_) { _Players.emplace_at(PlayerIndex_, CPlayer(_Callback, PlayerIndex_)); }
		inline size_t AddPlayer(void) { return _Players.emplace(CPlayer(_Callback, _Players.new_index())).Index(); }
		inline void DelPlayer(size_t PlayerIndex_) { _Players.erase(PlayerIndex_); }
		inline void Cast(const TTime& StartTime_, size_t PlayerIndex_, const SSkill& Skill_) { _Players[PlayerIndex_].Cast(StartTime_, Skill_); } // 내가 딜레이 있는 주문
		inline void Buff(const TTime& StartTime_, size_t PlayerIndex_, const SSkill& Skill_) { _Players[PlayerIndex_].Buff(StartTime_, Skill_); } // 나에게 딜레이 있는 주문
		inline void ClearCast(size_t PlayerIndex_) { _Players[PlayerIndex_].ClearCast(); }
		//inline SPoint GetCurPos(size_t PlayerIndex_, const TTime& Time_) const { return _Players[PlayerIndex_].GetCurPos(_TimePointToDouble(Time_); }
		//inline SPosTheta GetCurPosTheta(size_t PlayerIndex_, const TTime& Time_) const { return _Players[PlayerIndex_].GetCurPosTheta(_TimePointToDouble(Time_)); }
		//inline void SetPosTheta(size_t PlayerIndex_, const TTime& Time_, const SPosTheta& PosTheta_) const { return _Players[PlayerIndex_].SetPosTheta(_TimePointToDouble(Time_), PosTheta_); }
		//inline void SetSpeed(size_t PlayerIndex_, const TTime& Time_, double Speed_) const { return _Players[PlayerIndex_].SetSpeed(_TimePointToDouble(Time_), Speed_); }

		void Proc(void) noexcept
		{
			try
			{
				auto Now = system_clock::now();
				for (auto& i : _Players)
					i.Proc(Now);
			}
			catch (...)
			{
			}
		}

		//Cast
		//	발사체모양
		//		없음, 점, 선, 사각, 원, 파이, 도넛
		//	발사 형태 : 시작점
		//		길이를 가진 라인
		//		사거리 내의 대상 지정
		//			타겟(플레이어 번호) : 여기서 피아 식별 되도록 (모두 적이면 팀번호는?)
		//			지형
		//Buff
		//Hit (Effect)
		//	여기서 물리가 적용안되면 바로 콜백그대로 와서 비 효율적인데 해결법은?

		//Proc
		//	발사체 충돌 처리
		//		아군에게만 충돌알지, 등등




		// 1. 비유도발사체, 유도발사체, 범위스킬, 지속범위스킬, 타겟 지정 범위스킬
		// SetPosTheta 호출시 검증할것.
		// 3. 스킬 중복 추가 되지 않도록 할것? (효과 중첩적용, 앞스킬의 만료가 뒤에추가된 스킬효과까지 삭제해버릴 수 있음.)
		//     해결방법? multi index?
	};
}
