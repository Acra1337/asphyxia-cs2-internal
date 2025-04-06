#include "aim.h"

// used: sdk entity
#include "../../sdk/entity.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/interfaces/ccsgoinput.h"
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
#include <cmath>

double sqrt3, sqrt5;

void init_constants() {
	sqrt3 = sqrt(3);
	sqrt5 = sqrt(5);
}

struct TwoFloats {
	double first;
	double second;
};

Vector_t Cross(const Vector_t& a, const Vector_t& b) {
	Vector_t result;
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return result;
}

float Length(const Vector_t& v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
// Функция для вычисления расстояния между двумя точками
float GetDistance(const Vector_t& pos1, const Vector_t& pos2) {
	Vector_t delta = static_cast<Vector_t>(pos1) - static_cast<Vector_t>(pos2);
	return Length(delta);
}


TwoFloats wind_mouse(int start_x, int start_y, float dest_x, float dest_y, double G_0, double W_0, double M_0, double D_0) {
	int current_x = start_x;
	int current_y = start_y;
	double v_x = 0, v_y = 0, W_x = 0, W_y = 0;

	while (1) {
		double dist = sqrt(pow(dest_x - start_x, 2) + pow(dest_y - start_y, 2))*3;
		/*if (dist < 0.01) {
			return { dest_x , dest_y };
			break;
		}*/

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

		TwoFloats result = { v_x / 5, v_y / 5 };
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

QAngle_t GetRecoil(C_CSPlayerPawn* pLocalPawn) {

	using GetAimPunch_t = float(__fastcall*)(void*, Vector_t*, float, bool);
	static GetAimPunch_t GetAimPunch = reinterpret_cast<GetAimPunch_t>(MEM::FindPattern(CLIENT_DLL, CS_XOR("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F2 0F 29 74 24 ? 48 8B D1")));

	Vector_t AimPunchAngle;
	GetAimPunch(pLocalPawn, &AimPunchAngle, 0.0f, true);

	return QAngle_t(AimPunchAngle.x, AimPunchAngle.y, AimPunchAngle.z);
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

struct Vector {
	float x, y, z;
};

// Определение структуры углов
struct QAngle {
	float pitch, yaw, roll;
};

void AngleVectors(const Vector_t& angles, Vector_t* forward)
{
	// sry
	//Assert(s_bMathlibInitialized);
	//Assert(forward);

	float sp, sy, cp, cy;

	sy = sin(M_DEG2RAD(angles[1]));
	cy = cos(M_DEG2RAD(angles[1]));

	sp = sin(M_DEG2RAD(angles[0]));
	cp = cos(M_DEG2RAD(angles[0]));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

unsigned int iTickCount = 0;

inline double DegToRad(double degrees) {
	return degrees * (3.14159265 / 180.0);
}
void AutoStop(CBaseUserCmdPB* pUserCmd, int type) {//0 early, 1 full
	if (SDK::UserCmd->nButtons.nValue & IN_DUCK)
		return;
	if (!(SDK::LocalPawn->GetFlags() & FL_ONGROUND))
		return;

	pUserCmd->SetBits(BASE_BITS_FORWARDMOVE);
	pUserCmd->SetBits(BASE_BITS_LEFTMOVE);

	float flYaw = SDK::LocalPawn->GetVecVelocity().ToAngles().y + 180.0f;
	float flRotation = M_DEG2RAD(SDK::pData->ViewAngle.y - flYaw);

	float flCosRotation = std::cos(flRotation);
	float flSinRotation = std::sin(flRotation);

	//float flNewForwardMove = flCosRotation * SDK::BaseCmd->flForwardMove - flSinRotation * SDK::BaseCmd->flSideMove;
	//float flNewSideMove = flSinRotation * SDK::BaseCmd->flForwardMove + flCosRotation * SDK::BaseCmd->flSideMove;
		
	pUserCmd->flForwardMove = flCosRotation;
	pUserCmd->flSideMove = -flSinRotation;


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



int CalculateAutisticHitchance() {

	float flInaccuarcy = 0;
	//float flSpread = 0;
	if (C_CSWeaponBase* pActiveWeapon = SDK::LocalPawn->GetActiveWeapon()) {
		pActiveWeapon->UpdateAccuracyPenality();
		flInaccuarcy = pActiveWeapon->GetInaccuracy();
		//flSpread = pActiveWeapon->GetSpread();

	}	

	double scale = (100 - 0) / (0.008 - 0.1);
	double autisticHs = 0 + (flInaccuarcy - 0.1) * scale;
	if (autisticHs < 0) autisticHs = 0;
	if (SDK::pData->WeaponType == WEAPONTYPE_SNIPER_RIFLE) 
		autisticHs -= 5;
	//L_PRINT(LOG_INFO) << "hitch_val: " << autisticHs;
	return autisticHs;
}

void ActionFire() {

	/*SDK::UserCmd->nButtons.nValue |= IN_ATTACK;
	SDK::UserCmd->nButtons.nValueChanged |= IN_ATTACK;

	SDK::UserCmd->csgoUserCmd.nAttack1StartHistoryIndex = 0;
	SDK::UserCmd->csgoUserCmd.CheckAndSetBits(CSGOUSERCMD_BITS_ATTACK1START);*/
	TriggerMousePress();
}

//void AutoRevolver() {								//Нужно будет добавить если nButtons не вак детект и вернуть норм автофаер
//	static float revolverPrepareTime = 0.1f;
//	static float readyTime;
//
//	if (!SDK::pData->ItemDefinitionIndex == WEAPON_R8_REVOLVER)
//		return;
//
//	if (!readyTime)
//		readyTime = I::GlobalVars->flCurrentTime + revolverPrepareTime;
//
//	const auto ticksToReady = TIME_TO_TICKS(readyTime - I::GlobalVars->flCurrentTime - 0.5/*- interfaces::Engine->GetNetChannelInfo(0)->GetLatency(FLOW_OUTGOING)*/);
//
//	if (ticksToReady > 0 && ticksToReady <= TIME_TO_TICKS(revolverPrepareTime))
//		SDK::UserCmd->nButtons.nValue |= IN_ATTACK;
//	else
//		readyTime = 0.0f;
//}

Timer myTimer;

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
	std::uniform_real_distribution<float> dis(-1.3f, 1.3f);
	float randomValue = dis(gen);
	// Entity loop
	const int iHighestIndex = 32;//126
	UpdateMouseRelease();
	C_CSPlayerPawn* pPawn = nullptr;
	C_CSPlayerPawn* pTargetPawn = nullptr;
	float flBestDamage = -1;
	bool isPawned = false;
	bool isPenitration = false;
	bool isBaim = false;
	std::vector<std::uint32_t> cHitboxes ={ HEAD,STOMACH, CENTER }; //{ HEAD, STOMACH, CENTER };
	int final_bone = 0;

	if (SDK::pData->WeaponType == WEAPONTYPE_KNIFE)
		return;
	if (SDK::pData->WeaponType == WEAPONTYPE_C4)
		return;
	if (SDK::pData->WeaponType == WEAPONTYPE_GRENADE)
		return;
	if (SDK::pData->WeaponType == WEAPONTYPE_TASER)
		return;
	if (!SDK::pData->CanShoot && C_GET(bool, Vars.bAutoFire))
		return;
	if (SDK::pData->CanScope)
		return;

	for (int nIndex = 1; nIndex <= iHighestIndex; nIndex++)
	{
		/*if (myTimer.IsBlocked(delay))
			continue;*/

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
		//if (pTarget && flCurrentDistance > flDistance) // Override if this is the first move or if it is a better move
		//	continue;
		//Vector_t velocity = SDK::LocalPawn->GetVecVelocity();
		float distance_vec = GetDistance(pLocalPawn->GetEyePosition(), vecBestPosition);
		float VarminDamage = C_GET(float, Vars.flMinDamage);
		if (C_GET(bool, Vars.bAutoWall)) {
			const float flHealthFactor = pPawn->GetHealth();
			for (auto& iBone : cHitboxes) {
				AutoWall::mData.iHitGroup = iBone;
				AutoWall::mData.pTargetPawn = pPawn;
				AutoWall::mData.vecStartPos = pLocalPawn->GetEyePosition();
				Vector_t targetVec = pPawn->GetGameSceneNode()->GetSkeletonInstance()->pBoneCache->GetOrigin(iBone);
				if (iBone == 6) {
					Vector_t forward = (targetVec - AutoWall::mData.vecStartPos).Normalized();

					Vector_t worldUp(0, 0, 1);

					Vector_t left = Cross(worldUp, forward);

					float offset = randomValue;

					float correctedOffset = 20 * (60.f / distance_vec);
					targetVec.z += correctedOffset;
					
					targetVec += left * offset;

				}
				AutoWall::mData.vecEndPos = targetVec;

				if (!AutoWall::FireBullet(AutoWall::mData))
					continue;

				int currentBone = iBone;
				float currentDamage = static_cast<float>(AutoWall::mData.flDamage) - 2;

				//L_PRINT(LOG_INFO) << "bone: " << currentBone << " dmg: " << currentDamage << " hp: " << flHealthFactor;
				bool isTargetEntity = (trace.m_pHitEntity == pPawn);
				bool isDamageSufficient = (currentDamage >= VarminDamage || currentDamage >= flHealthFactor && currentDamage > flBestDamage);

				if (!(isTargetEntity || isDamageSufficient))
					continue;

				if (isDamageSufficient) {
					//vecPos = pBoneCache->GetOrigin(currentBone);
					flBestDamage = currentDamage;
					pTarget = pPlayer;
					flDistance = flCurrentDistance;
					vecBestPosition = targetVec;
					final_bone = iBone;

					pTargetPawn = pPawn;
					isPawned = true;
				}
				else if(!isDamageSufficient && currentDamage >= flBestDamage){
					vecBestPosition = targetVec;
				}
			}
		}
		else {
			vecBestPosition = pPawn->GetGameSceneNode()->GetSkeletonInstance()->pBoneCache->GetOrigin(3);
			Damage = 0;
		}

		
		if (trace.m_pHitEntity == pPawn) {
			isPenitration = false;
		}
		else {
			isPenitration = true;
		}
		

	}
	// Check if a target was found
	if (pTarget == nullptr)
		return;


	/*else if (Damage >= VarminDamage/2) {
		if (C_GET(bool, Vars.bAutoStop)) {
			AutoStop(pUserCmd, 0);
		}
	}*/
	Vector_t velocity = SDK::LocalPawn->GetVecVelocity();

	
	/*if (speed > 180)
		return;*/
	// Point at them
	float VarminDamage = C_GET(float, Vars.flMinDamage);
	float hit_chnce = C_GET(float, Vars.fHitChance);

	if (C_GET(bool, Vars.bAutoStop)) {
		AutoStop(pUserCmd, 1);
	}

	QAngle_t* pViewAngles = &(pUserCmd->pViewAngles->angValue); // Just for readability sake!

	// Find the change in angles
	QAngle_t vNewAngles = GetAngularDifference(pUserCmd, vecBestPosition, pLocalPawn);
	float distance_vec = GetDistance(pLocalPawn->GetEyePosition(), vecBestPosition);
	//L_PRINT(LOG_INFO) << "dist: " << vNewAngles;

	/*if (C_GET(bool, Vars.bAutoFire) && final_bone == 6 || C_GET(float, Vars.flSmoothing)<3) {
		vNewAngles.x = vNewAngles.x - 2.f* (60.f / distance_vec);
	}
	else {
		if (SDK::pData->WeaponType != WEAPONTYPE_SNIPER_RIFLE && SDK::pData->WeaponType != WEAPONTYPE_SHOTGUN) {
			vNewAngles.x = vNewAngles.x + 2.f * (60.f / distance_vec);
		}
		else {
			vNewAngles.x = vNewAngles.x + Vars.flSmoothing / 20;
		}
	}*/
	
	//L_PRINT(LOG_INFO) << "hc: " << ÑalculateHitÑhance(vNewAngles, pPawn, pLocalPawn);
	CCSPlayer_WeaponServices* WeaponServices = SDK::LocalPawn->GetWeaponServices();
	if (WeaponServices == nullptr)
		return;

	/*if (ÑalculateHitÑhance(vNewAngles, pPawn, pLocalPawn) < C_GET(float, Vars.fHitChance))
		return;*/

	float hitch_val = CalculateAutisticHitchance();
	
	

	if (abs(static_cast<float>(vNewAngles.x)) < 0.8f && abs(static_cast<float>(vNewAngles.y)) < 0.8f && C_GET(bool, Vars.bAutoFire) && hitch_val-10 >= hit_chnce) {
		flSmoothing = 1.0f;
	}
	else if (C_GET(bool, Vars.bAutoWallFast) && isPenitration && flSmoothing>1.6f) {
		flSmoothing = 1.6f;
	}
	else{
		flSmoothing = C_GET(float, Vars.flSmoothing);
		randomValue = 0;

	}

	pUserCmd->pViewAngles->SetBits(EBaseCmdBits::BASE_BITS_VIEWANGLES);

	auto aimPunch = GetRecoil(pLocalPawn); //get AimPunch angles
	if (C_GET(bool, Vars.bHumanize) && C_GET(bool, Vars.bAutoFire) && flSmoothing > 1.1f && (abs(static_cast<float>(vNewAngles.x)) > 2.0f && abs(static_cast<float>(vNewAngles.y)) > 2.0f )) {
		double G_0 = 8;
		double W_0 = 20;
		double M_0 = 10;
		double D_0 = 2;
		TwoFloats result_aim = wind_mouse(0, 0, static_cast<float>((vNewAngles.x )*10 ), 
			static_cast<float>((vNewAngles.y)*10),
			G_0, W_0, M_0, D_0);

		pViewAngles->x += result_aim.first/ flSmoothing - aimPunch.x; // minus AimPunch angle to counteract recoil
		pViewAngles->y += result_aim.second / flSmoothing - aimPunch.y;
	}
	else {
		// Apply smoothing and set angles
		pViewAngles->x += (vNewAngles.x - aimPunch.x) / flSmoothing;// minus AimPunch angle to counteract recoil
		pViewAngles->y += (vNewAngles.y - aimPunch.y) / flSmoothing;
	}
	pViewAngles->Normalize();


	pViewAngles = &(pUserCmd->pViewAngles->angValue); // Just for readability sake!
	vNewAngles = GetAngularDifference(pUserCmd, vecBestPosition, pLocalPawn);
	//L_PRINT(LOG_INFO) << "aimPunch.y" << aimPunch.y;
	if (abs(static_cast<float>(vNewAngles.x)) < 0.35f && abs(static_cast<float>(vNewAngles.y)) < 0.28f) {
		if (hitch_val >= hit_chnce) {
			if (C_GET(bool, Vars.bAutoFire)) {
				ActionFire();
				myTimer.Trigger();
				//L_PRINT(LOG_INFO) << "hitch_val: " << hitch_val;
			}
		}
	}

}

