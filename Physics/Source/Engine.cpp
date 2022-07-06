#include "Engine.h"

namespace rso::physics
{
    float CEngine::_ContactOffset;
    int32 CEngine::_FPS;
    int64 CEngine::_UnitTick;
    float CEngine::_DeltaTime;

    CEngine::CEngine(int64 NetworkTickSync_, int64 CurTick_, float ContactOffset_, int32 FPS_) :
        _NetworkTickSync(NetworkTickSync_),
        _CurTick(CurTick_),
        _Tick(CurTick_)
    {
        _ContactOffset = ContactOffset_;
        _FPS = FPS_;
        _UnitTick = 10000000 / _FPS;
        _DeltaTime = 1.0f / _FPS;
    }
    void CEngine::_Update(int64 ToTick_)
    {
        for (; _Tick < ToTick_; _Tick += _UnitTick)
        {
            for (auto& i : _MovingObjects)
            {
                if (i->fFixedUpdate)
                    i->fFixedUpdate(_Tick);
            }

            for (auto& p : _Players)
            {
                if (p->fFixedUpdate)
                    p->fFixedUpdate(_Tick);
            }

            for (size_t pi = 0; pi < _Players.size() - 1; ++pi)
            {
                for (size_t ti = pi + 1; ti < _Players.size(); ++ti)
                    _Players[pi]->OverlappedCheck(_Tick, _Players[pi], _Players[ti]);
            }

            for (auto& p : _Players)
            {
                for (auto& m : _MovingObjects)
                    p->OverlappedCheck(_Tick, p, m);

                for (auto& s : _Objects)
                    p->OverlappedCheck(_Tick, p, s);
            }

            if (fFixedUpdate)
                fFixedUpdate(_Tick);
        }
    }
    CEngine::TObjectsIt CEngine::AddObject(const shared_ptr<CCollider2D>& Object_)
    {
        return _Objects.emplace(Object_);
    }
    void CEngine::RemoveObject(TObjectsIt Iterator_)
    {
        (*Iterator_)->LocalEnabled = false;
        _Objects.erase(Iterator_);
    }
    CEngine::TMovingObjectsIt CEngine::AddMovingObject(const shared_ptr<CMovingObject2D>& Object_)
    {
        return _MovingObjects.emplace(Object_);
    }
    void CEngine::RemoveMovingObject(TMovingObjectsIt Iterator_)
    {
        (*Iterator_)->pCollider->LocalEnabled = false;
        _MovingObjects.erase(Iterator_);
    }
    void CEngine::AddPlayer(const shared_ptr<CPlayerObject2D>& Player_)
    {
        _Players.emplace_back(Player_);
    }
    void CEngine::Start(void)
    {
        _CurTick.Start();
    }
    bool CEngine::IsStarted(void) const
    {
        return _CurTick.IsStarted();
    }
}
