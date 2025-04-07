#pragma once

// @source: master/game/shared/cstrike15/gametypes.h
#pragma region valve_gametypes

enum EGameType : int
{
	GAMETYPE_UNKNOWN = -1,
	GAMETYPE_CLASSIC,
	GAMETYPE_GUNGAME,
	GAMETYPE_TRAINING,
	GAMETYPE_CUSTOM,
	GAMETYPE_COOPERATIVE,
	GAMETYPE_SKIRMISH,
	GAMETYPE_FREEFORALL
};
enum EBoneFlags : uint32_t
{
	FLAG_NO_BONE_FLAGS = 0x0,
	FLAG_BONEFLEXDRIVER = 0x4,
	FLAG_CLOTH = 0x8,
	FLAG_PHYSICS = 0x10,
	FLAG_ATTACHMENT = 0x20,
	FLAG_ANIMATION = 0x40,
	FLAG_MESH = 0x80,
	FLAG_HITBOX = 0x100,
	FLAG_BONE_USED_BY_VERTEX_LOD0 = 0x400,
	FLAG_BONE_USED_BY_VERTEX_LOD1 = 0x800,
	FLAG_BONE_USED_BY_VERTEX_LOD2 = 0x1000,
	FLAG_BONE_USED_BY_VERTEX_LOD3 = 0x2000,
	FLAG_BONE_USED_BY_VERTEX_LOD4 = 0x4000,
	FLAG_BONE_USED_BY_VERTEX_LOD5 = 0x8000,
	FLAG_BONE_USED_BY_VERTEX_LOD6 = 0x10000,
	FLAG_BONE_USED_BY_VERTEX_LOD7 = 0x20000,
	FLAG_BONE_MERGE_READ = 0x40000,
	FLAG_BONE_MERGE_WRITE = 0x80000,
	FLAG_ALL_BONE_FLAGS = 0xfffff,
	BLEND_PREALIGNED = 0x100000,
	FLAG_RIGIDLENGTH = 0x200000,
	FLAG_PROCEDURAL = 0x400000,
};

enum EGameMode : int
{
	GAMEMODE_UNKNOWN = -1,

	// GAMETYPE_CLASSIC
	GAMEMODE_CLASSIC_CASUAL = 0,
	GAMEMODE_CLASSIC_COMPETITIVE,
	GAMEMODE_CLASSIC_SCRIM_COMPETITIVE2V2,
	GAMEMODE_CLASSIC_SCRIM_COMPETITIVE5V5,

	// GAMETYPE_GUNGAME
	GAMEMODE_GUNGAME_PROGRESSIVE = 0,
	GAMEMODE_GUNGAME_BOMB,
	GAMEMODE_GUNGAME_DEATHMATCH,

	// GAMETYPE_TRAINING
	GAMEMODE_TRAINING_DEFAULT = 0,

	// GAMETYPE_CUSTOM
	GAMEMODE_CUSTOM_DEFAULT = 0,

	// GAMETYPE_COOPERATIVE
	GAMEMODE_COOPERATIVE_DEFAULT = 0,
	GAMEMODE_COOPERATIVE_MISSION,

	// GAMETYPE_SKIRMISH
	GAMEMODE_SKIRMISH_DEFAULT = 0,

	// GAMETYPE_FREEFORALL
	GAMEMODE_FREEFORALL_SURVIVAL = 0
};

#pragma endregion

enum ELifeState : int
{
	LIFE_ALIVE = 0,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY
};

enum EFlags : int
{
	FL_ONGROUND = (1 << 0), // entity is at rest / on the ground
	FL_DUCKING = (1 << 1), // player is fully crouched/uncrouched
	FL_ANIMDUCKING = (1 << 2), // player is in the process of crouching or uncrouching but could be in transition
	FL_WATERJUMP = (1 << 3), // player is jumping out of water
	FL_ONTRAIN = (1 << 4), // player is controlling a train, so movement commands should be ignored on client during prediction
	FL_INRAIN = (1 << 5), // entity is standing in rain
	FL_FROZEN = (1 << 6), // player is frozen for 3rd-person camera
	FL_ATCONTROLS = (1 << 7), // player can't move, but keeps key inputs for controlling another entity
	FL_CLIENT = (1 << 8), // entity is a client (player)
	FL_FAKECLIENT = (1 << 9), // entity is a fake client, simulated server side; don't send network messages to them
	FL_INWATER = (1 << 10), // entity is in water
	FL_FLY = (1 << 11),
	FL_SWIM = (1 << 12),
	FL_CONVEYOR = (1 << 13),
	FL_NPC = (1 << 14),
	FL_GODMODE = (1 << 15),
	FL_NOTARGET = (1 << 16),
	FL_AIMTARGET = (1 << 17),
	FL_PARTIALGROUND = (1 << 18), // entity is standing on a place where not all corners are valid
	FL_STATICPROP = (1 << 19), // entity is a static property
	FL_GRAPHED = (1 << 20),
	FL_GRENADE = (1 << 21),
	FL_STEPMOVEMENT = (1 << 22),
	FL_DONTTOUCH = (1 << 23),
	FL_BASEVELOCITY = (1 << 24), // entity have applied base velocity this frame
	FL_WORLDBRUSH = (1 << 25), // entity is not moveable/removeable brush (part of the world, but represented as an entity for transparency or something)
	FL_OBJECT = (1 << 26),
	FL_KILLME = (1 << 27), // entity is marked for death and will be freed by the game
	FL_ONFIRE = (1 << 28),
	FL_DISSOLVING = (1 << 29),
	FL_TRANSRAGDOLL = (1 << 30), // entity is turning into client-side ragdoll
	FL_UNBLOCKABLE_BY_PLAYER = (1 << 31)
};


