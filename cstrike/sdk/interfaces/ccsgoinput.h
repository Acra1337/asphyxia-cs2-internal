#pragma once

// used: mem_pad
#include "../../utilities/memory.h"

// used: cusercmd
#include "../datatypes/usercmd.h"

#define MULTIPLAYER_BACKUP 150

#include "../../sdk/entity.h"

class CSubtickMoves {
public:
	float flWhen; //0x0000
	float flDelta; //0x0004
	uint64_t nButton; //0x0008
	bool bPressed; //0x0010
	char pad0011[7]; //0x0011
}; //Size: 0x0018

class CCSInputMessage {
public:
	int32_t iFrameTickCount; // 0x0000
	float flFrameTickFraction; // 0x0004
	int32_t iPlayerTickCount; // 0x0008
	float flPlayerTickFraction; // 0x000C
	QAngle_t angViewAngle; // 0x0010
	Vector_t vecShootPosition; // 0x001C
	int32_t nTargetEntityIndex; // 0x0028
	Vector_t vecTargetHeadPosition; // 0x002C
	Vector_t vecTargetAbsOrigin; // 0x0038
	Vector_t vecTargetAngle; // 0x0044
	int32_t svShowHitRegistration; // 0x0050
	int32_t nEntryIndexMax; // 0x0054
	int32_t nPlayerIndex; //0x0058
	uint32_t nSceneLayer; //0x005C
}; // Size: 0x0060

class CCSGOInput {
public:
	char pad_0000[592]; //0x0000
	bool bBlockShot; //0x0250
	bool bInThirdPerson; //0x0251
	char pad_0252[6]; //0x0252
	QAngle_t angThirdPersonAngles; //0x0258
	char pad_0264[20]; //0x0264
	uint64_t nKeyboardPressed; //0x0278
	uint64_t nMouseWheelheelPressed; //0x0280
	uint64_t nUnPressed; //0x0288
	uint64_t nKeyboardCopy; //0x0290
	float flForwardMove; //0x0298
	float flSideMove; //0x029C
	float flUpMove; //0x02A0
	Vector2D_t nMousePos; //0x02A4
	int32_t iSubticksCount; //0x02AC
	CSubtickMoves pSubtickMoves[12]; //0x02B0
	Vector_t vecViewAngle; //0x03D0
	int32_t nTargetHandle; //0x03DC
	char pad_03E0[560]; //0x03E0
	int32_t nAttackStartHistoryIndex1; //0x0610
	int32_t nAttackStartHistoryIndex2; //0x0614
	int32_t nAttackStartHistoryIndex3; //0x0618
	char pad_061C[4]; //0x061C
	int32_t iMessageSize; //0x0620
	char pad_0624[4]; //0x0624
	CCSInputMessage pInputMessage; //0x0628

	template <typename T>
	void SetViewAngle(T& angView) {
		using SetViewAngles_t = std::int64_t(CS_FASTCALL*)(void*, std::int32_t, T&);
		static auto fnSetViewAngles = reinterpret_cast<SetViewAngles_t>(MEM::FindPattern2(CLIENT_DLL, CS_XOR("85 D2 75 3F 48")).get());

		fnSetViewAngles(this, 0, std::ref(angView));
	}

	QAngle_t GetViewAngles() {
		using GetViewAngles_t = void* (__fastcall*)(CCSGOInput*, int);
		static auto fnGetViewAngles = reinterpret_cast<GetViewAngles_t>(MEM::FindPattern2(CLIENT_DLL, CS_XOR("4C 8B C1 85 D2 74 08 48 8D 05 ? ? ? ? C3")).get());

		return *reinterpret_cast<QAngle_t*>(fnGetViewAngles(this, 0));
	}

	void* GetUserCmdEntry(void* pController, int nCommandIndex) {
		static auto GetUserCmdEntry = reinterpret_cast<void* (__fastcall*)(void*, int)>(MEM::FindPattern2(CLIENT_DLL, CS_XOR("E8 ? ? ? ? 48 8B CF 4C 8B E8 44 8B B8")).abs().get());

		return GetUserCmdEntry(pController, nCommandIndex);
	}

};