#pragma once

#include "MovingObject2D.h"

namespace rso::physics
{
    class CShuttleObject2D : public CMovingObject2D
    {
        SPoint _BeginPos;
        SPoint _EndPos;
        float _ScalarVelocity = 0.0f;
        float _Delay = 0.0f;
        SStructMove _StructMove;

    public:
        CShuttleObject2D(const STransform& Transform_, const list<shared_ptr<CCollider2D>>& Colliders_, const SPoint& BeginPos_, const SPoint& EndPos_, float ScalarVelocity_, float Delay_, const SStructMove& StructMove_);
        SStructMovePosition GetStructMovePosition(void) const { return SStructMovePosition(_StructMove, LocalPosition); }
        void _SetMovingVelocity(void);
        void _FixedUpdate(int64 tick);
    };
}