enum HitGroup_t : std::uint32_t {
	HITGROUP_INVALID = -1,
	HITGROUP_GENERIC = 0,
	HITGROUP_HEAD = 1,
	HITGROUP_CHEST = 2,
	HITGROUP_STOMACH = 3,
	HITGROUP_LEFTARM = 4,
	HITGROUP_RIGHTARM = 5,
	HITGROUP_LEFTLEG = 6,
	HITGROUP_RIGHTLEG = 7,
	HITGROUP_NECK = 8,
	HITGROUP_UNUSED = 9,
	HITGROUP_GEAR = 10,
	HITGROUP_SPECIAL = 11,
	HITGROUP_COUNT = 12,
};

//enum FlowDirection_t : int {
//	FLOW_OUTGOING = 0,
//	FLOW_INCOMING = 1,
//};


enum PredictionReason_t
{
	ClientCommandTick,
	DemoPreentity,
	DemoSimulation,
	Postnetupdate,
	ServerStarvedAndAddedUsercmds,
	ClientFrameSimulate
};

enum HITBOXES : uint32_t
{
	HEAD = 6,
	NECK = 5,
	CHEST = 4,
	RIGHT_CHEST = 8,
	LEFT_CHEST = 13,
	STOMACH = 3,
	PELVIS = 2,
	CENTER = 1,
	L_LEG = 23,
	L_FEET = 24,
	R_LEG = 26,
	R_FEET = 27,
	R_ARM = 13,
	R_FOREARM = 14,
	L_ARM = 8,
	L_FOREARM = 9
};


enum EEFlags : int
{
	EFL_KILLME = (1 << 0),
	EFL_DORMANT = (1 << 1),
	EFL_NOCLIP_ACTIVE = (1 << 2),
	EFL_SETTING_UP_BONES = (1 << 3),
	EFL_KEEP_ON_RECREATE_ENTITIES = (1 << 4),
	EFL_DIRTY_SHADOWUPDATE = (1 << 5),
	EFL_NOTIFY = (1 << 6),
	EFL_FORCE_CHECK_TRANSMIT = (1 << 7),
	EFL_BOT_FROZEN = (1 << 8),
	EFL_SERVER_ONLY = (1 << 9),
	EFL_NO_AUTO_EDICT_ATTACH = (1 << 10),
	EFL_DIRTY_ABSTRANSFORM = (1 << 11),
	EFL_DIRTY_ABSVELOCITY = (1 << 12),
	EFL_DIRTY_ABSANGVELOCITY = (1 << 13),
	EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS = (1 << 14),
	EFL_DIRTY_SPATIAL_PARTITION = (1 << 15),
	EFL_HAS_PLAYER_CHILD = (1 << 16),
	EFL_IN_SKYBOX = (1 << 17),
	EFL_USE_PARTITION_WHEN_NOT_SOLID = (1 << 18),
	EFL_TOUCHING_FLUID = (1 << 19),
	EFL_IS_BEING_LIFTED_BY_BARNACLE = (1 << 20),
	EFL_NO_ROTORWASH_PUSH = (1 << 21),
	EFL_NO_THINK_FUNCTION = (1 << 22),
	EFL_NO_GAME_PHYSICS_SIMULATION = (1 << 23),
	EFL_CHECK_UNTOUCH = (1 << 24),
	EFL_DONTBLOCKLOS = (1 << 25),
	EFL_DONTWALKON = (1 << 26),
	EFL_NO_DISSOLVE = (1 << 27),
	EFL_NO_MEGAPHYSCANNON_RAGDOLL = (1 << 28),
	EFL_NO_WATER_VELOCITY_CHANGE = (1 << 29),
	EFL_NO_PHYSCANNON_INTERACTION = (1 << 30),
	EFL_NO_DAMAGE_FORCES = (1 << 31)
};

