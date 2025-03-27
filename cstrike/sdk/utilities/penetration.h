#pragma once
#include "../../sdk/entity.h"
#include "../../core/sdk.h"
#include "../../sdk/vdata.h"
#include "../../sdk/interfaces/cgametracemanager.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdk/interfaces/ienginecvar.h"
#include "../../core/convars.h"

namespace AutoWall {
    static struct Data_t {
        C_CSPlayerPawn* pTargetPawn{};
        Vector_t vecStartPos{};
        Vector_t vecEndPos{};
        float flDamage{};
        int iHitGroup{};
    } mData;

    void ScaleDamage(Data_t& mData);
    bool FireBullet(Data_t& mData);

    static float flAmountExtrapolate = 3.f;
}