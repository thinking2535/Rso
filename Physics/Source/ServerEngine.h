#pragma once

#include "NetworkEngine.h"

namespace rso::physics
{
    class CServerEngine : public CNetworkEngine
    {
    public:
        using FSyncMessage = function<void()>;
        FSyncMessage fSyncMessage;
    private:
        int64 _LastSentTick = 0;

    public:
        CServerEngine(int64 CurTick_, float ContactOffset_, int32 FPS_, FSyncMessage fSyncMessage_, int64 NetworkTickSync_);
        void UpdateAndSyncMessage(void);
        void Update(void) override;
        void Send(void);
    };
}