enum EMoveType : std::uint8_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_OBSOLETE,
	MOVETYPE_WALK,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_OBSERVER,
	MOVETYPE_LADDER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST,
	MOVETYPE_INVALID,
	MOVETYPE_MAX_BITS = 5
};

// identifies how submerged in water a player is
enum : int
{
	WL_NOTINWATER = 0,
	WL_FEET,
	WL_WAIST,
	WL_EYES
};

enum ETeamID : int
{
	TEAM_UNK,
	TEAM_SPECTATOR,
	TEAM_TT,
	TEAM_CT
};

using ItemDefinitionIndex_t = std::uint16_t;
enum EItemDefinitionIndexes : ItemDefinitionIndex_t
{
	WEAPON_NONE,
	WEAPON_DESERT_EAGLE,
	WEAPON_DUAL_BERETTAS,
	WEAPON_FIVE_SEVEN,
	WEAPON_GLOCK_18,
	WEAPON_AK_47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALIL_AR = 13,
	WEAPON_M249,
	WEAPON_M4A4 = 16,
	WEAPON_MAC_10,
	WEAPON_P90 = 19,
	WEAPON_REPULSOR_DEVICE,
	WEAPON_MP5_SD = 23,
	WEAPON_UMP_45,
	WEAPON_XM1014,
	WEAPON_PP_BIZON,
	WEAPON_MAG_7,
	WEAPON_NEGEV,
	WEAPON_SAWED_OFF,
	WEAPON_TEC_9,
	WEAPON_ZEUS_X27,
	WEAPON_P2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_RIOT_SHIELD,
	WEAPON_SCAR_20,
	WEAPON_SG_553,
	WEAPON_SSG_08,
	WEAPON_KNIFE0,
	WEAPON_KNIFE1,
	WEAPON_FLASHBANG,
	WEAPON_HIGH_EXPLOSIVE_GRENADE,
	WEAPON_SMOKE_GRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY_GRENADE,
	WEAPON_INCENDIARY_GRENADE,
	WEAPON_C4_EXPLOSIVE,
	WEAPON_KEVLAR_VEST,
	WEAPON_KEVLAR_and_HELMET,
	WEAPON_HEAVY_ASSAULT_SUIT,
	WEAPON_NO_LOCALIZED_NAME0 = 54,
	WEAPON_DEFUSE_KIT,
	WEAPON_RESCUE_KIT,
	WEAPON_MEDI_SHOT,
	WEAPON_MUSIC_KIT,
	WEAPON_KNIFE2,
	WEAPON_M4A1_S,
	WEAPON_USP_S,
	WEAPON_TRADE_UP_CONTRACT,
	WEAPON_CZ75_AUTO,
	WEAPON_R8_REVOLVER,
	WEAPON_TACTICAL_AWARENESS_GRENADE = 68,
	WEAPON_BARE_HANDS,
	WEAPON_BREACH_CHARGE,
	WEAPON_TABLET = 72,
	WEAPON_KNIFE3 = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_WRENCH = 78,
	WEAPON_SPECTRAL_SHIV = 80,
	WEAPON_FIRE_BOMB,
	WEAPON_DIVERSION_DEVICE,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMP_MINE,
	WEAPON_BAYONET = 500,
	WEAPON_CLASSIC_KNIFE = 503,
	WEAPON_FLIP_KNIFE = 505,
	WEAPON_GUT_KNIFE,
	WEAPON_KARAMBIT,
	WEAPON_M9_BAYONET,
	WEAPON_HUNTSMAN_KNIFE,
	WEAPON_FALCHION_KNIFE = 512,
	WEAPON_BOWIE_KNIFE = 514,
	WEAPON_BUTTERFLY_KNIFE,
	WEAPON_SHADOW_DAGGERS,
	WEAPON_PARACORD_KNIFE,
	WEAPON_SURVIVAL_KNIFE,
	WEAPON_URSUS_KNIFE,
	WEAPON_NAVAJA_KNIFE,
	WEAPON_NOMAD_KNIFE,
	WEAPON_STILETTO_KNIFE,
	WEAPON_TALON_KNIFE,
	WEAPON_SKELETON_KNIFE = 525,
};

