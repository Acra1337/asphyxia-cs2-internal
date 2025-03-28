#include "../../sdk/interfaces/iengineclient.h" 
#include "hooks.h"
#include "variables.h"
#include "sdk.h"
#include "../../sdk/entity.h"
#include "interfaces.h"

// Existing code
void CS_FASTCALL H::DrawFlashlight(__int64 a1, int a2, __int64* a3, __int64 a4, __m128* a5) {
	if (C_GET(unsigned int, Vars.nViewRemovals) & VIEW_FLASH)
		return;

	hkDrawFlashlight.GetOriginal()(a1, a2, a3, a4, a5);
}

void* CS_FASTCALL H::DrawSmokeArray(void* rcx, void* pSomePointer1, void* pSomePointer2, void* pSomePointer3, void* pSomePointer4, void* pSomePointer5) {
	if ((C_GET(unsigned int, Vars.nViewRemovals) & VIEW_SMOKE) && SDK::LocalController) {
		if (!SDK::LocalController->IsPawnAlive())
			return nullptr;

		if (SDK::LocalController->IsPawnAlive())
			return nullptr;
	}

	hkDrawSmokeArray.GetOriginal()(rcx, pSomePointer1, pSomePointer2, pSomePointer3, pSomePointer4, pSomePointer5);
}



float CS_FASTCALL H::DrawFOV(uintptr_t rcx) {
	const float pFov = hkDrawFOV.GetOriginal()(rcx);

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame() || !SDK::LocalController || !SDK::LocalController->IsPawnAlive())
		return pFov;

	if (C_GET(unsigned int, Vars.nViewRemovals) & VIEW_SCOPE_ZOOM)
		goto CUSTOM;

	//if (ActiveWeapon->GetWeaponVData()->GetWeaponType() == WEAPONTYPE_SNIPER_RIFLE) {
	//	int iZoomLevel = ActiveWeapon->GetZoomLevel();

	//	if (!cheat->canShoot)
	//		goto CUSTOM;

	//	if (iZoomLevel == 1)
	//		return C_GET(float, Vars.flViewFOV) - 60;
	//	else if (iZoomLevel == 2)
	//		return C_GET(float, Vars.flViewFOV) - 90;
	//	else
	//		goto CUSTOM;
	//}
	//else
	//goto CUSTOM;

	return pFov;
CUSTOM:
	return 90;
}