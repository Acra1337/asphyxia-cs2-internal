#include "penetration.h"

void AutoWall::ScaleDamage(Data_t& mData) {
	if (!mData.pTargetPawn)
		return;

	CPlayer_ItemServices* ItemServices = mData.pTargetPawn->GetItemServices();
	if (!ItemServices)
		return;

	auto flDamageScaleCTHead = CONVAR::mp_damage_scale_ct_head->value.fl;
	auto flDamageScaleCTBody = CONVAR::mp_damage_scale_ct_body->value.fl;
	auto flDamageScaleTTHead = CONVAR::mp_damage_scale_t_head->value.fl;
	auto flDamageScaleTTBody = CONVAR::mp_damage_scale_t_body->value.fl;

	bool bIsCT = mData.pTargetPawn->GetTeam() == TEAM_CT;
	bool bIsTT = mData.pTargetPawn->GetTeam() == TEAM_TT;

	float flHeadDamageScale = bIsCT ? flDamageScaleCTHead : bIsTT ? flDamageScaleTTHead : 1.0f;
	float flBodyDamageScale = bIsCT ? flDamageScaleCTBody : bIsTT ? flDamageScaleTTBody : 1.0f;

	if (ItemServices->HasHeavyArmor())
		flHeadDamageScale *= 0.5f;
	switch (mData.iHitGroup) {
	case HEAD:
		mData.flDamage *= SDK::WeaponBaseVData->GetHeadshotMultiplier() * flHeadDamageScale;
		break;
	case CHEST:
	case L_ARM:
	case R_ARM:
	case NECK:
		mData.flDamage *= flBodyDamageScale;
		break;
	case STOMACH:
		mData.flDamage *= 1.25f * flBodyDamageScale;
		break;
	case L_LEG:
	case R_LEG:
		mData.flDamage *= 0.75f * flBodyDamageScale;
		break;
	default:
		break;
	}
	if (!mData.pTargetPawn->HasArmour(mData.iHitGroup))
		return;

	float flHeavyArmorBonus = 1.0f, flArmorBonus = 0.5f, flArmorRatio = SDK::WeaponBaseVData->GetArmorRatio() * 0.5f;

	if (ItemServices->HasHeavyArmor()) {
		flHeavyArmorBonus = 0.25f;
		flArmorBonus = 0.33f;
		flArmorRatio *= 0.20f;
	}

	float flDamageToHealth = mData.flDamage * flArmorRatio;
	float flDamageToArmor = (mData.flDamage - flDamageToHealth) * flHeavyArmorBonus * flArmorBonus;

	if (flDamageToArmor > static_cast<float>(mData.pTargetPawn->GetArmorValue()))
		flDamageToHealth = mData.flDamage - static_cast<float>(mData.pTargetPawn->GetArmorValue()) / flArmorBonus;

	mData.flDamage = flDamageToHealth;
}

bool AutoWall::FireBullet(Data_t& mData) {
	if (!SDK::LocalPawn || !mData.pTargetPawn || !SDK::WeaponBaseVData)
		return false;

	const Vector_t vecDirection = mData.vecEndPos - mData.vecStartPos;
	const Vector_t vecEndPosition = vecDirection * SDK::WeaponBaseVData->GetRange();

	TraceData_t pTraceData = TraceData_t();
	pTraceData.pArrayPointer = &pTraceData.arrTraceElemets;

	TraceFilter_t pTraceFilter;
	I::GameTraceManager->TraceInit(pTraceFilter, SDK::LocalPawn, 0x1C300Bu, 3, 7);
	I::GameTraceManager->CreateTrace(&pTraceData, mData.vecStartPos, vecEndPosition, pTraceFilter, 4);

	HandleBulletPenetrationData_t mHandleBulletData = HandleBulletPenetrationData_t(
		static_cast<float>(SDK::WeaponBaseVData->GetDamage()),
		SDK::WeaponBaseVData->GetPenetration(),
		SDK::WeaponBaseVData->GetRange(),
		SDK::WeaponBaseVData->GetRangeModifier(),
		1,//C_GET(int, Vars.iPenetrationCount),
		false);

	mData.flDamage = static_cast<float>(SDK::WeaponBaseVData->GetDamage());
	float flTraceLength = 0.f;
	auto flMaxRange = SDK::WeaponBaseVData->GetRange();

	for (int i = 0; i < pTraceData.uNumUpdate; i++)
	{
		UpdateValue_t* pUpdateValue = reinterpret_cast<UpdateValue_t* const>(
			reinterpret_cast<std::uintptr_t>(pTraceData.pUpdateValue) + i * sizeof(UpdateValue_t));

		GameTrace_t pGameTrace = GameTrace_t{};
		I::GameTraceManager->InitializeTraceInfo(&pGameTrace);
		I::GameTraceManager->GetTraceInfo(
			&pTraceData, &pGameTrace, 0.0f,
			reinterpret_cast<void*>(
				reinterpret_cast<std::uintptr_t>(pTraceData.arrTraceElemets.data()) + sizeof(TraceArrElement_t) * (pUpdateValue->nHandleIndex & ENT_ENTRY_MASK)));

		flMaxRange -= flTraceLength;

		if (pGameTrace.flFraction == 1.0f)
			break;
		flTraceLength += pGameTrace.flFraction * flMaxRange;
		//mData.flDamage *= std::powf(SDK::WeaponBaseVData->GetRangeModifier(), flTraceLength / 500.f);	/need fix

		/*if (flTraceLength > 3000.f)	/need fix
			break;*/
		if (pGameTrace.m_pHitEntity) {
			if (pGameTrace.m_pHitEntity->GetRefEHandle().GetEntryIndex() == mData.pTargetPawn->GetRefEHandle().GetEntryIndex()) {
				ScaleDamage(mData);
				return true;
			}
		}

		if (I::GameTraceManager->HandleBulletPenetration(&pTraceData, &mHandleBulletData, pUpdateValue, true))
			return false;

		mData.flDamage = mHandleBulletData.flDamage;
	}

	return false;
}