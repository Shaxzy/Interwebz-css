#include "DllMain.h"

//optimizing code so we call setupbones only once
Vector GetBoneFromMatrix(matrix3x4_t *MatrixArray, int iBone)
{
	return Vector(MatrixArray[iBone][0][3], MatrixArray[iBone][1][3], MatrixArray[iBone][2][3]);
}

void GetMaterialParameters(char sMaterial, float &fPenetrationPowerModifier, float &fDamageModifier)
{
	switch(sMaterial)
	{
	case CHAR_TEX_METAL : 
		fPenetrationPowerModifier = 0.5; 
		fDamageModifier = 0.3;
		break;
	case CHAR_TEX_DIRT :
		fPenetrationPowerModifier = 0.5;
		fDamageModifier = 0.3;
		break;
	case CHAR_TEX_CONCRETE : 
		fPenetrationPowerModifier = 0.4;
		fDamageModifier = 0.25;
		break;
	case CHAR_TEX_GRATE	:
		fPenetrationPowerModifier = 1.0;
		fDamageModifier = 0.99;
		break;
	case CHAR_TEX_VENT : 
		fPenetrationPowerModifier = 0.5;
		fDamageModifier = 0.45;
		break;
	case CHAR_TEX_TILE :
		fPenetrationPowerModifier = 0.65;
		fDamageModifier = 0.3;
		break;
	case CHAR_TEX_COMPUTER : 
		fPenetrationPowerModifier = 0.4;
		fDamageModifier = 0.45;
		break;
	case CHAR_TEX_WOOD :
		fPenetrationPowerModifier = 1.0;
		fDamageModifier = 0.6;
		break;
	default : 
		fPenetrationPowerModifier = 1.0;
		fDamageModifier = 0.5;
		break;
	}
}

