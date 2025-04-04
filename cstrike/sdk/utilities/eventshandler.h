#pragma once

#include "../../sdk/interfaces/cgameeventmanager.h"

class CGameEvents : public IGameEventListener {
public:
    void FireGameEvent(CGameEvent* pGameEvent);
    bool Intilization();

    void OnPlayerHurt(CGameEvent* pGameEvent, CGameEventHelper pGameEventHelper);
    void OnPlayerDeath(CGameEvent* pGameEvent, CGameEventHelper pGameEventHelper);
    void OnBulletImpact(CGameEvent* pGameEvent, CGameEventHelper pGameEventHelper);
    void OnRoundStart(CGameEvent* pGameEvent, CGameEventHelper pGameEventHelper);
    void OnRoundEnd(CGameEvent* pGameEvent, CGameEventHelper pGameEventHelper);
};

inline CGameEvents* GameEvents = new CGameEvents();