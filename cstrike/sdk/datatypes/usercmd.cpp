#include "usercmd.h"



CSubtickMoveStep* CBaseUserCmdPB::CreateSubtickStep() {
	using CreateSubtickStep_t = CSubtickMoveStep * (CS_FASTCALL*)(void*);
	static CreateSubtickStep_t fnCreateSubtickStep = reinterpret_cast<CreateSubtickStep_t>(MEM::FindPattern(CLIENT_DLL, CS_XOR("40 53 48 83 EC ? 48 8B D9 48 85 C9 75 ? 8D 4B ? E8 ? ? ? ? 48 85 C0 74 ? 33 D2 45 33 C0 48 8B C8 E8 ? ? ? ? 48 83 C4 ? 5B C3 48 83 C4 ? 5B C3 4C 8D 05 ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 48 8B D3 45 33 C0 48 8B C8 E8 ? ? ? ? 48 83 C4 ? 5B C3 CC CC CC CC CC CC CC 48 89 5C 24 ? 57 48 83 EC ? 33 FF 48 8B D9 48 85 C9 75 ? 8D 4B ? E8 ? ? ? ? 48 85 C0 74 ? 48 89 78 ? EB ? 48 8B C7 48 8B 5C 24 ? 48 83 C4 ? 5F C3 4C 8D 05 ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 48 89 58 ? 48 8B 5C 24 ? 48 8D 0D ? ? ? ? 48 89 08 33 C9 48 89 48 ? 48 89 78 ? 48 83 C4 ? 5F C3 CC CC CC CC CC CC CC 48 89 5C 24 ? 56")));

	return fnCreateSubtickStep(subtickMovesField.pArena);
}

CSubtickMoveStep* CBaseUserCmdPB::CreateSubtick() {
	if (subtickMovesField.pRep && (subtickMovesField.nCurrentSize < subtickMovesField.pRep->nAllocatedSize))
		return subtickMovesField.pRep->tElements[subtickMovesField.nCurrentSize++];

	CSubtickMoveStep* subtick = CreateSubtickStep();
	subtickMovesField.add(subtick);

	return subtick;
}