typedef void ( *ClipTraceToPlayers_t )( const Vector&, const Vector&, unsigned int, ValveSDK::CTrace::ITraceFilter*, ValveSDK::CTrace::trace_t* );
bool bIsPointPenetrable(WeaponInfo_t wiWeaponInfo, Vector vStart, Vector vEnd)
{
	ValveSDK::CTrace::trace_t trace;

	ValveSDK::CTrace::ITraceFilter *pTraceFilter = (ValveSDK::CTrace::ITraceFilter*)&g_Aimbot.tfNoPlayers;

	Vector vSource = vStart, vEnding, vDir = (vEnd - vStart), vClip;
	vDir.NormalizeInPlace();

	float fTmpLength, fSumDist, fPow;

	int iPenetration = wiWeaponInfo.iPenetration;

	float fCurrentDamage = wiWeaponInfo.iDamage;

	float fPenetrationPower = wiWeaponInfo.fPenetrationPower;

	static DWORD dwCliptracetoplayers = NULL;

	if(dwCliptracetoplayers == NULL)
	{
		dwCliptracetoplayers = Base::Utils::PatternSearch(/*client.dll*/XorStr<0x2E,11,0xAF7248CE>("\x4D\x43\x59\x54\x5C\x47\x1A\x51\x5A\x5B"+0xAF7248CE).s,(PBYTE)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x18",/*xxxxxxxxxxxxxxxxxxxxxxxx????xxx*/XorStr<0x12,32,0xA1BF7B95>("\x6A\x6B\x6C\x6D\x6E\x6F\x60\x61\x62\x63\x64\x65\x66\x67\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F\x50\x51\x15\x14\x13\x12\x56\x57\x48"+0xA1BF7B95).s,NULL,NULL);
#ifdef DEBUGMODE
		char szAddress[256];
		sprintf(szAddress,"ClipTraceToPlayers: 0x%x",dwCliptracetoplayers);
		Base::Debug::LOG(szAddress);
#endif
	}

	static ClipTraceToPlayers_t ClipTraceToPlayersCall = (ClipTraceToPlayers_t)dwCliptracetoplayers;

	float fRange = Vector(vEnd - vStart).Length();

	while(fCurrentDamage > 0.0f)
	{
		ValveSDK::CTrace::Ray_t ray;
		ray.Init(vSource,vEnd);
		g_Valve.pEngineTrace->TraceRay(ray,0x4600400B,pTraceFilter,&trace);

		vClip = (vDir * 40.0f) + vEnd;

		ClipTraceToPlayersCall(vSource,vClip,0x4600400B,pTraceFilter,&trace);

		if(trace.fraction == 1.0f)
			return true;

		ValveSDK::surfacedata_t *pSurfaceData = g_Valve.pPhysics->GetSurfaceData(trace.surface.surfaceProps);

		bool bGrate = (trace.contents & 0x8);

		fTmpLength = (fSumDist + ((fRange - fSumDist) * trace.fraction));

		fPow = (pow(wiWeaponInfo.fRangeModifier,(fTmpLength * 0.002f)));
		fCurrentDamage *= fPow;

		if(fTmpLength > wiWeaponInfo.fPenetrationDistance)
			iPenetration = ((iPenetration <= 0) ? iPenetration : 0);
	
		if(iPenetration < 0 || (iPenetration == 0 && !bGrate))
			break;

		float fPenetrationPowerModifier;
		float fDamageModifier;

		unsigned short iMaterial = pSurfaceData->game.material;
		char sMaterial = (char)iMaterial;

		GetMaterialParameters(sMaterial,fPenetrationPowerModifier,fDamageModifier);

		if ( bGrate )
		{
			fPenetrationPowerModifier = 1.0f;
			fDamageModifier = 0.99f;
		}

		Vector vEndPos2;

		float multiplier = 24;

		for( ; ; multiplier += 24)
		{
			vEndPos2 = (trace.endpos + (vDir * multiplier));

			if(!(g_Valve.pEngineTrace->GetPointContents( vEndPos2 ) & 0x200400B))		
				break;
							
			if(multiplier > 128)						
				return false;
		}

		ValveSDK::CTrace::trace_t trace2;

		ray.Init(vEndPos2,trace.endpos);
		g_Valve.pEngineTrace->TraceRay(ray,0x4600400B,0,&trace2);

		if((trace2.m_pEnt != trace.m_pEnt) && trace2.m_pEnt)
		{
			ValveSDK::CTrace::CSimpleTraceFilter pTraceTwoFilter(trace2.m_pEnt);

			ray.Init(vEndPos2,trace.endpos);
			g_Valve.pEngineTrace->TraceRay(ray,0x4600400B,(ValveSDK::CTrace::ITraceFilter*)&pTraceTwoFilter,&trace2);
		}

		float fNewTempLength = Vector(trace2.endpos - trace.endpos).Length();
		
		if(g_Valve.pPhysics->GetSurfaceData(trace2.surface.surfaceProps)->game.material == iMaterial && (iMaterial == 87 || iMaterial == 77))
			fPenetrationPowerModifier += fPenetrationPowerModifier;

		if(fNewTempLength > (fPenetrationPower * fPenetrationPowerModifier))
			break;

		fPenetrationPower -= (fNewTempLength / fPenetrationPowerModifier);

		vSource = trace2.endpos;

		fCurrentDamage *= fDamageModifier;

		fSumDist = fTmpLength + fNewTempLength;

		--iPenetration;
	}

	return false;
}

bool CAimbot::MultiPoints(CBaseEntity* pPlayer, Vector &vPos, int iHitBox)
{
	Vector vMax;
	Vector vMin;

	matrix3x4_t matrix[128];

	if (!pPlayer->SetupBones(matrix, 128, 0x00000100, 0.0f))
		return false;

	studiohdr_t* hdr = g_Valve.pModel->GetStudiomodel(pPlayer->GetModel());

	if(!hdr)
		return false;

	mstudiobbox_t* hitbox = hdr->pHitboxSet(0)->pHitbox(iHitBox);

	if (!hitbox)
		return false;

	g_Math.VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
	g_Math.VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);

	//main spot
	vPos = (vMin + vMax) * 0.5;

	if(pPlayer->IsUsingAntiAim())
		vPos.z += 4.2f;
	else
		vPos.z += (float)g_CVARS.CvarList[AimHeight];

	if(!g_CVARS.CvarList[Autowall] && IsVisible(vEyePos,vPos,0x46004003,(ValveSDK::CTrace::ITraceFilter*)&g_Aimbot.tfNoPlayers))
		return true;
	else if(g_CVARS.CvarList[Autowall] && bIsPointPenetrable(wiWeaponInfo,vEyePos,vPos))
		return true;
	else if(g_CVARS.CvarList[Hitscans])
	{
		for(int i = 0; i < 50; i++)
		{
			//dirty bones that dont really work well on all maps
			//if(i == 20 || i == 19 || i == 17 || i == 23)
			//	continue; 

			vPoints[i] = GetBoneFromMatrix(matrix,i);

			if(!g_CVARS.CvarList[HitscanAutowall] && IsVisible(vEyePos,vPoints[i],0x46004003,(ValveSDK::CTrace::ITraceFilter*)&g_Aimbot.tfNoPlayers))
			{
				vPos = vPoints[i];
				return true;//who cares which point is better??? lets just aim at first visible one
			}
			else if(g_CVARS.CvarList[HitscanAutowall] && bIsPointPenetrable(wiWeaponInfo,vEyePos,vPoints[i]))
			{
				vPos = vPoints[i];
				return true;
			}
		}	
	}

	return false;
}

bool GetHiboxPosition(CBaseEntity* pPlayer, Vector &vPos, int iHitBox)
{
	Vector vMax;
	Vector vMin;

	matrix3x4_t matrix[128];

	if (!pPlayer->SetupBones(matrix, 128, 0x00000100, 0.0f))
		return false;

	studiohdr_t* hdr = g_Valve.pModel->GetStudiomodel(pPlayer->GetModel());

	if(!hdr)
		return false;

	mstudiobbox_t* hitbox = hdr->pHitboxSet(0)->pHitbox(iHitBox);

	if (!hitbox)
		return false;

	g_Math.VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
	g_Math.VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);

	vPos = (vMin + vMax) * 0.5;

	return true;
}

