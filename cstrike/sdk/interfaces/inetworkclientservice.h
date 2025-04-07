#pragma once
#include "../../utilities/memory.h"

// Define Flow_t before using it
enum Flow_t {
	FLOW_OUTGOING,
	FLOW_INCOMING,
	FLOW_MAX
};

class CNetChannelInfo {
public:
	float GetLatency(Flow_t flow) {
		return MEM::CallVFunc<float, 10U>(this, flow);
	}
};

class CNetworkGameClient {
public:
	MEM_PAD(0xE8); //0x0000
	CNetChannelInfo* NetChannelInfo; // 0x00E8
	MEM_PAD(0x8); // 0x00F0
	bool bShouldPredict; // 0x00F8
	MEM_PAD(0x7B); // 0x00F9
	int iSomePredictionTick; // 0x0174
	MEM_PAD(0x104); // 0x0178
	int iDeltaTick; // 0x027C

	void ClientPredict() {
		using ClientPredict_t = void(__fastcall*)(CNetworkGameClient*, unsigned int);
		static ClientPredict_t fnClientPredict = reinterpret_cast<ClientPredict_t>(MEM::FindPattern(PARTICLES_DLL, CS_XOR("40 55 41 56 48 83 EC 28 80 B9")));

		return fnClientPredict(this, 1);
	}

	void SetPrediction(bool value) {
		*(bool*)(std::uintptr_t(this) + 0xDC) = value;
	}

	float GetClientInterp() {
		return MEM::CallVFunc<float, 61u>(this);
	}

	int GetClientTick() {
		return MEM::CallVFunc<int, 5U>(this);
	}

	int GetServerTick() {
		return MEM::CallVFunc<int, 6U>(this);
	}

	bool IsConnected() {
		return MEM::CallVFunc<bool, 12U>(this);
	}

	// force game to clear cache and reset delta tick
	void FullUpdate() {
		// @ida: #STR: "Requesting full game update (%s)...\n"
		MEM::CallVFunc<void, 28U>(this, CS_XOR("unk"));
	}

	int GetDeltaTick() {
		// @ida: offset in FullUpdate();
		// (nDeltaTick = -1) == FullUpdate() called
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + 0x25C);
	}

	const char* GetMapName() {
		return *reinterpret_cast<const char**>(reinterpret_cast<std::uintptr_t>(this) + 0x240);
	}
};

class INetworkClientService
{
public:
	[[nodiscard]] CNetworkGameClient* GetNetworkGameClient()
	{
		return MEM::CallVFunc<CNetworkGameClient*, 23U>(this);
	}
};
