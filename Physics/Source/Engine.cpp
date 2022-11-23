#include "Engine.h"

namespace rso::physics
{
    float CEngine::_ContactOffset;
    int32 CEngine::_FPS;
    int64 CEngine::_UnitTick;
    float CEngine::_DeltaTime;

    CEngine::CEngine(int64 CurTick_, float ContactOffset_, int32 FPS_) :
        _CurTick(CurTick_)
    {
        _Tick = CurTick_;
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

            // �ܺο��� Velocity�� ������ ��� ���� �ݿ��ϱ� ���� �浹 ó�� ���� �÷��̾��� fixedUpdate ó��
            // �׷��� ������ ��ź�� �浹 �Ͽ� �ӵ��� �����Ǿ����
            for (auto& p : _Players)
            {
                if (p->fFixedUpdate)
                    p->fFixedUpdate(_Tick);
            }

            for (size_t pi = 0; pi < _Players.size() - 1; ++pi)
            {
                for (size_t ti = pi + 1; ti < _Players.size(); ++ti)
                    _Players[pi]->CheckOverlapped(_Tick, _Players[ti].get());
            }

            for (auto& p : _Players)
            {
                for (auto it = _MovingObjects.begin(); it;)
                {
                    if (p->CheckOverlapped(_Tick, it->get()))
                    {
                        auto itCheck = it;
                        ++it;
                        RemoveMovingObject(itCheck);
                    }
                    else
                    {
                        ++it;
                    }
                }

                for (auto it = _Objects.begin(); it;)
                {
                    if (p->CheckOverlapped(_Tick, it->get()))
                    {
                        auto itCheck = it;
                        ++it;
                        RemoveObject(itCheck);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }

            if (fFixedUpdate)
                fFixedUpdate();
        }
    }
    CEngine::TObjectsIt CEngine::AddObject(const shared_ptr<CCollider2D>& Object_)
    {
        return _Objects.emplace(Object_);
    }
    void CEngine::RemoveObject(TObjectsIt Iterator_)
    {
        for (auto& i : _Players)
            i->NotOverlapped(_Tick, Iterator_->get());

        _Objects.erase(Iterator_);
    }
    CEngine::TMovingObjectsIt CEngine::AddMovingObject(const shared_ptr<CMovingObject2D>& Object_)
    {
        return _MovingObjects.emplace(Object_);
    }
    void CEngine::RemoveMovingObject(TMovingObjectsIt Iterator_)
    {
        for (auto& i : _Players)
            for (auto& c : Iterator_->get()->Colliders)
                i->NotOverlapped(_Tick, c.get());

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
    void CEngine::Stop(void)
    {
        _CurTick.Stop();
    }
    bool CEngine::IsStarted(void) const
    {
        return _CurTick.IsStarted();
    }
}