void CalcAngle(Vector &vSource, Vector &vDestination, Vector &qAngle)
{
	Vector vDelta = vSource - vDestination;

	float fHyp = (vDelta.x * vDelta.x) + (vDelta.y * vDelta.y);

	float fRoot;

	__asm
	{
		sqrtss xmm0, fHyp
		movss fRoot, xmm0
	}

	qAngle.x = RAD2DEG(atan(vDelta.z / fRoot));
	qAngle.y = RAD2DEG(atan(vDelta.y / vDelta.x));

	if(vDelta.x >= 0.0f)
		qAngle.y += 180.0f;

	qAngle.x = g_NoSpread.AngleNormalize(qAngle.x);
	qAngle.y = g_NoSpread.AngleNormalize(qAngle.y);
}

float GetFov(Vector vLocalOrigin, Vector vPosition, Vector vForward)
{
	Vector vLocal;

	VectorSubtract(vPosition, vLocalOrigin, vLocal);

	vLocal.NormalizeInPlace();

	float fValue = vForward.Dot(vLocal);

	//np for kolo's math skills
	if(fValue < -1.0f)
		fValue = -1.0f;

	if(fValue > 1.0f)
		fValue = 1.0f;

	return RAD2DEG(acos(fValue));
}

bool CAimbot::IsVisible(Vector vStart, Vector vEnd, unsigned int nMask, ValveSDK::CTrace::ITraceFilter *pTraceFilter)
{
	ValveSDK::CTrace::Ray_t ray;
	ValveSDK::CTrace::trace_t tr;

	ray.Init(vStart,vEnd);
	g_Valve.pEngineTrace->TraceRay(ray,nMask,pTraceFilter,&tr);

	return (tr.fraction == 1.0f);
}

void CAimbot::FixMovement(ValveSDK::CInput::CUserCmd* c, Vector &qOld, int iPositive)
{
	Vector vMove(c->forwardmove, c->sidemove, c->upmove);
	float fSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y);
	static Vector qMove;
	g_NoSpread.vectorAngles(vMove,qMove);

	float fYaw = DEG2RAD(c->viewangles.y - qOld.y + qMove.y);

	c->forwardmove = cos(fYaw) * fSpeed * iPositive;
	c->sidemove = sin(fYaw) * fSpeed;
}

