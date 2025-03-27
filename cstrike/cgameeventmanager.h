#pragma once
#pragma once
#include "../../utilities/memory.h"
#include "../../sdk/datatypes/vector.h"

#include "../../sdk/datatypes/stronghandle.h"
#include "../../sdk/datatypes/utlstringtoken.h"

#include "../../core/interfaces.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdigameresourceservice.h"

#include <string>

class CGameEvent;
class CCSPlayerController;

class CBuffer {
public:
    char Pad[0x8];
    const char* szName;
};


class CGameEventHelper {
public:
    CGameEventHelper(CGameEvent* pGameEvent) : pGameEvent{ std::move(pGameEvent) } {};
public:
    CCSPlayerController* GetPlayerController();
    CCSPlayerController* GetAttackerController();
    int GetDamage();
    int GetHealth();

protected:
    CGameEvent* pGameEvent;
};

class CGameEvent {
public:
    virtual ~CGameEvent() = 0;
    virtual const char* GetName() const = 0;

    virtual void Unk1() = 0;
    virtual void Unk2() = 0;
    virtual void Unk3() = 0;
    virtual void Unk4() = 0;

    virtual bool GetBool(const char* KeyName = nullptr, bool DefaultValue = false) = 0;
    virtual int GetInt(CBuffer* KeyName = nullptr, bool DefaultValue = false) = 0;
    virtual uint64_t GetUint64(const char* KeyName = nullptr, uint64_t DefaultValue = 0) = 0;
    virtual float GetFloat(const char* KeyName = nullptr, float DefaultValue = 0.f) = 0;
    virtual const char* GetString(const char* KeyName = nullptr, const char* DefaultValue = "") = 0;

    virtual void Unk5() = 0;
    virtual void Unk6() = 0;
    virtual void Unk7() = 0;
    virtual void Unk8() = 0;

    virtual void GetControllerId(int& ControllerId, CBuffer* Buffer = nullptr) = 0;

    CGameEventHelper GetEventHelper() {
        CGameEventHelper pGameEventHelper(this);
        return pGameEventHelper;
    }

    int GetInt2(const char* Name, bool Unk);
    float GetFloat2(const char* Name, bool Unk);
    Vector_t GetPosition();

    [[nodiscard]] float GetFloatNew(const cUltStringToken& keyToken, const float flDefault = 0.0f) const
    {
        using function_t = float(__thiscall*)(const void*, const cUltStringToken&, float);
        auto vtable = *reinterpret_cast<function_t**>(const_cast<void*>(static_cast<const void*>(this)));
        return vtable[9](this, keyToken, flDefault);
    }
};

class IGameEventListener {
public:
    virtual ~IGameEventListener() {}
    virtual void FireGameEvent(CGameEvent* Event) = 0;
};

class CGameEventManager {
public:
    int LoadEventsFromFile(const char* szFileName) {
        return MEM::CallVFunc<int, 1U>(this, szFileName);
    }

    void Reset() {
        MEM::CallVFunc<void, 2U>(this);
    }

    bool AddListener(IGameEventListener* pListener, const char* szName, bool bServerSide = false) {
        return MEM::CallVFunc<bool, 3U>(this, pListener, szName, bServerSide);
    }

    bool FindListener(IGameEventListener* pListener, const char* szName) {
        return MEM::CallVFunc<bool, 4U>(this, pListener, szName);
    }

    void RemoveListener(IGameEventListener* pListener) {
        MEM::CallVFunc<void, 5U>(this, pListener);
    }
};