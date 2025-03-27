// used: game trace manager
#include "cgametracemanager.h"
// used: c_csplayerpawn
#include "../../sdk/entity.h"

SurfaceData_t* GameTrace_t::GetSurfaceData()
{
	using fnGetSurfaceData = std::uint64_t(__fastcall*)(void*);
	static fnGetSurfaceData oGetSurfaceData = reinterpret_cast< fnGetSurfaceData >( MEM::GetAbsoluteAddress( MEM::FindPattern( CLIENT_DLL, CS_XOR( "E8 ? ? ? ? 48 85 C0 74 ? 44 38 68" ) ), 0x1, 0x0 ) );

#ifdef CS_PARANOID
	CS_ASSERT(oGetSurfaceData != nullptr);
#endif

	return reinterpret_cast<SurfaceData_t*>(oGetSurfaceData(m_pSurface));
}
void CGameTraceManager::InitializeTraceInfo(GameTrace_t* const hit) {
	using InitializeTraceInfo_t = void(__fastcall*)(GameTrace_t*);
	static InitializeTraceInfo_t fnInitializeTraceInfo = reinterpret_cast<InitializeTraceInfo_t>(MEM::FindPattern2(CLIENT_DLL, SIG_InitializeTrace).get());

	CS_ASSERT(fnInitializeTraceInfo != nullptr);
	fnInitializeTraceInfo(hit);
}

void CGameTraceManager::GetTraceInfo(TraceData_t* trace, GameTrace_t* hit, const float unknown_float, void* unknown) {
	using GetTraceInfo_t = void(__fastcall*)(TraceData_t*, GameTrace_t*, float, void*);
	static GetTraceInfo_t fnGetTraceInfo = reinterpret_cast<GetTraceInfo_t>(MEM::FindPattern2(CLIENT_DLL, SIG_GetTraceInfo).get());

	CS_ASSERT(fnGetTraceInfo != nullptr);
	return fnGetTraceInfo(trace, hit, unknown_float, unknown);
}
bool CGameTraceManager::HandleBulletPenetration(TraceData_t* const pTraceData, HandleBulletPenetrationData_t* pHandleBulletPenetrationData, UpdateValue_t* const pModValue, const bool bDrawShowimpacts) {
	using HandleBulletPenetration_t = bool(__fastcall*)(TraceData_t*, HandleBulletPenetrationData_t*, UpdateValue_t*, void*, void*, void*, void*, void*, bool);
	static HandleBulletPenetration_t fnHandleBulletPenetration = reinterpret_cast<HandleBulletPenetration_t>(MEM::FindPattern2(CLIENT_DLL, SIG_HandleBulletPenetration).get());

	CS_ASSERT(fnHandleBulletPenetration != nullptr);
	return fnHandleBulletPenetration(pTraceData, pHandleBulletPenetrationData, pModValue, nullptr, nullptr, nullptr, nullptr, nullptr, bDrawShowimpacts);
}


void CGameTraceManager::TraceInit(TraceFilter_t& filter, C_CSPlayerPawn* skip, uint64_t mask, uint8_t layer, uint16_t idk) {
	using TraceInit_t = TraceFilter_t * (__fastcall*)(TraceFilter_t&, void*, uint64_t, uint8_t, uint16_t);
	static TraceInit_t fnTraceInit = reinterpret_cast<TraceInit_t>(MEM::FindPattern2(CLIENT_DLL, SIG_TraceInit).get());

	CS_ASSERT(fnTraceInit != nullptr);
	fnTraceInit(filter, skip, mask, layer, idk);
}

void CGameTraceManager::CreateTrace(TraceData_t* const trace, const Vector_t start, const Vector_t end, const TraceFilter_t& filler, const int penetration_count) {
	using CreateTrace_t = void(__fastcall*)(TraceData_t*, Vector_t, Vector_t, TraceFilter_t, int);
	static CreateTrace_t fnCreateTrace = reinterpret_cast<CreateTrace_t>(MEM::FindPattern2(CLIENT_DLL, SIG_CreateTrace).get());

	CS_ASSERT(fnCreateTrace != nullptr);
	return fnCreateTrace(trace, start, end, filler, penetration_count);
}

int GameTrace_t::GetHitboxId()
{
	if (m_pHitboxData)
		return m_pHitboxData->m_nHitboxId;
	return 0;
}

int GameTrace_t::GetHitgroup()
{
	if (m_pHitboxData)
		return m_pHitboxData->m_nHitGroup;
	return 0;
}

bool GameTrace_t::IsVisible() const
{
	return (m_flFraction > 0.97f);
}



TraceFilter_t::TraceFilter_t(std::uint64_t uMask, C_CSPlayerPawn* pSkip1, C_CSPlayerPawn* pSkip2, int nLayer)
{
	m_uTraceMask = uMask;
	m_v1[0] = m_v1[1] = 0;
	m_v2 = 7;
	m_v3 = nLayer;
	m_v4 = 0x49;
	m_v5 = 0;

	if (pSkip1 != nullptr)
	{
		m_arrSkipHandles[0] = pSkip1->GetRefEHandle().GetEntryIndex();
		m_arrSkipHandles[2] = pSkip1->GetOwnerHandleIndex();
		m_arrCollisions[0] = pSkip1->GetCollisionMask();
	}

	if (pSkip2 != nullptr)
	{
		m_arrSkipHandles[0] = pSkip2->GetRefEHandle().GetEntryIndex();
		m_arrSkipHandles[2] = pSkip2->GetOwnerHandleIndex();
		m_arrCollisions[0] = pSkip2->GetCollisionMask();
	}
}
