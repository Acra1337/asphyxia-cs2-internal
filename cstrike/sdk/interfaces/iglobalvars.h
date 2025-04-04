#pragma once

// used: mem_pad
#include "../../utilities/memory.h"


class IGlobalVars {
public:
    float flRealTime; //0x0000
    int32_t iFrameCount; //0x0004
    float flFrameTime; //0x0008  // flAbsoluteFrameTime
    float flFrameTime2; //0x000C // flAbsoluteFrameStartTimeStdDev
    int32_t nMaxClients; //0x0010
    char pad_0014[28]; //0x0014
    float flIntervalPerTick; //0x0030
    float flCurrentTime; //0x0034
    float flCurrentTime2; //0x0038
    char pad_003C[20]; //0x003C
    int32_t nTickCount; //0x0050
    char pad_0054[292]; //0x0054
    uint64_t uCurrentMap; //0x0178
    uint64_t uCurrentMapName; //0x0180

    //float flIntervalPerTick = 1 / 64;
}; //Size: 0x0188