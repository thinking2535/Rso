#pragma once

#include "Base.h"
#include "Tick.h"
#include "PlayerObject2D.h"
#include <Rso/Base/List.h>

namespace rso::physics
{
	using namespace base;

	class CEngine
	{
	public:
		using TObjects = CList<shared_ptr<CCollider2D>>;
		using TObjectsIt = TObjects::iterator;
		using TMovingObjects = CList<shared_ptr<CMovingObject2D>>;
		using TMovingObjectsIt = TMovingObjects::iterator;

	protected:
		int64 _NetworkTickSync;
		CTick _CurTick;
		int64 _Tick = 0; // ���� ó���� ���� ������ Tick
	private:
		static float _ContactOffset;
		static int32 _FPS;
		static int64 _UnitTick;
		static float _DeltaTime;
	public:
		static inline float GetContactOffset(void) { return _ContactOffset; }
		static inline int32 GetFPS(void) { return _FPS; }
		static inline int64 GetUnitTick(void) { return _UnitTick; }
		static inline float GetDeltaTime(void) { return _DeltaTime; }
		inline int64 GetTick(void) const { return _Tick; }
	protected:
		CList<shared_ptr<CCollider2D>> _Objects;
		CList<shared_ptr<CMovingObject2D>> _MovingObjects;
		vector<shared_ptr<CPlayerObject2D>> _Players;
	public:
		CEngine(int64 NetworkTickSync_, int64 CurTick_, float ContactOffset_, int32 FPS_);
	protected:
		void _Update(int64 ToTick_);
	public:
		TObjectsIt AddObject(const shared_ptr<CCollider2D>& Object_);
		void RemoveObject(TObjectsIt Iterator_);
		TMovingObjectsIt AddMovingObject(const shared_ptr<CMovingObject2D>& Object_);
		void RemoveMovingObject(TMovingObjectsIt Iterator_);
		// RemovePlayer �� �߰����� ����
		// �ܺο��� ����� Collision �ݹ��� ȣ���ϴµ� �ű⿡�� RemovePlayer �� ��� OtherPlayer�� ���� CollisionCallback�� ȣ�� �� �� ����
		void AddPlayer(const shared_ptr<CPlayerObject2D>& Player_);
		void Start(void);
		bool IsStarted(void) const;
		function<void(int64 Tick_)> fFixedUpdate;
	};
}
