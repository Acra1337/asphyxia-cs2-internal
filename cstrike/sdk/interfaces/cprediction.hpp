#pragma once

// used: find pattern, call virtual function
#include "../../utilities/memory.h"
// used: vector
#include "../../sdk/datatypes/vector.h"

#include "inetworkclientservice.h"
#include "../../core/sdk.h"
#include "iengineclient.h"
#include "../entity.h"

class CUserCmd;

class CPrediction {
public:
    char gap0[48];
    int iReason;
    bool bInPrediction;
    bool bEnginePause;
    char gap36[6];
    int iIncomingAcknowledged;
    std::uint64_t deltaTick;
    std::uint64_t slot;
    CCSPlayerController* ControllerEntity;
    char gap58[4];
    int int5C;
    char gap60[32];
    bool bFirstPrediction;
    char pad90[15];
    int iStorageEntitySize;
    char gap94[4];
    void* storageEntitys;
    char gapA0[48];
    int iCountUnknown;
    char gapD4[4];
    void* position;
    char gapE0[8];
    float flRealTime;
    char gapEC[12];
    int iMaxClient;
    char gapFC[12];
    std::uintptr_t* unkClass;
    float flCurrentTime;
    char gap11C[12];
    int iTickCount;
    char gap12C[4];
    char gap130[2];
    char byte132;
    char gap133[3];
    bool bHasUpdate;
};