void GetWeaponInfo(ValveSDK::CBaseCombatWeapon *pWeapon, WeaponInfo_t &wiInfo, int iWeaponID, bool bSilencer)
{
	switch(iWeaponID)
	{
	case WEAPON_AK47:
		{
			wiInfo.iBulletType = 2;
			wiInfo.iPenetration = 2;
			wiInfo.iDamage = 36;
			wiInfo.fMaxRange = 8192.0f;
			wiInfo.fRangeModifier = 0.98f;
			break;
		}

	case WEAPON_AUG:
		{
			wiInfo.iBulletType = 2;
			wiInfo.iPenetration = 2;
			wiInfo.iDamage = 32;
			wiInfo.fMaxRange = 8192.0f;
			wiInfo.fRangeModifier = 0.96f;
			break;
		}

	case WEAPON_AWP:
		{
			wiInfo.iBulletType = 5;
			wiInfo.iPenetration = 3;
			wiInfo.iDamage = 115;
			wiInfo.fMaxRange = 8192.0f;
			wiInfo.fRangeModifier = 0.99f;
			break;
		}

	case WEAPON_DEAGLE:
		{
			wiInfo.iBulletType = 1;
			wiInfo.iPenetration = 2;
			wiInfo.iDamage = 54;
			wiInfo.fMaxRange = 4096.0f;
			wiInfo.fRangeModifier = 0.81f;
			break;		
		}

	case WEAPON_ELITES:
		{
			wiInfo.iBulletType = 6;
			wiInfo.iPenetration = 1;
			wiInfo.iDamage = 45;
			wiInfo.fMaxRange = 4096.0f;
			wiInfo.fRangeModifier = 0.75f;
			break;	
		}

	case WEAPON_FAMAS:
		{
			wiInfo.iBulletType = 3;
			wiInfo.iPenetration = 2;
			wiInfo.iDamage = 30;
			wiInfo.fMaxRange = 8192.0f;
			wiInfo.fRangeModifier = 0.96f;
			break;
		}

	case WEAPON_FIVESEVEN:
		{
			wiInfo.iBulletType = 10;
			wiInfo.iPenetration = 1;
			wiInfo.iDamage = 25;
			wiInfo.fMaxRange = 4096.0f;
			wiInfo.fRangeModifier = 0.885f;
			break;
		}

	case WEAPON_G3SG1:
		{
			wiInfo.iBulletType = 2;
			wiInfo.iPenetration = 3;
			wiInfo.iDamage = 80;
			wiInfo.fMaxRange = 8192.0f;
			wiInfo.fRangeModifier = 0.98f;
			break;
		}

	case WEAPON_GALIL:
		{
			wiInfo.iBulletType = 3;
			wiInfo.iPenetration = 2;
			wiInfo.iDamage = 30;
			wiInfo.fMaxRange = 8192.0f;
			wiInfo.fRangeModifier = 0.98f;
			break;
		}

	case WEAPON_GLOCK:
		{
			wiInfo.iBulletType = 6;
			wiInfo.iPenetration = 1;
			wiInfo.iDamage = 25;
			wiInfo.fMaxRange = 4096.0f;
			wiInfo.fRangeModifier = 0.75f;
			break;	
		}

	case WEAPON_M249:
		{
			wiInfo.iBulletType = 4;
			wiInfo.iPenetration = 2;
			wiInfo.iDamage = 32;
			wiInfo.fMaxRange = 8192.0f;
			wiInfo.fRangeModifier = 0.97f;
			break;
		}

	case WEAPON_M4A1:
		{
			wiInfo.iBulletType = 3;
			wiInfo.iPenetration = 2;
			wiInfo.iDamage = 33;
			wiInfo.fMaxRange = 8192.0f;			
			wiInfo.fRangeModifier = (bSilencer ? 0.95f : 0.97f);
			break;
		}

	case WEAPON_MAC10:
		{
			wiInfo.iBulletType = 8;
			wiInfo.iPenetration = 1;
			wiInfo.iDamage = 29;
			wiInfo.fMaxRange = 4096.0f;
			wiInfo.fRangeModifier = 0.82f;
			break;
		}

	case WEAPON_MP5:
		{
			wiInfo.iBulletType = 6;
			wiInfo.iPenetration = 1;
			wiInfo.iDamage = 26;
			wiInfo.fMaxRange = 4096.0f;
			wiInfo.fRangeModifier = 0.84f;
			break;
		}

	case WEAPON_P228:
		{
			wiInfo.iBulletType = 9;
			wiInfo.iPenetration = 1;
			wiInfo.iDamage = 40;
			wiInfo.fMaxRange = 4096.0f;
			wiInfo.fRangeModifier = 0.8f;
			break;			
		}

	case WEAPON_P90:
		{
			wiInfo.iBulletType = 10;
			wiInfo.iPenetration = 1;
			wiInfo.iDamage = 26;
			wiInfo.fMaxRange = 4096.0f;
			wiInfo.fRangeModifier = 0.84f;
			break;
		}

	case WEAPON_SCOUT:
		{
			wiInfo.iBulletType = 2;
			wiInfo.iPenetration = 1;
			wiInfo.iDamage = 75;
			wiInfo.fMaxRange = 8192.0f;
			wiInfo.fRangeModifier = 0.98f;
			break;
		}

	case WEAPON_SG550:
		{
			wiInfo.iBulletType = 3;
			wiInfo.iPenetration = 2;
			wiInfo.iDamage = 70;
			wiInfo.fMaxRange = 8192.0f;
			wiInfo.fRangeModifier = 0.98f;
			break;
		}

	case WEAPON_SG552:
		{
			wiInfo.iBulletType = 3;
			wiInfo.iPenetration = 2;
			wiInfo.iDamage = 33;
			wiInfo.fMaxRange = 8192.0f;
			wiInfo.fRangeModifier = 0.955f;
			break;
		}

	case WEAPON_TMP:
		{
			wiInfo.iBulletType = 6;
			wiInfo.iPenetration = 1;
			wiInfo.iDamage = 26;
			wiInfo.fMaxRange = 4096.0f;
			wiInfo.fRangeModifier = 0.84f;
			break;
		}

	case WEAPON_UMP:
		{
			wiInfo.iBulletType = 8;
			wiInfo.iPenetration = 1;
			wiInfo.iDamage = 30;
			wiInfo.fMaxRange = 4096.0f;
			wiInfo.fRangeModifier = 0.82f;
			break;
		}

	case WEAPON_USP:
		{
			wiInfo.iBulletType = 8;
			wiInfo.iPenetration = 1;
			wiInfo.fMaxRange = 4096.0f;
			wiInfo.fRangeModifier = 0.79f;		
			wiInfo.iDamage = (bSilencer ? 30 : 34);
			break;
		}
	case WEAPON_XM1014:
		{
			wiInfo.iBulletType = 7;
			wiInfo.iPenetration = 1;
			wiInfo.iDamage = 22;
			wiInfo.fRangeModifier = 0.7f;
			wiInfo.fMaxRange = 3000.0f;
			break;
		}
	case WEAPON_M3:
		{
			wiInfo.iBulletType = 7;
			wiInfo.iPenetration = 1;
			wiInfo.iDamage = 26;
			wiInfo.fRangeModifier = 0.7f;
			wiInfo.fMaxRange = 3000.0f;
			break;
		}
	default:
		{
			break;
		}
	}

	static DWORD dwBulletParams = NULL;

	if(dwBulletParams == NULL)
	{
		dwBulletParams = Base::Utils::PatternSearch(/*client.dll*/XorStr<0x04,11,0x9335C284>("\x67\x69\x6F\x62\x66\x7D\x24\x6F\x60\x61"+0x9335C284).s,(PBYTE)"\x55\x8B\xEC\x56\x8B\x75\x08\x68\x00\x00\x00\x00\x56\xE8\x00\x00\x00\x00\x83\xC4\x08\x84\xC0",/*xxxxxxxx????xx????xxxxx*/XorStr<0x44,24,0x99359960>("\x3C\x3D\x3E\x3F\x30\x31\x32\x33\x73\x72\x71\x70\x28\x29\x6D\x6C\x6B\x6A\x2E\x2F\x20\x21\x22"+0x99359960).s,NULL,NULL);
#ifdef DEBUGMODE
		char szAddy[256];
		sprintf(szAddy,"GetBulletTypeParameters: 0x%x",dwBulletParams);
		Base::Debug::LOG(szAddy);
#endif
	}

	float fPenDist;
	float fPenPow;

	int iBulletTup = wiInfo.iBulletType;

	__asm
	{
		LEA ECX, fPenDist
		PUSH ECX
		LEA EDX, fPenPow
		PUSH EDX
		PUSH iBulletTup
		CALL dwBulletParams
	}

	wiInfo.fPenetrationDistance = fPenDist;
	wiInfo.fPenetrationPower = fPenPow;
}

