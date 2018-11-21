#include "DllMain.h"

CDraw g_Draw;

bool bMenu = true;

std::vector<ValveSDK::IMaterial*> vecMapMaterials;
std::vector<ValveSDK::IMaterial*> vecOldMapMaterials;
std::vector<ValveSDK::IMaterial*> vecSkyMaterials;
ValveSDK::IMaterial *pHands = NULL;

bool bGetMaterials = true;

bool LoadFromBuff(KeyValues *pThis, const char *pszFirst, const char *pszSecond, PVOID pSomething = 0, PVOID pAnother = 0)
{
	typedef bool(__thiscall *LoadFromBuffer_t)(KeyValues*,const char*,const char*,PVOID,PVOID);	

	static DWORD dwAddress = NULL;

	if(dwAddress == NULL)
	{
		dwAddress = Base::Utils::PatternSearch(/*client.dll*/XorStr<0xA1,11,0x8F27B968>("\xC2\xCE\xCA\xC1\xCB\xD2\x89\xCC\xC5\xC6"+0x8F27B968).s,(PBYTE)"\x55\x8B\xEC\x83\xEC\x38\x53\x8B\x5D\x0C",/*xxxxxxxxxx*/XorStr<0x4A,11,0x4007B1F8>("\x32\x33\x34\x35\x36\x37\x28\x29\x2A\x2B"+0x4007B1F8).s,NULL,NULL);
#ifdef DEBUGMODE
		char szLog[256];
		sprintf(szLog,"LoadFromBuffer: 0x%x",dwAddress);
		Base::Debug::LOG(szLog);
#endif
	}

	static LoadFromBuffer_t callLoadBuff = (LoadFromBuffer_t)(dwAddress);

	return callLoadBuff(pThis,pszFirst,pszSecond,pSomething,pAnother);
}

void ChamModel( float r, float g, float b, ValveSDK::IMaterial *pMat)
{
	float fColor[4] = { r/255, g/255, b/255, 1.0f };

	static float fGetColor[4];

	g_Valve.pRenderView->GetColorModulation(fGetColor);

	if(fGetColor[0] != fColor[0] || fGetColor[1] != fColor[1] || fGetColor[2] != fColor[2])
		g_Valve.pRenderView->SetColorModulation( fColor );

	if(pMat)
		g_Valve.pModelRender->ForcedMaterialOverride(pMat);
}

void FullCham( ValveSDK::IMaterial *pMat, const ValveSDK::ModelRenderInfo_t &pInfo, int r, int g, int b, int r2, int g2, int b2, bool bDeadIgnorez)
{
	CBaseEntity *pBaseEnt = g_Valve.pEntList->GetClientEntity(pInfo.entity_index);
	
	if(!pBaseEnt)
	{
		g_Valve.pModelRender->ForcedMaterialOverride(NULL);
		return;
	}

	if(pBaseEnt->GetHealth() >= 1)//regular lifestate checking didnt work -.-''''
	{
		int iTeamNum = pBaseEnt->GetTeamNum();

		CBaseEntity *pLocal = g_Valve.pEntList->GetClientEntity(g_Valve.pEngine->GetLocalPlayer());

		int iMyTeamNum = pLocal->GetTeamNum();

		if(g_CVARS.CvarList[ChamsEnemyOnly] && iTeamNum == iMyTeamNum)
			return;

		if( iTeamNum == 3 )
			ChamModel(r, g, b, pMat);
		else if( iTeamNum == 2 )
			ChamModel(r2, g2, b2, pMat);
		else
			g_Valve.pModelRender->ForcedMaterialOverride(NULL);
	}
	else if(!bDeadIgnorez)
		ChamModel(255, 255, 255, pMat);//if deed then they white hehe
}

//kolonote:
//macro c&p from my REAAAAALY old source (tbh kiro didnt make this first as i made it already in 2010 hah ownd - yes i did give him my code why ? hehe)
#define MAT( _TYPE_ ) "\"" + _TYPE_ + "\"\n{\n\t\"$basetexture\" \"vgui/white_additive\"\n\t\"$ignorez\" \"0\"\n\t\"$nofog\" \"1\"\n\t\"$halflambert\" \"1\"\n}"
#define MAT_IGNOREZ( _TYPE_ ) "\"" + _TYPE_ + "\"\n{\n\t\"$basetexture\" \"vgui/white_additive\"\n\t\"$ignorez\" \"1\"\n\t\"$nofog\" \"1\"\n\t\"$halflambert\" \"1\"\n}"

void GenerateRandomString(char *s, const int len)
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; i++)
	{
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

	s[len] = 0;
}

ValveSDK::IMaterial *CreateMaterial(BOOL bFullbright, BOOL bIgnorez)
{
	char szBuff[2048];

	std::string type = (bFullbright ? /*UnlitGeneric*/XorStr<0xAF,13,0x932BA6F1>("\xFA\xDE\xDD\xDB\xC7\xF3\xD0\xD8\xD2\xCA\xD0\xD9"+0x932BA6F1).s : /*VertexLitGeneric*/XorStr<0xFD,17,0x1247276C>("\xAB\x9B\x8D\x74\x64\x7A\x4F\x6D\x71\x41\x62\x66\x6C\x78\x62\x6F"+0x1247276C).s);
	std::string tmp( (bIgnorez ? MAT_IGNOREZ(type) : MAT(type)) );
	sprintf( szBuff, tmp.c_str() );

	KeyValues *pKeyValues = new KeyValues(type.c_str());

	//generate some random file name for the .vmt (residing only in memory)
	char szMaterialName[128];
	GenerateRandomString(szMaterialName,17);
	strcat(szMaterialName,/*.vmt*/XorStr<0x26,5,0x8CB78199>("\x08\x51\x45\x5D"+0x8CB78199).s);

	LoadFromBuff(pKeyValues,szMaterialName,szBuff);

	ValveSDK::IMaterial *pNew = g_Valve.pMaterialSystem->CreateMaterial(szMaterialName,(PVOID)pKeyValues);

	if(pNew)
		pNew->IncrementReferenceCount();

#ifdef DEBUGMODE
	char szNewMaterial[1024];
	sprintf(szNewMaterial,"Created new Valve Material Type[%s] Fullbright[%i] Ignorez[%i]",szMaterialName,bFullbright,bIgnorez);
	Base::Debug::LOG(szNewMaterial);
#endif

	return pNew;
}






