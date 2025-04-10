#pragma once

// used: callvfunc
#include "../../utilities/memory.h"
#include "../datatypes/vector.h"


enum EClientFrameStage : int
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	// a network packet is being received
	FRAME_NET_UPDATE_START,
	// data has been received and we are going to start calling postdataupdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// data has been received and called postdataupdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// received all packets, we can now do interpolation, prediction, etc
	FRAME_NET_UPDATE_END,
	// start rendering the scene
	FRAME_RENDER_START,
	// finished rendering the scene
	FRAME_RENDER_END,
	FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};

class CNetworkedClientInfo {
public:
	MEM_PAD(0x4);
	int iRenderTick;
	float flRenderTickFraction;
	int iPlayerTickCount;
	float flPlayerTickFraction;
	MEM_PAD(0x4);

	struct {
		MEM_PAD(0xC);
		Vector_t vecEyePos;
	} *mLocalData;

	MEM_PAD(0x8);
};

class INetChannelInfo {
public:
	void SendNetMsg(void* message_handle, void* message_data, int unk) {
		// xref: SetSignonState
		MEM::CallVFunc<void, 68>(this, message_handle, message_data, unk);
	}

	float GetNetworkLatency() {
		// xref: StartLagcompensation
		return MEM::CallVFunc<float, 10>(this);
	}

	float GetEngineLatency() {
		// xref: StartLagcompensation
		return MEM::CallVFunc<float, 11>(this);
	}
};

class IEngineClient {
public:
	int GetMaxClients() {
		return MEM::CallVFunc<int, 34U>(this);
	}

	bool IsInGame() {
		return MEM::CallVFunc<bool, 35U>(this);
	}

	bool IsConnected() {
		return MEM::CallVFunc<bool, 36U>(this);
	}

	// return CBaseHandle index
	int GetLocalPlayer() {
		int nIndex = -1;
		MEM::CallVFunc<void, 49U>(this, std::ref(nIndex), 0);

		return nIndex + 1;
	}

	void SendCommandUnrestricted(const char* command) {
		return MEM::CallVFunc<void, 45U>(this, 0, command, 0x7FFEF001);
	}

	CNetworkedClientInfo* GetNetworkedClientInfo() {
		CNetworkedClientInfo pClientInfo;
		MEM::CallVFunc<void*, 178>(this, &pClientInfo);

		return &pClientInfo;
	}

	[[nodiscard]] const char* GetLevelName() {
		return MEM::CallVFunc<const char*, 56U>(this);
	}

	[[nodiscard]] const char* GetLevelNameShort() {
		return MEM::CallVFunc<const char*, 57U>(this);
	}

	[[nodiscard]] const char* GetProductVersionString() {
		return MEM::CallVFunc<const char*, 84U>(this);
	}

	[[nodiscard]] INetChannelInfo* GetNetChannelInfo(int nSplitScreenSlot = 0) {
		return MEM::CallVFunc<INetChannelInfo*, 37U>(this, nSplitScreenSlot);
	}
};