void CAimbot::Main(ValveSDK::CInput::CUserCmd* pUserCmd, CBaseEntity* pLocal, ValveSDK::CBaseCombatWeapon *pWeapon)
{
	int iMyTeam = pLocal->GetTeamNum();

	//optimizing this crap
	if(g_CVARS.CvarList[Autowall] || g_CVARS.CvarList[HitscanAutowall])
	{
		static int iOldWeaponID;
		static bool bOldSilenced;

		int iWeaponID = pWeapon->GetWeaponID();
		bool bSilenced = pWeapon->IsSilencerOn();

		if(iWeaponID != iOldWeaponID || bSilenced != bOldSilenced)
			GetWeaponInfo(pWeapon,wiWeaponInfo,iWeaponID,bSilenced);

		iOldWeaponID = iWeaponID;
		bOldSilenced = bSilenced;
	}

	vEyePos = pLocal->GetEyePosition();

	static Vector vClientViewAngles;
	g_Valve.pEngine->GetViewAngles(vClientViewAngles);

	static Vector vAngle;
	g_NoSpread.angleVectors(vClientViewAngles,vAngle);

	Reset();

	for (INT ax = 1; ax <= g_Valve.pEngine->GetMaxClients(); ax++)
	{
		CBaseEntity* pBaseEntity = g_Valve.pEntList->GetClientEntity(ax);

		if (!pBaseEntity
		|| pBaseEntity == pLocal
		|| !pBaseEntity->isValidPlayer()
		|| fWhiteList[ax]
		|| (!g_CVARS.CvarList[AimTeam] && pBaseEntity->GetTeamNum() == iMyTeam)
		|| (g_CVARS.CvarList[AntiDM] && pBaseEntity->IsSpawnProtectedPlayer())
		|| !MultiPoints(pBaseEntity,vTarget,(fSpecialAimspot[ax] != -1 ? fSpecialAimspot[ax] : g_CVARS.CvarList[AimSpot])))
			continue;

		float fCurrFOV = GetFov(vEyePos,vTarget,vAngle);

		if(fCurrFOV > g_CVARS.CvarList[AimFOV])	//aimfov
			continue;

		if(g_CVARS.CvarList[TargetSelection] == 1)//targetselection
			fCurrFOV = (Vector(vTarget - vEyePos).Length());

		if(fBestTarget < fCurrFOV)
			continue;

		fBestTarget = fCurrFOV;

		vFinal = vTarget;

		//this is our target which is closest to crosshair
		iTarget = ax;
	}

	if(HasTarget())
	{
		CalcAngle(vEyePos,vFinal,pUserCmd->viewangles);

		if(g_CVARS.CvarList[SmoothAim] > 0)
		{
			vClientViewAngles.x = g_NoSpread.AngleNormalize(vClientViewAngles.x);
			vClientViewAngles.y = g_NoSpread.AngleNormalize(vClientViewAngles.y);

			Vector qDelta = pUserCmd->viewangles - vClientViewAngles;

			qDelta.x = g_NoSpread.AngleNormalize(qDelta.x);
			qDelta.y = g_NoSpread.AngleNormalize(qDelta.y);

			pUserCmd->viewangles.x = vClientViewAngles.x + qDelta.x / (float)g_CVARS.CvarList[SmoothAim];
			pUserCmd->viewangles.y = vClientViewAngles.y + qDelta.y / (float)g_CVARS.CvarList[SmoothAim];			

			pUserCmd->viewangles.x = g_NoSpread.AngleNormalize(pUserCmd->viewangles.x);
			pUserCmd->viewangles.y = g_NoSpread.AngleNormalize(pUserCmd->viewangles.y);
		}

		if(!g_CVARS.CvarList[SilentAim])
			g_Valve.pEngine->SetViewAngles(pUserCmd->viewangles);

		if(g_CVARS.CvarList[Autoshoot])
			pUserCmd->buttons |= IN_ATTACK;
	}
}

//valve sdk.. i think this is outdated for csgo but cba to recheck hehe
Vector head_hull_mins( -16, -16, -18 );
Vector head_hull_maxs( 16, 16, 18 );

#define	MASK_SOLID (0x1|0x4000|0x2|0x2000000|0x8)

