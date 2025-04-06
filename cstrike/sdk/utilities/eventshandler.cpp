#include "eventshandler.hpp"  
#include "../../core/sdk.h"  
#include "../../sdk/entity.h"  
#include "../../core/variables.h"  
#include "../../resource.h"  

#pragma comment(lib, "winmm.lib")  
#include <windows.h>  
#include <mmsystem.h>  
#include <shlobj.h>      
#include <iostream>  
#include <chrono>  


size_t currentHitSoundIndex = 0;  
std::chrono::steady_clock::time_point lastHitTime;  

//void PlaySound(const char* nameSound)     ТА Я РОТ ТОГО ЕБАЛ, КАК ТЫ РАБОТАЕШЬ ТА НАХУЙ, КАКОЙ ТЕ ПУТЬ НУЖЕН
//{
//	using function_t = void* (__fastcall*)(const char*); //xref: "C:\\buildworker\\csgo_rel_win64\\build\\src\\soundsystem\\snd_dma.cpp",
//	static const function_t fn = reinterpret_cast<function_t>(MEM::FindPattern(SOUND_DLL, CS_XOR("48 89 5C 24 ?? 48 89 74 24 ?? 48 89 7C 24 ?? 55 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 8B")));
//    CS_ASSERT(fn != nullptr);
//	fn(nameSound);
//    L_PRINT(LOG_INFO) << fn(nameSound);
//}

std::wstring GetDocumentsFolder()  
{  
   PWSTR path = nullptr;  
   HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);  
   std::wstring documentsPath;  
   if (SUCCEEDED(hr))  
   {  
       documentsPath = path;  
       CoTaskMemFree(path);  
   }  
   else  
   {  
       L_PRINT(LOG_INFO) << CS_XOR("failed to find hitsounds");  
   }  
   return documentsPath;  
}  

bool CGameEvents::Intilization() {  
   I::GameEventManager->AddListener(this, "player_hurt", false);  
   if (!I::GameEventManager->FindListener(this, "player_hurt"))  
       return false;  

   I::GameEventManager->AddListener(this, "player_death", false);  
   if (!I::GameEventManager->FindListener(this, "player_death"))  
       return false;  

   I::GameEventManager->AddListener(this, "bullet_impact", false);  
   if (!I::GameEventManager->FindListener(this, "bullet_impact"))  
       return false;  

   I::GameEventManager->AddListener(this, "round_start", false);  
   if (!I::GameEventManager->FindListener(this, "round_start"))  
       return false;  

   I::GameEventManager->AddListener(this, "round_end", false);  
   if (!I::GameEventManager->FindListener(this, "round_end"))  
       return false;  

   return true;  
}  

void CGameEvents::FireGameEvent(CGameEvent* pGameEvent) {  
   if (!pGameEvent)  
       return;  

   CGameEventHelper pGameEventHelper = pGameEvent->GetEventHelper();  
   std::string strName = pGameEvent->GetName();  

   if (strName.find("player_hurt") != std::string::npos)  
       this->OnPlayerHurt(pGameEvent, pGameEventHelper);  

   if (strName.find("player_death") != std::string::npos)  
       this->OnPlayerDeath(pGameEvent, pGameEventHelper);  

   if (strName.find("bullet_impact") != std::string::npos)  
       this->OnBulletImpact(pGameEvent, pGameEventHelper);  

   if (strName.find("round_start") != std::string::npos)  
       this->OnRoundStart(pGameEvent, pGameEventHelper);  

   if (strName.find("round_end") != std::string::npos)  
       this->OnRoundEnd(pGameEvent, pGameEventHelper);  
}  

void CGameEvents::OnPlayerHurt(CGameEvent* pGameEvent, CGameEventHelper pGameEventHelper) {
    int iDamage = pGameEventHelper.GetDamage();
    if (iDamage < 1)
        return;

    CCSPlayerController* pVictimController = pGameEventHelper.GetPlayerController();
    CCSPlayerController* pAttackerController = pGameEventHelper.GetAttackerController();
    if (!pVictimController || !pAttackerController)
        return;

    C_CSPlayerPawn* pVictimPawn = pVictimController->GetPlayerPawn();
    if (!pVictimPawn)
        return;

    if (pAttackerController == SDK::LocalController) {
        if (C_GET(bool, Vars.bHitSound)) {
            std::wstring documentsPath = GetDocumentsFolder();
            if (documentsPath.empty())
                return;

            static DWORD g_dwLastHitTime = 0;
            static int g_iCurrentToneIndex = 0;

            DWORD dwCurrentTime = GetTickCount();
            DWORD dwElapsed = dwCurrentTime - g_dwLastHitTime;
            if (dwElapsed <= 1500) {
                if (g_iCurrentToneIndex < 2)
                    g_iCurrentToneIndex++;
            }
            else {
                g_iCurrentToneIndex = 0;
            }
            g_dwLastHitTime = dwCurrentTime;


            wchar_t buffer[256];
            swprintf(buffer, 256, CS_XOR(L"\\.asphyxia\\hitsound_%d.wav"), g_iCurrentToneIndex);
            std::wstring finalPath = documentsPath + CS_XOR(buffer);


            float fVolume = (C_GET(float, Vars.flSoundVolume));
            DWORD dwVolume = static_cast<DWORD>(fVolume * 0xFFFF);
            // DWORD dwVolume = 0x8000;
            waveOutSetVolume(NULL, MAKELONG(dwVolume, dwVolume));

            // Воспроизводим звук асинхронно.
            PlaySoundW(finalPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);

        }
    }
}

void CGameEvents::OnPlayerDeath(CGameEvent* pGameEvent, CGameEventHelper pGameEventHelper) {  
}  

void CGameEvents::OnBulletImpact(CGameEvent* pGameEvent, CGameEventHelper pGameEventHelper) {  
  /* if (!SDK::LocalPawn || !SDK::LocalController)  
       return;  

   CCSPlayerController* pEventController = pGameEventHelper.GetPlayerController();  
   if (pEventController != SDK::LocalController)  
       return;  

   Vector_t vecImpactPos = pGameEvent->GetPosition();*/  
}  

void CGameEvents::OnRoundStart(CGameEvent* pGameEvent, CGameEventHelper pGameEventHelper) {  
}  

void CGameEvents::OnRoundEnd(CGameEvent* pGameEvent, CGameEventHelper pGameEventHelper) {  
}