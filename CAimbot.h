//CSS only
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_COMPUTER		'P'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
#define CHAR_TEX_GLASS			'Y'

typedef struct
{
	int iBulletType;//CSS only
	int iPenetration;//CSS only
	int iDamage;
	float fMaxRange;
	float fRangeModifier;
	float fPenetrationPower;
	float fPenetrationDistance;//CSS only
} WeaponInfo_t;

class CAimbot
{
public:
	CAimbot()
	{
		for(int i = 0; i < 66; i++)
		{
			fIsSelected[i] = 0;
			fYawMod[i] = 0;
			fPitchMod[i] = 0;
			fWhiteList[i] = 0;
			fSpecialAimspot[i] = -1;
		}
	}

	void Main(ValveSDK::CInput::CUserCmd* pUserCmd, CBaseEntity* pLocal, ValveSDK::CBaseCombatWeapon *pWeapon);
	bool MultiPoints(CBaseEntity* pPlayer, Vector &vPos, int iHitBox);

	bool Main_Triggerbot(Vector vSource, Vector qCurAngle, Vector vForward, Vector vRight, Vector vUp, float fMaxDistance, int iLocalTeamNumber,ValveSDK::CBaseCombatWeapon *pWeapon, CBaseEntity *pLocal, int iSeed);
	bool Triggerbot(ValveSDK::CBaseCombatWeapon *pWeapon, CBaseEntity *pLocal, ValveSDK::CInput::CUserCmd* pUserCmd);
	void Main_RCS(ValveSDK::CInput::CUserCmd* pUserCmd, CBaseEntity* pLocal, ValveSDK::CBaseCombatWeapon *pWeapon);

	bool Main_Knifebot(CBaseEntity* pLocal, ValveSDK::CBaseCombatWeapon *pWeapon);

	bool IsVisible(Vector vStart, Vector vEnd, unsigned int nMask, ValveSDK::CTrace::ITraceFilter *pTraceFilter);

	void FixMovement(ValveSDK::CInput::CUserCmd* c, Vector &qOld, int iPositive = 1);

	void Reset()
	{
		iTarget = -1;
		fBestTarget = 99999.9f;
		vTarget.Init(0.0f,0.0f,0.0f);
		vFinal.Init(0.0f,0.0f,0.0f);
	}

	void ResetRCS()
	{
		iRCSTarget = -1;
		fBestRCSTarget = 99999.9f;
		vRCSTarget.Init(0.0f,0.0f,0.0f);
		vRCSFinal.Init(0.0f,0.0f,0.0f);
	}

	bool HasTarget()
	{
		return (iTarget != -1);
	}

	bool HasTargetRCS()
	{
		return (iRCSTarget != -1);
	}

	void ResetPlayerList(float &handle)
	{
		if(handle)
		{
			for(int i = 0; i < 66; i++)
			{
				fIsSelected[i] = 0;
				fYawMod[i] = 0;
				fPitchMod[i] = 0;
				fWhiteList[i] = 0;
				fSpecialAimspot[i] = -1;
			}

			handle = 0;
		}
	}

	ValveSDK::CTrace::CTraceFilterNoPlayer tfNoPlayers;

	int iTarget;
	int iRCSTarget;

	float fBestTarget;
	float fBestRCSTarget;

	Vector vPoints[55];

	Vector vTarget;
	Vector vRCSTarget;

	Vector vFinal;
	Vector vRCSFinal;

	Vector vEyePos;

	WeaponInfo_t wiWeaponInfo;

	//playerlist specifics
	float fIsSelected[66];
	float fYawMod[66];
	float fPitchMod[66];
	float fWhiteList[66];
	float fSpecialAimspot[66];
};

extern CAimbot g_Aimbot;