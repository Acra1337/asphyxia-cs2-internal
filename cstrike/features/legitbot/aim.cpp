#include "aim.h"

// used: sdk entity
#include "../../sdk/entity.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdk/interfaces/iengineclient.h"
// used: cusercmd
#include "../../sdk/datatypes/usercmd.h"

// used: activation button
#include "../../utilities/inputsystem.h"

// used: cheat variables
#include "../../core/variables.h"

#include "../../sdk/interfaces/cgametracemanager.h"
#include "../../core/sdk.h"

#include "../../sdk/utilities/penetration.h"
#include <chrono>
#include "../../core/menu.h"


class Timer {
public:
	void Trigger() {
		lastTriggerTime = std::chrono::steady_clock::now();
		isBlocked = true;
	}

	bool IsBlocked(float delaySeconds) {
		if (!isBlocked) return false;

		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration<float>(now - lastTriggerTime).count();

		if (elapsed >= delaySeconds) {
			isBlocked = false;
		}

		return isBlocked;
	}

private:
	std::chrono::steady_clock::time_point lastTriggerTime;
	bool isBlocked = false;
};



void F::LEGITBOT::AIM::OnMove(CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd, CCSPlayerController* pLocalController, C_CSPlayerPawn* pLocalPawn)
{
	// Check if the legitbot is enabled
	if (!C_GET(bool, Vars.bLegitbot))
		return;

	if (!pLocalController->IsPawnAlive())
		return;

	AimAssist(pBaseCmd, pLocalPawn, pLocalController);
}

QAngle_t GetAimPunch(C_CSPlayerPawn* pLocalPawn) {
	using GetAimPunch_t = float(__fastcall*)(void*, Vector_t*, float, bool);
	static GetAimPunch_t GetAimPunch = reinterpret_cast<GetAimPunch_t>(MEM::FindPattern(CLIENT_DLL, CS_XOR("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F2 0F 29 74 24 ? 48 8B D1")));

	Vector_t AimPunchAngle;
	GetAimPunch(pLocalPawn, &AimPunchAngle, 0.0f, true);

	return QAngle_t(AimPunchAngle.x, AimPunchAngle.y, AimPunchAngle.z);
}

QAngle_t GetRecoil(C_CSPlayerPawn* pLocalPawn) {
	QAngle_t AimPunchAngle;


	return GetAimPunch(pLocalPawn);
}
QAngle_t GetAngularDifference(CBaseUserCmdPB* pCmd, Vector_t vecTarget, C_CSPlayerPawn* pLocal)
{
	// The current position
	Vector_t vecCurrent = pLocal->GetEyePosition();

	// The new angle
	QAngle_t vNewAngle = (vecTarget - vecCurrent).ToAngles();
	vNewAngle.Normalize(); // Normalise it so we don't jitter about

	// Store our current angles
	QAngle_t vCurAngle = pCmd->pViewAngles->angValue;

	// Find the difference between the two angles (later useful when adding smoothing)
	vNewAngle -= vCurAngle;

	return vNewAngle;
}
void AutoStop() {
	if (SDK::UserCmd->nButtons.nValue & IN_DUCK)
		return;

	if (!(SDK::LocalPawn->GetFlags() & FL_ONGROUND))
		return;

	SDK::BaseCmd->SetBits(BASE_BITS_FORWARDMOVE);
	SDK::BaseCmd->SetBits(BASE_BITS_LEFTMOVE);

	SDK::BaseCmd->flSideMove = 0.0f;
	SDK::BaseCmd->flForwardMove = SDK::LocalPawn->GetVecVelocity().Length2D() > 20.0f ? 1.0f : 0.0f;

	float flYaw = SDK::LocalPawn->GetVecVelocity().ToAngles().y + 180.0f;
	float flRotation = M_DEG2RAD(SDK::pData->ViewAngle.y - flYaw);

	float flCosRotation = std::cos(flRotation);
	float flSinRotation = std::sin(flRotation);

	float flNewForwardMove = flCosRotation * SDK::BaseCmd->flForwardMove - flSinRotation * SDK::BaseCmd->flSideMove;
	float flNewSideMove = flSinRotation * SDK::BaseCmd->flForwardMove + flCosRotation * SDK::BaseCmd->flSideMove;

	SDK::BaseCmd->flForwardMove = flNewForwardMove*0.6;
	SDK::BaseCmd->flSideMove = -flNewSideMove*0.6;
}

