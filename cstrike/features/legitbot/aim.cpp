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
#include <random>


double sqrt3, sqrt5;

void init_constants() {
	sqrt3 = sqrt(3);
	sqrt5 = sqrt(5);
}

struct TwoFloats {
	float first;
	float second;
};

TwoFloats wind_mouse(int start_x, int start_y, float dest_x, float dest_y, double G_0, double W_0, double M_0, double D_0) {
	int current_x = start_x;
	int current_y = start_y;
	double v_x = 0, v_y = 0, W_x = 0, W_y = 0;

	while (1) {
		double dist = sqrt(pow(dest_x - start_x, 2) + pow(dest_y - start_y, 2));
		if (dist < 0.01) {
			return { dest_x , dest_y };
			break;
		}

		double W_mag = fmin(W_0, dist);

		if (dist >= D_0) {
			W_x = W_x / sqrt3 + ((2.0 * 0.5f + (static_cast<float>(rand()) / RAND_MAX) * 0.5f) - 1) * W_mag / sqrt5;
			W_y = W_y / sqrt3 + ((2.0 * 0.5f + (static_cast<float>(rand()) / RAND_MAX) * 0.5f) - 1) * W_mag / sqrt5;
		}
		else {
			W_x /= sqrt3;
			W_y /= sqrt3;
			if (M_0 < 3) {
				M_0 = (3 + (rand() / (double)RAND_MAX) * 3);
			}
			else {
				M_0 /= sqrt5;
			}
		}

		v_x += W_x + G_0 * (dest_x - start_x) / dist;
		v_y += W_y + G_0 * (dest_y - start_y) / dist;

		double v_mag = sqrt(v_x * v_x + v_y * v_y);
		if (v_mag > M_0) {
			double v_clip = M_0 / 2 + (rand() / (double)RAND_MAX) * M_0 / 2;
			v_x = (v_x / v_mag) * v_clip;
			v_y = (v_y / v_mag) * v_clip;
		}

		TwoFloats result = { v_x/5, v_y/5 };
		return result;
	}

}

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
void AutoStop(CBaseUserCmdPB* pUserCmd) {
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

	float flNewForwardMove = (flCosRotation * SDK::BaseCmd->flForwardMove - flSinRotation)*0.7 * SDK::BaseCmd->flSideMove;
	float flNewSideMove = (flSinRotation * SDK::BaseCmd->flForwardMove + flCosRotation)*0.7 * SDK::BaseCmd->flSideMove;

	/*SDK::BaseCmd->flForwardMove = flNewForwardMove*2;
	SDK::BaseCmd->flSideMove = -flNewSideMove*2;*/
	pUserCmd->flForwardMove = flNewForwardMove * 2;
	pUserCmd->flSideMove = -flNewSideMove * 2;
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
	float flSmoothing = 1.0f;
	if (!IPT::IsKeyDown(C_GET(unsigned int, Vars.nLegitbotActivationKey)) && !C_GET(bool, Vars.bLegitbotAlwaysOn) || MENU::bMainWindowOpened == 1)
		return;
	// Seed the random number generator
	srand(time(NULL));
	init_constants();
	float Damage = 0;
	// The current best distance
	float flDistance = INFINITY;
	// The target we have chosen
	CCSPlayerController* pTarget = nullptr;
	// Cache'd position
	Vector_t vecBestPosition = Vector_t();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-0.15f, 0.15f);
	float randomValue = dis(gen);
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
		Vector_t velocity = SDK::LocalPawn->GetVecVelocity();
		
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
	float hit_chnce = 200 - 2 * C_GET(float, Vars.fHitChance);
	if (speed > 0 && C_GET(bool, Vars.bAutoStop)) {
		AutoStop(pUserCmd);
	}
	QAngle_t* pViewAngles = &(pUserCmd->pViewAngles->angValue); // Just for readability sake!

	// Find the change in angles
	QAngle_t vNewAngles = GetAngularDifference(pUserCmd, vecBestPosition, pLocalPawn);
	vNewAngles.x = vNewAngles.x - 0.2f;

	if (abs(static_cast<float>(vNewAngles.x)) < 0.35f && abs(static_cast<float>(vNewAngles.y)) < 0.28f) {
		if (speed < hit_chnce) {
			if (C_GET(bool, Vars.bAutoFire)) {
				ActionFire();
				myTimer.Trigger();
			}
		}
	}

	if (abs(static_cast<float>(vNewAngles.x)) < 0.8f && abs(static_cast<float>(vNewAngles.y)) < 0.8f && C_GET(bool, Vars.bAutoFire)) {
		flSmoothing = 1.0f;
	}
	else {
		// Get the smoothing
		flSmoothing = C_GET(float, Vars.flSmoothing);
		randomValue = 0;

	}

	auto aimPunch = GetRecoil(pLocalPawn); //get AimPunch angles

	if (C_GET(bool, Vars.bHumanize) && C_GET(bool, Vars.bAutoFire) && flSmoothing > 1.1f && (abs(static_cast<float>(vNewAngles.x)) > 1.5f && abs(static_cast<float>(vNewAngles.y)) > 1.5f )) {
		double G_0 = 8;
		double W_0 = 30;
		double M_0 = 15;
		double D_0 = 1;
		TwoFloats result_aim = wind_mouse(0, 0, static_cast<float>(((vNewAngles.x ) + randomValue))*10, 
			static_cast<float>(((vNewAngles.y) + randomValue))*10,
			G_0, W_0, M_0, D_0);

		pViewAngles->x += result_aim.first/ flSmoothing - aimPunch.x; // minus AimPunch angle to counteract recoil
		pViewAngles->y += result_aim.second / flSmoothing - aimPunch.y;
	}
	else {
		// Apply smoothing and set angles
		pViewAngles->x += (vNewAngles.x - aimPunch.x) / flSmoothing + randomValue;// minus AimPunch angle to counteract recoil
		pViewAngles->y += (vNewAngles.y - aimPunch.y) / flSmoothing + randomValue;
	}
	pViewAngles->Normalize();
	//L_PRINT(LOG_INFO) << MENU::bMainWindowOpened;

}

