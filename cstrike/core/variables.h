#pragma once

#include "config.h"

#pragma region variables_combo_entries
using VisualOverlayBox_t = int;

enum EVisualOverlayBox : VisualOverlayBox_t
{
	VISUAL_OVERLAY_BOX_NONE = 0,
	VISUAL_OVERLAY_BOX_FULL,
	VISUAL_OVERLAY_BOX_CORNERS,
	VISUAL_OVERLAY_BOX_MAX
};

using VisualChamMaterial_t = int;
enum EVisualsChamMaterials : VisualChamMaterial_t
{
	VISUAL_MATERIAL_PRIMARY_WHITE = 0,
	VISUAL_MATERIAL_ILLUMINATE,
	VISUAL_MATERIAL_MAX
};

using MiscDpiScale_t = int;

enum EMiscDpiScale : MiscDpiScale_t
{
	MISC_DPISCALE_DEFAULT = 0,
	MISC_DPISCALE_125,
	MISC_DPISCALE_150,
	MISC_DPISCALE_175,
	MISC_DPISCALE_200,
	MISC_DPISCALE_MAX
};

enum ESelectedRemovals : unsigned int
{
	VIEW_NONE = 0U,
	VIEW_FLASH = 1 << 0,
	VIEW_SMOKE = 1 << 1,
	VIEW_SCOPE_ZOOM = 1 << 2,
	VIEW_SCOPE_OVERLAY = 1 << 3,
	VIEW_REMOVALS_ALL = VIEW_FLASH | VIEW_SMOKE | VIEW_SCOPE_ZOOM | VIEW_SCOPE_OVERLAY,
	VIEW_REMOVALS = VIEW_NONE
};

#pragma endregion

#pragma region variables_multicombo_entries
using MenuAddition_t = unsigned int;
enum EMenuAddition : MenuAddition_t
{
	MENU_ADDITION_NONE = 0U,
	MENU_ADDITION_DIM_BACKGROUND = 1 << 0,
	MENU_ADDITION_BACKGROUND_PARTICLE = 1 << 1,
	MENU_ADDITION_GLOW = 1 << 2,
	MENU_ADDITION_ALL = MENU_ADDITION_DIM_BACKGROUND | MENU_ADDITION_BACKGROUND_PARTICLE | MENU_ADDITION_GLOW
};
#pragma endregion

struct Variables_t
{
#pragma region variables_visuals
	C_ADD_VARIABLE(bool, bVisualOverlay, true);

	C_ADD_VARIABLE(FrameOverlayVar_t, overlayBox, FrameOverlayVar_t(true));
	C_ADD_VARIABLE(TextOverlayVar_t, overlayName, TextOverlayVar_t(true));
	C_ADD_VARIABLE(BarOverlayVar_t, overlayHealthBar, BarOverlayVar_t(true, false, false, 1.f, Color_t(0, 255, 0), Color_t(255, 0, 0)));
	C_ADD_VARIABLE(BarOverlayVar_t, overlayArmorBar, BarOverlayVar_t(true, false, false, 1.f, Color_t(0, 255, 255), Color_t(255, 0, 0)));
	C_ADD_VARIABLE(TextOverlayVar_t, overlayWeapon, TextOverlayVar_t(true));

	C_ADD_VARIABLE(bool, bVisualChams, true);
	C_ADD_VARIABLE(int, nVisualChamMaterial, VISUAL_MATERIAL_ILLUMINATE);
	C_ADD_VARIABLE(bool, bVisualChamsIgnoreZ, true); // invisible chams
	C_ADD_VARIABLE(Color_t, colVisualChams, Color_t(185, 255, 83, 255));
	C_ADD_VARIABLE(Color_t, colVisualChamsIgnoreZ, Color_t(66, 0, 147, 96));

	C_ADD_VARIABLE(unsigned int, nViewRemovals, VIEW_REMOVALS_ALL);

	C_ADD_VARIABLE(bool, bThirdperson, false);
	C_ADD_VARIABLE(int, flThirdPersonDistance, 90);
	C_ADD_VARIABLE(KeyBind_t, nThirdPersonKey, 0);