enum EWeaponType : std::uint32_t
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL = 1,
	WEAPONTYPE_SUBMACHINEGUN = 2,
	WEAPONTYPE_RIFLE = 3,
	WEAPONTYPE_SHOTGUN = 4,
	WEAPONTYPE_SNIPER_RIFLE = 5,
	WEAPONTYPE_MACHINEGUN = 6,
	WEAPONTYPE_C4 = 7,
	WEAPONTYPE_TASER = 8,
	WEAPONTYPE_GRENADE = 9,
	WEAPONTYPE_EQUIPMENT = 10,
	WEAPONTYPE_STACKABLEITEM = 11,
	WEAPONTYPE_FISTS = 12,
	WEAPONTYPE_BREACHCHARGE = 13,
	WEAPONTYPE_BUMPMINE = 14,
	WEAPONTYPE_TABLET = 15,
	WEAPONTYPE_MELEE = 16,
	WEAPONTYPE_SHIELD = 17,
	WEAPONTYPE_ZONE_REPULSOR = 18,
	WEAPONTYPE_UNKNOWN = 19
};

enum Contents_t
{
	CONTENTS_EMPTY = 0,
	CONTENTS_SOLID = 0x1,
	CONTENTS_WINDOW = 0x2,
	CONTENTS_AUX = 0x4,
	CONTENTS_GRATE = 0x8,
	CONTENTS_SLIME = 0x10,
	CONTENTS_WATER = 0x20,
	CONTENTS_BLOCKLOS = 0x40,
	CONTENTS_OPAQUE = 0x80,
	CONTENTS_TESTFOGVOLUME = 0x100,
	CONTENTS_UNUSED = 0x200,
	CONTENTS_BLOCKLIGHT = 0x400,
	CONTENTS_TEAM1 = 0x800,
	CONTENTS_TEAM2 = 0x1000,
	CONTENTS_IGNORE_NODRAW_OPAQUE = 0x2000,
	CONTENTS_MOVEABLE = 0x4000,
	CONTENTS_AREAPORTAL = 0x8000,
	CONTENTS_PLAYERCLIP = 0x10000,
	CONTENTS_MONSTERCLIP = 0x20000,
	CONTENTS_CURRENT_0 = 0x40000,
	CONTENTS_CURRENT_90 = 0x80000,
	CONTENTS_CURRENT_180 = 0x100000,
	CONTENTS_CURRENT_270 = 0x200000,
	CONTENTS_CURRENT_UP = 0x400000,
	CONTENTS_CURRENT_DOWN = 0x800000,
	CONTENTS_ORIGIN = 0x1000000,
	CONTENTS_MONSTER = 0x2000000,
	CONTENTS_DEBRIS = 0x4000000,
	CONTENTS_DETAIL = 0x8000000,
	CONTENTS_TRANSLUCENT = 0x10000000,
	CONTENTS_LADDER = 0x20000000,
	CONTENTS_HITBOX = 0x40000000,
};

enum Masks_t
{
	MASK_ALL = 0xFFFFFFFF,
	MASK_SOLID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
	MASK_PLAYERSOLID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
	MASK_NPCSOLID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
	MASK_NPCFLUID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
	MASK_WATER = CONTENTS_WATER | CONTENTS_MOVEABLE | CONTENTS_SLIME,
	MASK_OPAQUE = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_OPAQUE,
	MASK_OPAQUE_AND_NPCS = MASK_OPAQUE | CONTENTS_MONSTER,
	MASK_BLOCKLOS = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_BLOCKLOS,
	MASK_BLOCKLOS_AND_NPCS = MASK_BLOCKLOS | CONTENTS_MONSTER,
	MASK_VISIBLE = MASK_OPAQUE | CONTENTS_IGNORE_NODRAW_OPAQUE,
	MASK_VISIBLE_AND_NPCS = MASK_OPAQUE_AND_NPCS | CONTENTS_IGNORE_NODRAW_OPAQUE,
	MASK_SHOT = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE | CONTENTS_HITBOX,
	MASK_SHOT_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_DEBRIS,
	MASK_SHOT_HULL = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE,
	MASK_SHOT_PORTAL = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER,
	MASK_SOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_GRATE,
	MASK_PLAYERSOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_PLAYERCLIP | CONTENTS_GRATE,
	MASK_NPCSOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE,
	MASK_NPCWORLDSTATIC = CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE,
	MASK_NPCWORLDSTATIC_FLUID = CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP,
	MASK_SPLITAREPORTAL = CONTENTS_WATER | CONTENTS_SLIME,
	MASK_CURRENT = CONTENTS_CURRENT_0 | CONTENTS_CURRENT_90 | CONTENTS_CURRENT_180 | CONTENTS_CURRENT_270 | CONTENTS_CURRENT_UP | CONTENTS_CURRENT_DOWN,
	MASK_DEADSOLID = CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_GRATE,
};