float GetAngularDistance(CBaseUserCmdPB* pCmd, Vector_t vecTarget, C_CSPlayerPawn* pLocal)
{
	return GetAngularDifference(pCmd, vecTarget, pLocal).Length2D();
}

// Задержка удержания кнопки (в миллисекундах)
const int PRESS_DURATION_MS = 40;

// Глобальные переменные для отслеживания состояния
bool g_isMousePressed = false;
std::chrono::steady_clock::time_point g_pressTime;

void TriggerMousePress()
{
	if (!g_isMousePressed)
	{
		// Отправляем событие нажатия
		INPUT input = {};
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(INPUT));

		// Фиксируем время нажатия
		g_pressTime = std::chrono::steady_clock::now();
		g_isMousePressed = true;
	}
}

// Функция для проверки времени удержания и отпускания левой кнопки мыши
void UpdateMouseRelease()
{
	if (g_isMousePressed)
	{
		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - g_pressTime).count();
		if (elapsed >= PRESS_DURATION_MS)
		{
			// Отправляем событие отпускания
			INPUT input = {};
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(1, &input, sizeof(INPUT));

			g_isMousePressed = false;
		}
	}
}

void ActionFire() {

	/*SDK::UserCmd->nButtons.nValue |= IN_ATTACK;
	SDK::UserCmd->nButtons.nValueChanged |= IN_ATTACK;

	SDK::UserCmd->csgoUserCmd.nAttack1StartHistoryIndex = 0;
	SDK::UserCmd->csgoUserCmd.CheckAndSetBits(CSGOUSERCMD_BITS_ATTACK1START);*/
	TriggerMousePress();
}

Timer myTimer;
float delay = 0.400f; 

