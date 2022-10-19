#include "ServerEngine.h"

namespace rso::physics
{
    CServerEngine::CServerEngine(int64 CurTick_, float ContactOffset_, int32 FPS_, FSyncMessage fSyncMessage_, int64 NetworkTickSync_) :
        CNetworkEngine(CurTick_, ContactOffset_, FPS_, NetworkTickSync_),
        fSyncMessage(fSyncMessage_)
    {
    }
    void CServerEngine::UpdateAndSyncMessage(void)
    {
        Update();

        if (_Tick - _LastSentTick > _NetworkTickSync)
        {
            Send();
            fSyncMessage();
        }
    }
    void CServerEngine::Update(void)
    {
        _Update(_CurTick.Get());
    }
    void CServerEngine::Send(void)
    {
        _LastSentTick = _Tick;
    }
}
