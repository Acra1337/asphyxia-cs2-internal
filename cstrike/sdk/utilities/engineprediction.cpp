#include "engineprediction.h"

void CPredictionSystem::RunPrediction(CNetworkGameClient* pNetworkGameClient, int nPredictionReason) {
	using RunPrediction_t = void(__fastcall*)(CNetworkGameClient*, int);
	static RunPrediction_t fnRunPrediction = reinterpret_cast<RunPrediction_t>(MEM::FindPattern(ENGINE2_DLL, "40 55 41 56 48 83 EC ? 80 B9"));
	// @ida: #STR: 'CNetworkGameClient::ClientSidePredict', '%i pred reason %s -- start (latest command created %d)\n', '%i pred reason %s -- finish\n'
	fnRunPrediction(pNetworkGameClient, nPredictionReason);
}

void CPredictionSystem::PhysicsRunThink(CCSPlayerController* pPlayerController) {
	using PhysicsRunThink_t = void* (__fastcall*)(CCSPlayerController*);
	static PhysicsRunThink_t fnPhysicsRunThink = reinterpret_cast<PhysicsRunThink_t>(MEM::FindPattern2(CLIENT_DLL, CS_XOR("E8 ? ? ? ? 49 8B D6 48 8B CE E8 ? ? ? ? 48 8B 06")).abs().get());
	// @ida: #STR: 'controller->PhysicsRunThink()'
	fnPhysicsRunThink(pPlayerController);
}

void CPredictionSystem::InterpolateShootPosition(CCSPlayer_WeaponServices* pWeaponService, Vector_t vecEyePos, int iShootTick) {
	using InterpolateShootPosition_t = void* (__fastcall*)(void*, Vector_t*, int, float);
	static InterpolateShootPosition_t fnInterpolateShootPosition = reinterpret_cast<InterpolateShootPosition_t>(MEM::FindPattern2(CLIENT_DLL, CS_XOR("E8 ? ? ? ? 41 8B 86 ? ? ? ? C1 E8 ? A8 ? 0F 85")).abs().get());
	// @ida: #STR: '%s: GetInterpolatedShootPosition time [ #%d, #%d, %.3f ]\n', 'Interpolating between %.3f, %.3f, %.3f tick %d (%.3f) and %.3f, %.3f, %.3f tick %d (%.3f)\n'
	fnInterpolateShootPosition(pWeaponService, &vecEyePos, iShootTick, 0.0099999998f);
}

int CPredictionSystem::GetPlayerTick(std::uintptr_t* nContainer) {
	using GetPlayerTick_t = int* (__fastcall*)(std::uintptr_t*, std::uintptr_t*, char);
	static GetPlayerTick_t fnGetPlayerTick = reinterpret_cast<GetPlayerTick_t>(MEM::FindPattern(CLIENT_DLL, "48 89 5C 24 08 57 48 83 EC 20 33 C9 41"));

	return *fnGetPlayerTick(nullptr, nContainer, 0);
}

int CPredictionSystem::GetPlayerTick() {
	std::uintptr_t nContainer;
	int iPlayerTick = GetPlayerTick(&nContainer) - 1;

	return iPlayerTick;
}


