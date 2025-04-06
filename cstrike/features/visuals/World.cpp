#include "../../sdk/datatypes/color.h"
#include "World.h"
#include "../../core/variables.h"
#include "../../sdk\interfaces\iengineclient.h"
#include "../../sdk\interfaces\igameresourceservice.h"
#include "../../sdk\interfaces\cgameentitysystem.h"



void CL_World::skybox(C_EnvSky* env_sky)
{

	Color_t skybox_clr = C_GET(Color_t, Vars.colSky); // g_cfg->world.m_sky;

	ByteColor color = (skybox_clr * 255).ToByte();

	env_sky->m_vTintColor() = color;

	env_sky->m_flBrightnessScale() =  1.f;
}


void CL_World::lighting(C_SceneLightObject* light_object)
{
	Color_t lighintg_color = C_GET(Color_t, Vars.colLightning); /*g_cfg->world.m_lighting;*/

	light_object->r = lighintg_color.r / 255.0f * C_GET(float, Vars.flLightingIntensity);
	light_object->g = lighintg_color.g / 255.0f * C_GET(float, Vars.flLightingIntensity);
	light_object->b = lighintg_color.b / 255.0f * C_GET(float, Vars.flLightingIntensity);
}



void CL_World::exposure(C_CSPlayerPawn* pawn)
{
	if (!I::Engine->IsInGame())
		return;

	static const auto update_exposure = reinterpret_cast<void(__fastcall*)(CPlayer_CameraServices*, int)>(MEM::FindPattern(CLIENT_DLL, CS_XOR("48 89 5C 24 08 57 48 83 EC 20 8B FA 48 8B D9 E8 ?? ?? ?? ?? 84 C0 0F 84")));

	static float prev_exposure = -1;

	float exposure = static_cast<float>(C_GET(float, Vars.flExposure) /*g_cfg->world.m_exposure*/);

	if (prev_exposure == exposure)
		return;

	if (!pawn || !pawn->GetCameraServices())
		return;

	CPlayer_CameraServices* camera_service = pawn->GetCameraServices();
	if (!camera_service)
		return;

	C_PostProcessingVolume* post_processing_volume = reinterpret_cast<C_PostProcessingVolume*>(I::GameResourceService->pGameEntitySystem->Get(camera_service->m_PostProcessingVolumes().GetEntryIndex()));
	if (!post_processing_volume)
		return;

	prev_exposure = exposure;

	this->exposure(post_processing_volume);
	update_exposure(camera_service, 0);
}

void CL_World::exposure(C_PostProcessingVolume* post_processing)
{
	float exposure = C_GET(float, Vars.flExposure) * 0.01f;

	if (!SDK::isAlive)
		return;

	if (!post_processing)
		return;
	try {
		post_processing->ExposureControl() = true;

		post_processing->FadeSpeedDown() = post_processing->FadeSpeedUp() = 0;

		post_processing->MinExposure() = post_processing->MaxExposure() = exposure;
	}
	catch (...)
	{
		L_PRINT(LOG_INFO) << "ExposureControl";
	}
}


