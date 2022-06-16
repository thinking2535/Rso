#pragma once

#include "Engine.h"

namespace rso::physics
{
    class CServerEngine : public CEngine
    {
    public:
        using FSyncMessage = function<void(int64 Tick_)>;
        FSyncMessage fSyncMessage;
    private:
        int64 _LastSentTick = 0;

    public:
        CServerEngine(int64 NetworkTickSync_, int64 CurTick_, float ContactOffset_, int32 FPS_, FSyncMessage fSyncMessage_);
        void UpdateAndSyncMessage(void);
        void Update(void);
        void Send(void);
    };
}