bool CAimbot::Main_Knifebot(CBaseEntity* pLocal, ValveSDK::CBaseCombatWeapon *pWeapon)
{
	float fRange = ((g_CVARS.CvarList[Knifebot] == 2) ? 48 : 32);

	int iWeaponID = pWeapon->GetWeaponID();

	if(iWeaponID != WEAPON_KNIFE)//knife
		return false;

	Vector qEyeAngles = pLocal->GetEyeAngles();
	
	static Vector vForward;
	g_NoSpread.angleVectors(qEyeAngles,vForward);

	Vector vSource = pLocal->GetEyePosition();
	Vector vEnd = vSource + (vForward * fRange);

	ValveSDK::CTrace::trace_t tr;
	ValveSDK::CTrace::Ray_t ray;
	ValveSDK::CTrace::CSimpleTraceFilter pTraceFilter(pLocal);

	ray.Init(vSource,vEnd);
	g_Valve.pEngineTrace->TraceRay(ray,MASK_SOLID,(ValveSDK::CTrace::ITraceFilter*)&pTraceFilter,&tr);

	if(tr.fraction >= 1.0f)
	{
		ValveSDK::CTrace::Ray_t ray2;
		ray2.Init(vSource,vEnd,head_hull_mins,head_hull_maxs);
		g_Valve.pEngineTrace->TraceRay(ray2,MASK_SOLID,(ValveSDK::CTrace::ITraceFilter*)&pTraceFilter,&tr);

		if(tr.fraction < 1.0f)
			vEnd = tr.endpos;
	}

	if(tr.fraction >= 1.0f
		|| !tr.m_pEnt
		|| !tr.m_pEnt->IsPlayer()
		|| fWhiteList[tr.m_pEnt->GetIndex()]
		|| (tr.surface.flags & 0x0004)
		|| (!g_CVARS.CvarList[AimTeam] && tr.m_pEnt->GetTeamNum() == pLocal->GetTeamNum())
		|| (g_CVARS.CvarList[AntiDM] && tr.m_pEnt->IsSpawnProtectedPlayer()))
		return false;

	int iHealth = tr.m_pEnt->GetHealth();

	float fDot;

	if(g_CVARS.CvarList[BackstabOnly] || g_CVARS.CvarList[Knifebot] == 2)
	{
		Vector vTragetForward;

		Vector qEntAngles = tr.m_pEnt->GetEyeAngles();
		g_NoSpread.angleVectors(qEntAngles,vTragetForward);
		
		Vector vTo = Vector(tr.m_pEnt->GetAbsOrigin() - pLocal->GetNetworkedOrigin());

		//vector2d quick implementation
		float vLOS[2];

		vLOS[0] = vTo.x;
		vLOS[1] = vTo.y;

		float fLen = vTo.Length2D();
		if (fLen != 0.0f)
		{
			vLOS[0] /= fLen;
			vLOS[1] /= fLen;
		}
		else
			vLOS[0] = vLOS[1] = 0.0f;

		fDot = (vLOS[0] * vTragetForward.x) + (vLOS[1] * vTragetForward.y);

		if(g_CVARS.CvarList[Knifebot] == 2)
		{
			float fNextPrimary = pWeapon->GetNextPrimaryAttack();

			float fCurTime = pLocal->GetTickBase() * g_Valve.pGlobalVars->interval_per_tick;

			bool bFirstSwing = ((fNextPrimary + 0.5f) < fCurTime);

			if(!bFirstSwing && iHealth >= 21 && !(fDot > 0.475f))
				return false;
		}

		int iCheck = ((g_CVARS.CvarList[Knifebot] == 2) ? 34 : 55);

		//Triple the damage if we are stabbing them in the back.
		if(g_CVARS.CvarList[BackstabOnly] && iHealth >= iCheck)
		{
			if (!(fDot > 0.475f))
				return false;
			else
				return true;
		}
	}

	return true;
}

