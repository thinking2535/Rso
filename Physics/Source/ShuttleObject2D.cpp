#include "ShuttleObject2D.h"
#include "Physics.h"
#include "Engine.h"

namespace rso::physics
{
    using namespace placeholders;

    CShuttleObject2D::CShuttleObject2D(const STransform& Transform_, const SPoint& BeginPos_, const SPoint& EndPos_, float ScalarVelocity_, float Delay_, const SStructMove& StructMove_) :
        CMovingObject2D(Transform_, SPoint()),
        _BeginPos(BeginPos_),
        _EndPos(EndPos_),
        _ScalarVelocity(ScalarVelocity_),
        _Delay(Delay_),
        _StructMove(StructMove_)
    {
        fFixedUpdate = std::bind(&CShuttleObject2D::_FixedUpdate, this, _1);

        if (_StructMove.IsMoving)
            _SetMovingVelocity();
    }
    void CShuttleObject2D::_SetMovingVelocity(void)
    {
        auto Vector = (_EndPos - _BeginPos) * static_cast<float>(_StructMove.Direction);
        Velocity = Vector * (_ScalarVelocity / GetScalar(Vector));
    }
    void CShuttleObject2D::_FixedUpdate(int64 /*Tick_*/)
    {
        // Position을 바꾸는 것은 Engine이고 여기서는 Velocity만 변경
        if (_StructMove.IsMoving)
        {
            if (_StructMove.Direction == 1)
            {
                if (MoveTowards(LocalPosition, _EndPos, _ScalarVelocity * CEngine::GetDeltaTime()))
                {
                    _StructMove.Direction = -1;
                    _StructMove.IsMoving = false;
                    Velocity = SPoint();
                }
            }
            else
            {
                if (MoveTowards(LocalPosition, _BeginPos, _ScalarVelocity * CEngine::GetDeltaTime()))
                {
                    _StructMove.Direction = 1;
                    _StructMove.IsMoving = false;
                    Velocity = SPoint();
                }
            }
        }
        else
        {
            _StructMove.StoppedDuration += CEngine::GetDeltaTime();

            if (_StructMove.StoppedDuration >= _Delay)
            {
                _StructMove.StoppedDuration = 0.0f;
                _StructMove.IsMoving = true;
                _SetMovingVelocity();
            }
        }
    }
}
