#pragma once

// used: viewmatrix_t
#include "../sdk/datatypes/matrix.h"
// used: color_t
#include "../sdk/datatypes/color.h"
// used: cmd
#include "../sdk/datatypes/usercmd.h"
#include "../sdk/entity.h"

#pragma region sdk_definitions
// @source: master/public/worldsize.h
// world coordinate bounds
#define MAX_COORD_FLOAT 16'384.f
#define MIN_COORD_FLOAT (-MAX_COORD_FLOAT)

// @source: master/public/vphysics_interface.h
// coordinates are in HL units. 1 unit == 1 inch
#define METERS_PER_INCH 0.0254f
#pragma endregion

class CCSPlayerController;
class C_CSPlayerPawn;
class CCSWeaponBaseVData;
class C_CSWeaponBase;

class CData {
public:
	float ServerTime;
	std::int32_t WeaponType;
	std::uint16_t ItemDefinitionIndex;
	QAngle_t ViewAngle;
	Vector_t EyePosition;

	bool IsAlive = false;
	bool CanShoot = false;
	bool CanScope = false;
	bool NoSpread = false;
	bool InThirdPerson = false;
};

namespace SDK
{
	// capture game's exported functions
	bool Setup();

	inline ViewMatrix_t ViewMatrix = ViewMatrix_t();
	inline Vector_t CameraPosition = Vector_t();
	inline CCSPlayerController* LocalController = nullptr;
	inline C_CSPlayerPawn* LocalPawn = nullptr;
	inline CUserCmd* UserCmd = nullptr;
	inline CBaseUserCmdPB* BaseCmd = nullptr;
	inline C_CSWeaponBase* WeaponBase = nullptr;
	inline CCSWeaponBaseVData* WeaponBaseVData = nullptr;
	inline bool isAlive = 0;
	inline CData* pData = new CData();

	inline void(CS_CDECL* fnConColorMsg)(const Color_t&, const char*, ...) = nullptr;
}