void CPredictionSystem::Begin() {
	if (!SDK::LocalPawn->IsAlive())
		return;

	CPlayer_MovementServices* pMovementServices = SDK::LocalPawn->GetMovementServices();
	CNetworkGameClient* pNetworkGameClient = I::NetworkClientService->GetNetworkGameClient();
	if (!pMovementServices || !pNetworkGameClient)
		return;

	PredStorage.nFlags = SDK::LocalPawn->GetFlags();

	// store current global variables
	PredStorage.TickBase.flIntervalPerSubtick = I::GlobalVars->flIntervalPerTick;
	PredStorage.TickBase.flCurrentTime = I::GlobalVars->flCurrentTime;
	PredStorage.TickBase.flCurrentTime2 = I::GlobalVars->flCurrentTime2;
	PredStorage.TickBase.iTickCount = I::GlobalVars->nTickCount;
	PredStorage.TickBase.flFrameTime = I::GlobalVars->flFrameTime;
	PredStorage.TickBase.flFrameTime2 = I::GlobalVars->flFrameTime2;
	PredStorage.TickBase.iTickBase = SDK::LocalController->GetTickBase();

	// store prediction state
	PredStorage.bInPrediction = I::Prediction->bInPrediction;

	PredStorage.bFirstPrediction = I::Prediction->bFirstPrediction;

	PredStorage.bHasBeenPredicted = SDK::UserCmd->bHasBeenPredicted;
	PredStorage.bShouldPredict = pNetworkGameClient->bShouldPredict;

	// store weapon data
	if (C_CSWeaponBase* pActiveWeapon = SDK::LocalPawn->GetActiveWeapon()) {
		pActiveWeapon->UpdateAccuracyPenality();

		PredStorage.flSpread = pActiveWeapon->GetSpread();
		PredStorage.flInaccuracy = pActiveWeapon->GetInaccuracy();
	}

	// set prediction state
	SDK::UserCmd->bHasBeenPredicted = false;
	pNetworkGameClient->bShouldPredict = true;
	I::Prediction->bFirstPrediction = false;
	I::Prediction->bInPrediction = true;

	// store network info
	if (CNetworkedClientInfo* pNetworkedClientInfo = I::Engine->GetNetworkedClientInfo()) {
		if (pNetworkedClientInfo->mLocalData)
			PredStorage.vecEyePos = pNetworkedClientInfo->mLocalData->vecEyePos;

		if (pNetworkedClientInfo->flPlayerTickFraction > (1.f - 0.0099999998f))
			PredStorage.TickBase.iTickBase++;
	}

	pMovementServices->SetPredictionCommand(SDK::UserCmd);
	SDK::LocalController->GetCurrentCommand() = SDK::UserCmd;

	PhysicsRunThink(SDK::LocalController);

	// update prediction
	if (pNetworkGameClient->bShouldPredict && pNetworkGameClient->iDeltaTick > 0 && pNetworkGameClient->iSomePredictionTick > 0)
		RunPrediction(pNetworkGameClient, ClientCommandTick);

}

void CPredictionSystem::Update()
{
	CPlayer_MovementServices* pMovementServices = SDK::LocalPawn->GetMovementServices();
	if (!pMovementServices)
		return;

	pMovementServices->RunCommand(SDK::UserCmd);
}

void CPredictionSystem::End() {
	if (!SDK::LocalPawn->IsAlive())
		return;

	CPlayer_MovementServices* pMovementServices = SDK::LocalPawn->GetMovementServices();
	CCSPlayer_WeaponServices* pWeaponService = SDK::LocalPawn->GetWeaponServices();
	CNetworkGameClient* pNetworkGameClient = I::NetworkClientService->GetNetworkGameClient();
	if (!pMovementServices || !pNetworkGameClient)
		return;

	pMovementServices->ResetPredictionCommand();
	SDK::LocalController->GetCurrentCommand() = nullptr;

	I::GlobalVars->flIntervalPerTick = PredStorage.TickBase.flIntervalPerSubtick;
	I::GlobalVars->flCurrentTime = PredStorage.TickBase.flCurrentTime;
	I::GlobalVars->flCurrentTime2 = PredStorage.TickBase.flCurrentTime2;
	I::GlobalVars->nTickCount = PredStorage.TickBase.iTickCount;
	I::GlobalVars->flFrameTime = PredStorage.TickBase.flFrameTime;
	I::GlobalVars->flFrameTime2 = PredStorage.TickBase.flFrameTime2;

	PredStorage.iShootTick = SDK::LocalController->GetTickBase() - 1;
	InterpolateShootPosition(pWeaponService, PredStorage.vecEyePos, PredStorage.iShootTick);

	SDK::LocalController->GetTickBase() = PredStorage.TickBase.iTickBase;

	// restore prediction state
	I::Prediction->bFirstPrediction = PredStorage.bFirstPrediction;
	I::Prediction->bInPrediction = PredStorage.bInPrediction;
	SDK::UserCmd->bHasBeenPredicted = PredStorage.bHasBeenPredicted;
	pNetworkGameClient->bShouldPredict = PredStorage.bShouldPredict;
}