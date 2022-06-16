#pragma once

#include "MovingObject2D.h"
#include "CollectionCollider2D.h"

namespace rso::physics
{
    class CShuttleObject2D : public CMovingObject2D
    {
        SPoint _BeginPos;
        SPoint _EndPos;
        float _Velocity = 0.0f;
        float _Delay = 0.0f;
        SStructMove _StructMove;

    public:
        CShuttleObject2D(const STransform& Transform_, const SPoint& BeginPos_, const SPoint& EndPos_, float Velocity_, float Delay_, const SStructMove& StructMove_);
        SStructMovePosition GetStructMovePosition(void) const { return SStructMovePosition(_StructMove, LocalPosition); }
        void _FixedUpdate(int64 Tick_);
    };
}
