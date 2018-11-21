#pragma once

#define MAX_CVARS 118

enum
{
	NameESP, HealthESP, NoRecoil, NoSpread, EnableAim, EnableESP, Bunnyhop, Autopistol, AimOnFire,
	Autoshoot, SilentAim, AimTeam, AimFOV, PSilent, FakelagAmount, Triggerbot, PTrigger, TriggerBurst,
	TriggerHead, TriggerAll, NoVisRecoil, RCS, RCSFOV, RCSSmooth, RCSSpot, SCS, AimSpot,
	RCSAimTeam, TriggerSeed, AimHeight, Hitscans, Autowall, HitscanAutowall, Chams, BrightChams,
	ESPEnemyOnly, ChamsEnemyOnly, Glow, GlowEnemyOnly, AimKey, TriggerKey, SmoothAim, AimTime,
	TargetSelection, AntiDM, IgnoreLegsAndArms, Knifebot, BackstabOnly, AntiAimX, AntiAimY,
	Strafebot, XQZ, NoHands, AsusAmount, NoSky, HeadESP, BoxESP, Chams_T_Vis, RESERVED0,
	RESERVED1, Chams_T_InVis, RESERVED3, RESERVED4, Chams_CT_Vis, RESERVED6,
	RESERVED7, Chams_CT_InVis, RESERVED9, RESERVED10, Glow_T_Vis, RESERVED12,
	RESERVED35, Glow_T_InVis, RESERVED24, RESERVED23, Glow_CT_Vis, RESERVED13,
	RESERVED34, Glow_CT_InVis, RESERVED25, RESERVED22, Esp_T_Vis, RESERVED14,
	RESERVED33, RESERVED30, Esp_T_InVis, RESERVED26, RESERVED21, RESERVED18, Esp_CT_Vis, RESERVED15,
	RESERVED32, RESERVED31, Esp_CT_InVis, RESERVED27, RESERVED20, RESERVED19, ModelAlpha, VisChecks,
	SmartAim, SMACBot, HelmetESP, BombESP, BombDetonation, GrenadeESP, EntESP, WeaponESP, FlashedESP,
	DefuseESP, Radar, RadarX, RadarY, RadarHeight, RadarWorld, RadarVisuals, RadarRange, NoSmoke,
	NoFlash, TriggerWall
};

enum
{
	SaveConfig,
	LoadConfig
};

class cCVARS
{
public:
	void SetDefaultConfig()
	{
		CvarList[NameESP] = 1;
		CvarList[HealthESP] = 1;
		CvarList[ESPEnemyOnly] = 1;
		CvarList[Bunnyhop] = 1;
		CvarList[Autopistol] = 1;
		CvarList[EnableESP] = 1;
		CvarList[BoxESP] = 1;
		//CvarList[RCS] = 1;
		CvarList[RCSSpot] = 11;
		CvarList[RCSFOV] = 4;
		CvarList[RCSSmooth] = 7;
		CvarList[RadarRange] = 31;
		//CvarList[XQZ] = 1;
		CvarList[NoVisRecoil] = 1;
		CvarList[Triggerbot] = 0;
		CvarList[PTrigger] = 0;
		CvarList[TriggerSeed] = 0;
		CvarList[TriggerHead] = 1;
		//CvarList[Chams] = 2;
		CvarList[ChamsEnemyOnly] = 1;
		CvarList[AimSpot] = 12;
		CvarList[EnableAim] = 1;
		CvarList[NoSpread] = 1;
		//CvarList[NoRecoil] = 1;
		//CvarList[Autowall] = 1;
		//CvarList[PSilent] = 1;
		//CvarList[Chams] = 2;
		//CvarList[Glow] = 1;
		//CvarList[AntiAimX] = 2;
		//CvarList[AntiAimY] = 1;
		CvarList[BoxESP] = 1;
		CvarList[AsusAmount] = 255;
		CvarList[NoSky] = 1;
		CvarList[NoHands] = 1;
		CvarList[Autoshoot] = 1;
		CvarList[Knifebot] = 1;
		CvarList[AimFOV] = 360.0f;
		CvarList[ModelAlpha] = 255;
		CvarList[GlowEnemyOnly] = 1;
		CvarList[AntiDM] = 0;
		CvarList[AimKey] = 1;//lbutton todo: virtual key binder inside inkeyevent (when menu is done)
		CvarList[TriggerKey] = 69;//e
		CvarList[RadarVisuals] = 1;

		CvarList[Chams_T_InVis] = 255;
		CvarList[Chams_CT_InVis + 2] = 255;

		CvarList[Chams_T_Vis] = 255;
		CvarList[Chams_T_Vis + 1] = 255;
		CvarList[Chams_CT_Vis + 1] = 255;

		CvarList[Esp_T_InVis] = 255;
		CvarList[Esp_T_InVis + 1] = 255;
		CvarList[Esp_CT_InVis + 1] = 255;

		CvarList[Esp_T_Vis] = 255;
		CvarList[Esp_CT_Vis + 2] = 255;
		CvarList[Esp_T_InVis + 3] = CvarList[Esp_T_Vis + 3] = CvarList[Esp_CT_InVis + 3] = CvarList[Esp_CT_Vis + 3] = 255;

		CvarList[Glow_T_InVis] = 255;
		CvarList[Glow_CT_InVis + 2] = 255;

		CvarList[Glow_T_Vis] = 255;
		CvarList[Glow_T_Vis + 1] = 255;
		CvarList[Glow_CT_Vis + 1] = 255;
	}

	void HandleConfig(const char *pszConfigName, int iType);

	void Init(HMODULE hinstDLL);

	float CvarList[MAX_CVARS];

private:

	char szIniFilePath[2048];
};

extern cCVARS g_CVARS;