void CAimbot::Main_RCS(ValveSDK::CInput::CUserCmd* pUserCmd, CBaseEntity* pLocal, ValveSDK::CBaseCombatWeapon *pWeapon)
{
	int iWpnID = pWeapon->GetWeaponID();

	//we only want rifles and dualies are fine as well ehi ehi
	bool bIgnoreWeapon = (iWpnID != WEAPON_XM1014 && iWpnID != WEAPON_AWP && iWpnID != WEAPON_DEAGLE && iWpnID != WEAPON_USP
		&& iWpnID != WEAPON_FIVESEVEN && iWpnID != WEAPON_GLOCK && iWpnID != WEAPON_P228 && iWpnID != WEAPON_SCOUT && iWpnID != WEAPON_SG550
		&& iWpnID != WEAPON_G3SG1);

	if(!bIgnoreWeapon)
		return;

	int iMyTeam = pLocal->GetTeamNum();

	Vector vEyePosition = pLocal->GetEyePosition();

	static Vector vClientViewAngles;
	g_Valve.pEngine->GetViewAngles(vClientViewAngles);

	static Vector vAngle;
	g_NoSpread.angleVectors(vClientViewAngles,vAngle);

	ResetRCS();

	for (INT ax = 1; ax <= g_Valve.pEngine->GetMaxClients(); ax++)
	{
		CBaseEntity* pBaseEntity = g_Valve.pEntList->GetClientEntity(ax);

		if (!pBaseEntity
			|| !pBaseEntity->isValidPlayer()
			|| pBaseEntity == pLocal
			|| (!g_CVARS.CvarList[RCSAimTeam] && pBaseEntity->GetTeamNum() == iMyTeam)
			|| !GetHiboxPosition(pBaseEntity,vRCSTarget,g_CVARS.CvarList[RCSSpot])
			|| !IsVisible(vEyePosition,vRCSTarget,0x46004003,(ValveSDK::CTrace::ITraceFilter*)&tfNoPlayers))
			continue;

		float fCurrFOV = GetFov(vEyePosition,vRCSTarget,vAngle);

		if(fBestRCSTarget < fCurrFOV)
			continue;

		fBestRCSTarget = fCurrFOV;

		vRCSFinal = vRCSTarget;

		//this is our target which is closest to crosshair
		iRCSTarget = ax;
	}

	if(HasTargetRCS())
	{
		float fLen = Vector(vRCSFinal - vEyePosition).Length();

		Vector qAngle;
		CalcAngle(vEyePosition,vRCSFinal,qAngle);

		if(g_CVARS.CvarList[SCS])
		{
			float fSpeed = pLocal->GetVelocity().Length();

			if(fSpeed >= 0.0f && fSpeed < 1.0f)
				g_NoSpread.GetSpreadFix(pWeapon,pUserCmd->random_seed,qAngle);
		}

		Vector qPunchangle = pLocal->GetPunchAngle();

		//again dont apply to fkn z fraction
		qAngle.x -= qPunchangle.x * 2;
		qAngle.y -= qPunchangle.y * 2;

		qAngle.x = g_NoSpread.AngleNormalize(qAngle.x);
		qAngle.y = g_NoSpread.AngleNormalize(qAngle.y);

		static Vector vForward;
		g_NoSpread.angleVectors(qAngle,vForward);

		vForward = (vForward * fLen) + vEyePosition;

		if(GetFov(vEyePosition,vForward,vAngle) <= g_CVARS.CvarList[RCSFOV])
		{
			pUserCmd->viewangles = qAngle;

			if(g_CVARS.CvarList[RCSSmooth] > 0)
			{
				vClientViewAngles.x = g_NoSpread.AngleNormalize(vClientViewAngles.x);
				vClientViewAngles.y = g_NoSpread.AngleNormalize(vClientViewAngles.y);

				Vector qDelta = pUserCmd->viewangles - vClientViewAngles;

				qDelta.x = g_NoSpread.AngleNormalize(qDelta.x);
				qDelta.y = g_NoSpread.AngleNormalize(qDelta.y);

				pUserCmd->viewangles.x = vClientViewAngles.x + qDelta.x / (float)g_CVARS.CvarList[RCSSmooth];
				pUserCmd->viewangles.y = vClientViewAngles.y + qDelta.y / (float)g_CVARS.CvarList[RCSSmooth];			

				pUserCmd->viewangles.x = g_NoSpread.AngleNormalize(pUserCmd->viewangles.x);
				pUserCmd->viewangles.y = g_NoSpread.AngleNormalize(pUserCmd->viewangles.y);
			}

			pUserCmd->viewangles.z = 0.0f;

			g_Valve.pEngine->SetViewAngles(pUserCmd->viewangles);
		}
	}
}

bool CAimbot::Main_Triggerbot(Vector vSource, Vector qCurAngle, Vector vForward, Vector vRight, Vector vUp, float fMaxDistance, int iLocalTeamNumber,ValveSDK::CBaseCombatWeapon *pWeapon, CBaseEntity *pLocal, int iSeed)
{
	static bool bBurst;

	bool bRet = false;

	ValveSDK::CTrace::trace_t tr;
	ValveSDK::CTrace::Ray_t ray;

	if(g_CVARS.CvarList[PTrigger])
	{
		//*NOSPREAD PART*
		g_NoSpread.RandomSeed((iSeed & 255) + 1);
		float flA = g_NoSpread.RandomFloat(0.0f, 6.283185f);
		float flB = g_NoSpread.RandomFloat(0.0f, pWeapon->GetSpread());
		float flC = g_NoSpread.RandomFloat(0.0f, 6.283185f);
		float flD = g_NoSpread.RandomFloat(0.0f, pWeapon->GetCone());

		Vector vSpread;

		vSpread.x = (cos(flA) * flB) + (cos(flC) * flD);
		vSpread.y = (sin(flA) * flB) + (sin(flC) * flD);

		vForward.x = vForward.x + vSpread.x * vRight.x + vSpread.y * vUp.x;
		vForward.y = vForward.y + vSpread.x * vRight.y + vSpread.y * vUp.y;
		vForward.z = vForward.z + vSpread.x * vRight.z + vSpread.y * vUp.z;

		vForward.NormalizeInPlace();

		g_NoSpread.vectorAngles(vForward,qCurAngle);

		qCurAngle.x = g_NoSpread.AngleNormalize(qCurAngle.x);
		qCurAngle.y = g_NoSpread.AngleNormalize(qCurAngle.y);

		//*NORECOIL PART*
		qCurAngle += pLocal->GetPunchAngle() * 2;
	}

	Vector vEnd;
	g_NoSpread.angleVectors(qCurAngle,vEnd);

	vEnd = vSource + (vEnd * fMaxDistance);
	//g_Valve.TraceLine(vSource,vEnd,MASK_SHOT,pLocal,0,&trace);

	ValveSDK::CTrace::CSimpleTraceFilter tfSimple(pLocal);
	ray.Init(vSource,vEnd);
	g_Valve.pEngineTrace->TraceRay(ray,0x46004003,(ValveSDK::CTrace::ITraceFilter*)&tfSimple,&tr);

	bool bNowBurst = false;
	static DWORD fTimer = GetTickCount();

	if(tr.m_pEnt && tr.m_pEnt->IsPlayer())
	{
		//kolonote:
		//css fix for head triggering (bbox_maxs z component is too small)
		//credits: me, wav
		
		//int iTeam = g_Valve.pClient->GetPlayerTeamNumber(trace.m_pEnt);
		//int MiTeam = g_Valve.pClient->GetPlayerTeamNumber(pLocal);

		if( !fWhiteList[tr.m_pEnt->GetIndex()]
			&& ((!g_CVARS.CvarList[TriggerAll] && tr.m_pEnt->GetTeamNum() != iLocalTeamNumber) || g_CVARS.CvarList[TriggerAll])
			&& ((g_CVARS.CvarList[TriggerHead] && tr.hitgroup == 1) || !g_CVARS.CvarList[TriggerHead])
			&& ((g_CVARS.CvarList[IgnoreLegsAndArms] && tr.hitgroup != 4 && tr.hitgroup != 5 && tr.hitgroup != 6 && tr.hitgroup != 7) || !g_CVARS.CvarList[IgnoreLegsAndArms])
			&& ((g_CVARS.CvarList[AntiDM] && !tr.m_pEnt->IsSpawnProtectedPlayer()) || !g_CVARS.CvarList[AntiDM]) )
		{
			bRet = true;

			fTimer = 0;
			bBurst = true;
			bNowBurst = true;
		}
	}

	if(!bNowBurst && bBurst)
	{
		fTimer = GetTickCount() + (int)g_NoSpread.RandomFloat(200.0f,350.0f);
		bBurst = false;
	}

	if(g_CVARS.CvarList[TriggerBurst] && fTimer > GetTickCount())
		bRet = true;

	return bRet;
}

