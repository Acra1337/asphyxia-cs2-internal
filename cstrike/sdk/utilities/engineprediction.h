#pragma once

#include "../interfaces/inetworkclientservice.h"
#include "../interfaces/cprediction.hpp"
#include "../entity.h"

struct CPredictionStorage {
    void reset() {
        *this = {};
    }

    uint32_t nFlags{};
    Vector_t vecEyePos{};

    bool bInPrediction = false;
    bool bFirstPrediction = false;
    bool bHasBeenPredicted = false;
    bool bShouldPredict = false;

    float flSpread = 0.0f;
    float flInaccuracy = 0.0f;

    int  iShootTick = 0;

    struct TickBase_t {
        int iTickBase = 0;
        int iTickCount = 0;
        float flIntervalPerSubtick = 0.0f;
        float flCurrentTime = 0.0f;
        float flCurrentTime2 = 0.0f;

        float flFrameTime = 0.0f;
        float flFrameTime2 = 0.0f;
    } TickBase = {};
};

class CPredictionSystem {
private:
    void RunPrediction(CNetworkGameClient* pNetworkGameClient, int nPredictionReason);
    void PhysicsRunThink(CCSPlayerController* pPlayerController);
    void InterpolateShootPosition(CCSPlayer_WeaponServices* pWeaponService, Vector_t vecEyePos, int iShootTick);
    int GetPlayerTick(std::uintptr_t* nContainer);

public:
    void Update();
    void Begin();
    void End();

    int GetPlayerTick();

public:
    CPredictionStorage PredStorage;
};

inline CPredictionSystem* PredictionSystem = new CPredictionSystem();