void F::LEGITBOT::AIM::AimAssist(CBaseUserCmdPB* pUserCmd, C_CSPlayerPawn* pLocalPawn, CCSPlayerController* pLocalController)
{
	// Check if the activation key is down

	if (!IPT::IsKeyDown(C_GET(unsigned int, Vars.nLegitbotActivationKey)) && !C_GET(bool, Vars.bLegitbotAlwaysOn) || MENU::bMainWindowOpened == 1)
		return;
	float Damage = 0;
	// The current best distance
	float flDistance = INFINITY;
	// The target we have chosen
	CCSPlayerController* pTarget = nullptr;
	// Cache'd position
	Vector_t vecBestPosition = Vector_t();

	// Entity loop
	const int iHighestIndex = 126;
	UpdateMouseRelease();
	for (int nIndex = 1; nIndex <= iHighestIndex; nIndex++)
	{
		if (myTimer.IsBlocked(delay))
			continue;

		// Get the entity
		C_BaseEntity* pEntity = I::GameResourceService->pGameEntitySystem->Get(nIndex);
		if (pEntity == nullptr)
			continue;

		// Get the class info
		SchemaClassInfoData_t* pClassInfo = nullptr;
		pEntity->GetSchemaClassInfo(&pClassInfo);
		if (pClassInfo == nullptr)
			continue;

		// Get the hashed name
		const FNV1A_t uHashedName = FNV1A::Hash(pClassInfo->szName);

		// Make sure they're a player controller
		if (uHashedName != FNV1A::HashConst("CCSPlayerController"))
			continue;

		// Cast to player controller
		CCSPlayerController* pPlayer = reinterpret_cast<CCSPlayerController*>(pEntity);
		if (pPlayer == nullptr)
			continue;

		// Check the entity is not us
		if (pPlayer == pLocalController)
			continue;

		// Get the player pawn
		C_CSPlayerPawn* pPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pPlayer->GetPawnHandle());
		if (pPawn == nullptr)
			continue;
		// Make sure they're alive
		if (!pPlayer->IsPawnAlive())
			continue;

		// Check if they're an enemy
		if (!pLocalPawn->IsOtherEnemy(pPawn))
			continue;

		// Check if they're dormant
		CGameSceneNode* pCGameSceneNode = pPawn->GetGameSceneNode();
		if (pCGameSceneNode == nullptr || pCGameSceneNode->IsDormant())
			continue;

		// Get the position

		// Firstly, get the skeleton
		CSkeletonInstance* pSkeleton = pCGameSceneNode->GetSkeletonInstance();
		if (pSkeleton == nullptr)
			continue;
		// Now the bones
		Matrix2x4_t* pBoneCache = pSkeleton->pBoneCache;
		if (pBoneCache == nullptr)
			continue;

		const int iBone = 6; // You may wish to change this dynamically but for now let's target the head.

		// Get the bone's position
		Vector_t vecPos = pBoneCache->GetOrigin(iBone);

		// @note: this is a simple example of how to check if the player is visible

		// initialize trace, construct filterr and initialize ray
		GameTrace_t trace = GameTrace_t();
		TraceFilter_t filter = TraceFilter_t(0x1C3003, pLocalPawn, nullptr, 4);
		Ray_t ray = Ray_t();

		// cast a ray from local player eye positon -> player head bone
		// @note: would recommend checking for nullptrs
		I::GameTraceManager->TraceShape(&ray, pLocalPawn->GetEyePosition(), pPawn->GetGameSceneNode()->GetSkeletonInstance()->pBoneCache->GetOrigin(6), &filter, &trace);
		// check if the hit entity is the one we wanted to check and if the trace end point is visible
		
		float flCurrentDistance = GetAngularDistance(pUserCmd, vecPos, pLocalPawn);
		if (flCurrentDistance > C_GET(float, Vars.flAimRange))// Skip if this move out of aim range
			continue;
		if (pTarget && flCurrentDistance > flDistance) // Override if this is the first move or if it is a better move
			continue;
		
		if (C_GET(bool, Vars.bAutoWall)) {
			AutoWall::mData.iHitGroup = iBone;
			AutoWall::mData.pTargetPawn = pPawn;
			AutoWall::mData.vecEndPos = pPawn->GetGameSceneNode()->GetSkeletonInstance()->pBoneCache->GetOrigin(6);
			AutoWall::mData.vecStartPos = pLocalPawn->GetEyePosition();

			Damage = static_cast<float>(AutoWall::mData.flDamage);
			if (!AutoWall::FireBullet(AutoWall::mData))
				continue;
		}
		else {
			Damage = 0;
		}
		//if (trace.m_pHitEntity != pPawn || (!trace.IsVisible() || static_cast<int>(Damage < 0))// if invisible, skip this entity
		//	continue;

		if (!(trace.m_pHitEntity == pPawn || Damage > 0))
			continue;


		// Get the distance/weight of the move
		
		// Better move found, override.
		pTarget = pPlayer;
		flDistance = flCurrentDistance;
		vecBestPosition = vecPos;
	}

	// Check if a target was found
	if (pTarget == nullptr)
		return;

	Vector_t velocity = SDK::LocalPawn->GetVecVelocity();
	float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	/*if (speed > 180)
		return;*/
	// Point at them
	QAngle_t* pViewAngles = &(pUserCmd->pViewAngles->angValue); // Just for readability sake!

	// Find the change in angles
	QAngle_t vNewAngles = GetAngularDifference(pUserCmd, vecBestPosition, pLocalPawn);
	vNewAngles.x = vNewAngles.x - 0.2f;
	float hit_chnce = 200 - 2 * C_GET(float, Vars.fHitChance);
	if (speed > hit_chnce && C_GET(bool, Vars.bAutoStop)) {
		AutoStop();
	}
	if (abs(static_cast<float>(vNewAngles.x)) < 0.4 && abs(static_cast<float>(vNewAngles.y)) < 0.3) {
		if (speed < hit_chnce) {
			if (C_GET(bool, Vars.bAutoFire)) {
				ActionFire();
				myTimer.Trigger();
			}
		}
	}
	// Get the smoothing
	const float flSmoothing = C_GET(float, Vars.flSmoothing);
	auto aimPunch =  GetRecoil(pLocalPawn); //get AimPunch angles
	// Apply smoothing and set angles
	pViewAngles->x +=  ( vNewAngles.x - aimPunch.x ) / flSmoothing;// minus AimPunch angle to counteract recoil
	pViewAngles->y +=  ( vNewAngles.y - aimPunch.y ) / flSmoothing;
	pViewAngles->Normalize();
	//L_PRINT(LOG_INFO) << MENU::bMainWindowOpened;
	// Point at them
	

}
