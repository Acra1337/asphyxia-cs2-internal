#pragma once
#include "../../sdk/entity.h"
#include "../../core/sdk.h"

class CL_World
{
public:

	void skybox();

	void skybox(C_EnvSky* env_sky);
	void lighting(C_SceneLightObject* light_object);
	void exposure(C_CSPlayerPawn* pawn);
	void exposure(C_PostProcessingVolume* post_processing);
	void DrawScopeOverlay();
};

inline std::unique_ptr<CL_World> g_world = std::make_unique<CL_World>();