	C_ADD_VARIABLE(Color_t, colSunClouds, Color_t(255, 255, 255))
	C_ADD_VARIABLE(Color_t, colClouds, Color_t(255, 255, 255))
	C_ADD_VARIABLE(bool, bWorldModulation, false);
	C_ADD_VARIABLE(Color_t, colWorld, Color_t(255, 255, 255));
	C_ADD_VARIABLE(Color_t, colSky, Color_t(255, 255, 255));
	C_ADD_VARIABLE(Color_t, colLightning, Color_t(255, 255, 255));
	C_ADD_VARIABLE(float, flLightingIntensity, 1.f);
	C_ADD_VARIABLE(float, flExposure, 90.f);

#pragma endregion

#pragma region variables_misc
	C_ADD_VARIABLE(bool, bAntiUntrusted, true);
	C_ADD_VARIABLE(bool, bWatermark, true);
	C_ADD_VARIABLE(bool, bHitSound, true);
	C_ADD_VARIABLE(float, flSoundVolume, 0.05f);

	C_ADD_VARIABLE(bool, bAutoBHop, false);
	//C_ADD_VARIABLE(int, nAutoBHopChance, 100);

	C_ADD_VARIABLE(bool, bAutoStrafe, false);
#pragma endregion

#pragma region variables_menu
	C_ADD_VARIABLE(unsigned int, nMenuKey, VK_INSERT);
	C_ADD_VARIABLE(unsigned int, nPanicKey, VK_END);
	C_ADD_VARIABLE(int, nDpiScale, 0);

	/*
	 * color navigation:
	 * [definition N][purpose]
	 * 1. primitive:
	 * - primtv 0 (text)
	 * - primtv 1 (background)
	 * - primtv 2 (disabled)
	 * - primtv 3 (control bg)
	 * - primtv 4 (border)
	 * - primtv 5 (hover)
	 *
	 * 2. accents:
	 * - accent 0 (main)
	 * - accent 1 (dark)
	 * - accent 2 (darker)
	 */
	C_ADD_VARIABLE(unsigned int, bMenuAdditional, MENU_ADDITION_ALL);
	C_ADD_VARIABLE(float, flAnimationSpeed, 1.f);


	C_ADD_VARIABLE(ColorPickerVar_t, colPrimtv0, ColorPickerVar_t(255, 255, 255)); // (text)
	C_ADD_VARIABLE(ColorPickerVar_t, colPrimtv1, ColorPickerVar_t(50, 55, 70)); // (background)
	C_ADD_VARIABLE(ColorPickerVar_t, colPrimtv2, ColorPickerVar_t(190, 190, 190)); // (disabled)
	C_ADD_VARIABLE(ColorPickerVar_t, colPrimtv3, ColorPickerVar_t(20, 20, 30)); // (control bg)
	C_ADD_VARIABLE(ColorPickerVar_t, colPrimtv4, ColorPickerVar_t(0, 0, 0)); // (border)

	C_ADD_VARIABLE(ColorPickerVar_t, colAccent0, ColorPickerVar_t(85, 90, 160)); // (main)
	C_ADD_VARIABLE(ColorPickerVar_t, colAccent1, ColorPickerVar_t(100, 105, 175)); // (dark)
	C_ADD_VARIABLE(ColorPickerVar_t, colAccent2, ColorPickerVar_t(115, 120, 190)); // (darker)
#pragma endregion
#pragma region variables_legitbot
	C_ADD_VARIABLE(bool, bLegitbot, false);
	C_ADD_VARIABLE(float, flAimRange, 10.0f);
	C_ADD_VARIABLE(float, flSmoothing, 2.5f);
	C_ADD_VARIABLE(bool, bLegitbotAlwaysOn, true);
	C_ADD_VARIABLE(unsigned int, nLegitbotActivationKey, VK_HOME);
	C_ADD_VARIABLE(bool, bHumanize, true);
	C_ADD_VARIABLE(bool, bAimLock, true);
	C_ADD_VARIABLE(bool, bAutoFire, true);
	C_ADD_VARIABLE(bool, bAutoStop, true);
	C_ADD_VARIABLE(bool, bAutoWall, true);
	C_ADD_VARIABLE(bool, bAutoWallFast, true);
	C_ADD_VARIABLE(float, flMinDamage, 105.0f);
	C_ADD_VARIABLE(float, fHitChance, 98.0f);

#pragma endregion
};

inline Variables_t Vars = {};
