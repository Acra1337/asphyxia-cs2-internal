#pragma once
// used: pad and findpattern
#include "../../utilities/memory.h"
// used: vector
#include "../../sdk/datatypes/vector.h"
// used: array
#include <array>

#define SIG_CreateTrace				CS_XOR("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 56 41 57 48 83 EC 40 F2")
#define SIG_TraceInit				CS_XOR("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 0F B6 41 37 33")
#define SIG_InitializeTrace			CS_XOR("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 33 FF 48 8B 0D ? ? ? ? 48 85 C9")
#define SIG_GetTraceInfo			CS_XOR("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 60 48 8B E9 0F")
#define SIG_HandleBulletPenetration CS_XOR("48 8B C4 44 89 48 ? 48 89 50 ? 48 89 48")


struct Ray_t
{
public:
	Vector_t m_vecStart;
	Vector_t m_vecEnd;
	Vector_t m_vecMins;
	Vector_t m_vecMaxs;
	MEM_PAD(0x4);
	std::uint8_t UnkType;
};
static_assert(sizeof(Ray_t) == 0x38);

struct SurfaceData_t
{
public:
	MEM_PAD(0x8)
	float m_flPenetrationModifier;
	float m_flDamageModifier;
	MEM_PAD(0x4)
	int m_iMaterial;
};

static_assert(sizeof(SurfaceData_t) == 0x18);

struct TraceHitboxData_t
{
public:
	MEM_PAD(0x38);
	int m_nHitGroup;
	MEM_PAD(0x4);
	int m_nHitboxId;
};
static_assert(sizeof(TraceHitboxData_t) == 0x44);

class C_CSPlayerPawn;
struct GameTrace_t
{
public:
	GameTrace_t() = default;

	SurfaceData_t* GetSurfaceData();
	int GetHitboxId();
	int GetHitgroup();
	bool IsVisible() const;

	void* m_pSurface;
	C_CSPlayerPawn* m_pHitEntity;
	TraceHitboxData_t* m_pHitboxData;
	MEM_PAD(0x38);
	std::uint32_t m_uContents;
	MEM_PAD(0x24);
	Vector_t m_vecStartPos;
	Vector_t m_vecEndPos;
	Vector_t m_vecNormal;
	Vector_t m_vecPosition;
	float flFraction; // Not work
	MEM_PAD(0x4);
	float m_flFraction;
	MEM_PAD(0x6);
	bool m_bAllSolid;
	MEM_PAD(0x4D)
}; // Size: 0x108

static_assert(sizeof(GameTrace_t) == 0x108);

struct HandleBulletPenetrationData_t {
	HandleBulletPenetrationData_t(const float flDamage, const float flPenetration, const float flRangeModifier, const float flRange,
		const int iPenetrationCount, const bool bFailed) :
		flDamage(flDamage),
		flPenetration(flPenetration),
		flRangeModifier(flRangeModifier),
		flRange(flRange),
		iPenetrationCount(iPenetrationCount),
		bFailed(bFailed)
	{
	}

	float flDamage{};
	float flPenetration{};
	float flRangeModifier{};
	float flRange{};
	int iPenetrationCount{};
	bool bFailed{};
};


struct TraceFilter_t
{
public:
	MEM_PAD(0x8);
	std::int64_t m_uTraceMask;
	std::array<std::int64_t, 2> m_v1;
	std::array<std::int32_t, 4> m_arrSkipHandles;
	std::array<std::int16_t, 2> m_arrCollisions;
	std::int16_t m_v2;
	std::uint8_t m_v3;
	std::uint8_t m_v4;
	std::uint8_t m_v5;

	TraceFilter_t() = default;
	TraceFilter_t(std::uint64_t uMask, C_CSPlayerPawn* pSkip1, C_CSPlayerPawn* pSkip2, int nLayer);
};
static_assert(sizeof(TraceFilter_t) == 0x40);

struct TraceArrElement_t {
	MEM_PAD(0x30);
};

struct UpdateValue_t {
	float flPreviousLenght{};
	float flCurrentLenght{};
	MEM_PAD(0x8);
	std::int16_t nHandleIndex{};
	MEM_PAD(0x6);
};

struct TraceData_t {
	std::int32_t v1{};
	float v2{ 52.0f };
	void* pArrayPointer{};
	std::int32_t v3{ 128 };
	std::int32_t v4{ static_cast<std::int32_t>(0x80000000) };
	std::array<TraceArrElement_t, 0x80> arrTraceElemets = {};
	MEM_PAD(0x8);
	std::int64_t uNumUpdate{};
	void* pUpdateValue{};
	MEM_PAD(0xC8);
	Vector_t vecStart{}, vecEnd{};
	MEM_PAD(0x50);
};

class CGameTraceManager
{
public:

	void CreateTrace(TraceData_t* const trace, const Vector_t start, const Vector_t end, const TraceFilter_t& filler, const int penetration_count);
	void TraceInit(TraceFilter_t& filter, C_CSPlayerPawn* skip, uint64_t mask, uint8_t layer, uint16_t idk);
	void GetTraceInfo(TraceData_t* trace, GameTrace_t* hit, const float unknown_float, void* unknown);
	bool HandleBulletPenetration(TraceData_t* const pTraceData, HandleBulletPenetrationData_t* pHandleBulletPenetrationData, UpdateValue_t* const pModValue, const bool bDrawShowimpacts = false);
	void InitializeTraceInfo(GameTrace_t* const hit);

	bool TraceShape(Ray_t* pRay, Vector_t vecStart, Vector_t vecEnd, TraceFilter_t* pFilter, GameTrace_t* pGameTrace)
	{
		using fnTraceShape = bool(__fastcall*)(CGameTraceManager*, Ray_t*, Vector_t*, Vector_t*, TraceFilter_t*, GameTrace_t*);
		// Credit: https://www.unknowncheats.me/forum/4265752-post6333.html
		static fnTraceShape oTraceShape = reinterpret_cast<fnTraceShape>(MEM::FindPattern(CLIENT_DLL, CS_XOR("48 89 5C 24 ? 48 89 4C 24 ? 55 56 41 55")));

		#ifdef CS_PARANOID
		CS_ASSERT(oTraceShape != nullptr);
		#endif

		return oTraceShape(this, pRay, &vecStart, &vecEnd, pFilter, pGameTrace);
	}

	bool ClipRayToEntity(Ray_t* pRay, Vector_t vecStart, Vector_t vecEnd, C_CSPlayerPawn* pPawn, TraceFilter_t* pFilter, GameTrace_t* pGameTrace)
	{
		using fnClipRayToEntity = bool(__fastcall*)(CGameTraceManager*, Ray_t*, Vector_t*, Vector_t*, C_CSPlayerPawn*, TraceFilter_t*, GameTrace_t*);
		static fnClipRayToEntity oClipRayToEntity = reinterpret_cast<fnClipRayToEntity>(MEM::FindPattern(CLIENT_DLL, CS_XOR("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC C0 00 00 00 48 8B 9C")));

		#ifdef CS_PARANOID
		CS_ASSERT(oClipRayToEntity != nullptr);
		#endif

		return oClipRayToEntity(this, pRay, &vecStart, &vecEnd, pPawn, pFilter, pGameTrace);
	}
};
