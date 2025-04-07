// cgameeventmanager.cpp
#pragma once

#include "cgameeventmanager.h"
#include "../../sdk/interfaces/igameresourceservice.h"
#include "../../sdk/interfaces/cgameentitysystem.h"

// Ensure the header file where CGameEvent is defined is included
#include "cgameeventmanager.h"

int CGameEvent::GetInt2(const char* Name, bool Unk) {
    using GetEventInt_t = int(__fastcall*)(void*, const char*, bool);
    static GetEventInt_t GetEventInt = reinterpret_cast<GetEventInt_t>(MEM::FindPattern(CLIENT_DLL, "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 41 56 48 83 EC 30 48 8B 01 41 8B F0 4C 8B F1 41 B0 01 48 8D 4C 24 20 48 8B DA 48 8B 78"));

    return GetEventInt(this, Name, Unk);
}

float CGameEvent::GetFloat2(const char* Name, bool Unk) {
    using GetEventFloat_t = float(__fastcall*)(void*, const char*, bool);
    static GetEventFloat_t GetEventFloat = reinterpret_cast<GetEventFloat_t>(MEM::FindPattern(CLIENT_DLL, "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 40 48 8B 01 48 8B F1 0F 29 74 24 30 48 8D 4C 24 20 0F 28 F2 48 8B DA 48 8B 78"));

    return GetEventFloat(this, Name, Unk);
}

Vector_t CGameEvent::GetPosition() {
    std::string_view strTokenX = CS_XOR("x");
    std::string_view strTokenY = CS_XOR("y");
    std::string_view strTokenZ = CS_XOR("z");

    float flPosX = this->GetFloatNew(strTokenX.data());
    float flPosY = this->GetFloatNew(strTokenY.data());
    float flPosZ = this->GetFloatNew(strTokenZ.data());

    return Vector_t(flPosX, flPosY, flPosZ);
}

CCSPlayerController* CGameEventHelper::GetPlayerController() {
    CBuffer pBuffer;
    pBuffer.szName = "userid";

    int iIndex;
    pGameEvent->GetControllerId(iIndex, &pBuffer);
    if (iIndex == -1)
        return nullptr;

    return reinterpret_cast<CCSPlayerController*>(I::GameResourceService->pGameEntitySystem->GetEntityByIndex(iIndex + 1));
}

CCSPlayerController* CGameEventHelper::GetAttackerController() {
    CBuffer pBuffer;
    pBuffer.szName = "attacker";

    int iIndex;
    pGameEvent->GetControllerId(iIndex, &pBuffer);
    if (iIndex == -1)
        return nullptr;

    return reinterpret_cast<CCSPlayerController*>(I::GameResourceService->pGameEntitySystem->GetEntityByIndex(iIndex + 1));
}

int CGameEventHelper::GetDamage() {
    return pGameEvent->GetInt2("dmg_health", false);
}

int CGameEventHelper::GetHealth() {
    CBuffer pBuffer;
    pBuffer.szName = "health";

    return pGameEvent->GetInt(&pBuffer);
}
