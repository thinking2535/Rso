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
		//	0			0			1ȸ��
		//	0			+			1ȸ��
		//	+			0			���Ӽ�
		//	+			+			�ֱ⼺
		using TCallback = function<void(const TTime& Time_, size_t PlayerIndex_, const SEffect& Effect_, bool End_)>;
		class CPlayer : public SObjectStraight
		{
			using _TSkills = multimap<TTime, SSkill>;

			TCallback _Callback;
			size_t _Index = c_Index_Null;
			_TSkills _Casts; // �ڽ��� �����ϴ� �ֹ��� ���ؼ��� ������ ��� �����ϹǷ� _Buffs�� ���Խ�Ű�� ����.
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
					if (it->second.EndTime != TTime() && !End) // 1ȸ���� �ƴϸ�
					{
						// �ͼ��� �� �� �����Ƿ� emplace ���� ����
						if (it->second.Period.count() > 0 && it->first + it->second.Period < it->second.EndTime) // �ֱ⼺�̰�, ���� Time �� EndTime ���� ������
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
		inline void Cast(const TTime& StartTime_, size_t PlayerIndex_, const SSkill& Skill_) { _Players[PlayerIndex_].Cast(StartTime_, Skill_); } // ���� ������ �ִ� �ֹ�
		inline void Buff(const TTime& StartTime_, size_t PlayerIndex_, const SSkill& Skill_) { _Players[PlayerIndex_].Buff(StartTime_, Skill_); } // ������ ������ �ִ� �ֹ�
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
		//	�߻�ü���
		//		����, ��, ��, �簢, ��, ����, ����
		//	�߻� ���� : ������
		//		���̸� ���� ����
		//		��Ÿ� ���� ��� ����
		//			Ÿ��(�÷��̾� ��ȣ) : ���⼭ �Ǿ� �ĺ� �ǵ��� (��� ���̸� ����ȣ��?)
		//			����
		//Buff
		//Hit (Effect)
		//	���⼭ ������ ����ȵǸ� �ٷ� �ݹ�״�� �ͼ� �� ȿ�����ε� �ذ����?

		//Proc
		//	�߻�ü �浹 ó��
		//		�Ʊ����Ը� �浹����, ���




		// 1. �������߻�ü, �����߻�ü, ������ų, ���ӹ�����ų, Ÿ�� ���� ������ų
		// SetPosTheta ȣ��� �����Ұ�.
		// 3. ��ų �ߺ� �߰� ���� �ʵ��� �Ұ�? (ȿ�� ��ø����, �ս�ų�� ���ᰡ �ڿ��߰��� ��ųȿ������ �����ع��� �� ����.)
		//     �ذ���? multi index?
	};
}