bool CAimbot::Triggerbot(ValveSDK::CBaseCombatWeapon *pWeapon, CBaseEntity *pLocal, ValveSDK::CInput::CUserCmd* pUserCmd)
{
	Vector vEyePosition = pLocal->GetEyePosition();
	Vector qCurAngle = pUserCmd->viewangles;

	Vector vForward,vRight,vUp;
	g_NoSpread.angleVectors(qCurAngle, vForward, vRight, vUp);

	int iMyTeam = pLocal->GetTeamNum();

	//kolonote:
	//css fix for head triggering (bbox_maxs z component is too small)
	//credits: me, wav
	for(INT ax = 1; ax <= g_Valve.pEngine->GetMaxClients(); ax++)
	{
		CBaseEntity* pBaseEntity = g_Valve.pEntList->GetClientEntity(ax);

		if (!pBaseEntity
		|| !pBaseEntity->isValidPlayer()
		|| pBaseEntity == pLocal)
			continue;

		PVOID pCollisionProperty = pBaseEntity->GetCollisionProperty();

		PFLOAT pfvecMaxsZ = (PFLOAT)((DWORD)pCollisionProperty + 0x1C);//vecMaxs.z
		PFLOAT pfvecMaxsZTwo = (PFLOAT)((DWORD)pCollisionProperty + 0x34);//vecMaxs2.z ???

		Vector vMini, vMaxi;
		pBaseEntity->GetRenderBounds(vMini,vMaxi);

		if(*pfvecMaxsZ == vMaxi.z && *pfvecMaxsZTwo == vMaxi.z)
			continue;

		*pfvecMaxsZ = vMaxi.z;
		*pfvecMaxsZTwo = vMaxi.z;

		Vector vecSize;
		VectorSubtract( vMaxi, vMini, vecSize );
		float fNewRadius = vecSize.Length() * 0.5f;

		*(PFLOAT)((DWORD)pCollisionProperty + 0x38) = fNewRadius;//m_flRadius

		pBaseEntity->AddEFlags(0x4000);
	}

	if(!g_CVARS.CvarList[TriggerSeed])
		return Main_Triggerbot(vEyePosition,qCurAngle,vForward,vRight,vUp,8192.0f,iMyTeam,pWeapon,pLocal,pUserCmd->random_seed);
	else
	{
		int iHitSeed = 1337;

		int iStrength = 256;

		if(g_CVARS.CvarList[TriggerSeed] == 1)
			iStrength = 32;
		else if(g_CVARS.CvarList[TriggerSeed] == 2)
			iStrength = 64;
		else if(g_CVARS.CvarList[TriggerSeed] == 3)
			iStrength = 128;

		for(int iSeed = 0; iSeed < iStrength; iSeed++)
		{
			if(Main_Triggerbot(vEyePosition,qCurAngle,vForward,vRight,vUp,8192.0f,iMyTeam,pWeapon,pLocal,iSeed))
			{
				iHitSeed = iSeed;
				break;//dont waste cpu more
			}
		}

		if(iHitSeed != 1337)
		{
			int iCurrentCommand = pUserCmd->command_number;

			for( ; ; iCurrentCommand++)
			{
				int iRandomSeed = (MD5_PseudoRandom(iCurrentCommand) & 0x7fffffff) & 255;

				if(iRandomSeed == iHitSeed)
				{
					pUserCmd->command_number = iCurrentCommand;

					//shoot
					pUserCmd->buttons |= IN_ATTACK;

					break;//done
				}
			}
		}

		return false;
	}
}








