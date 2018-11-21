#include "DllMain.h"

color32 CBaseEntity::GetModelColor()
{
	static int iOffset = g_NetworkedVariableManager.GetOffset("DT_CSPlayer", "m_clrRender");
	return *(color32*)((DWORD)this + iOffset);
}

ValveSDK::CBaseCombatWeapon* CBaseEntity::GetActiveBaseCombatWeapon()
{
	static int iOffset = g_NetworkedVariableManager.GetOffset("DT_CSPlayer","m_hActiveWeapon"); //m_hActiveWeapon
	ULONG pWeepEhandle = *(PULONG)((DWORD)this + iOffset);
	return (ValveSDK::CBaseCombatWeapon*)(g_Valve.pEntList->GetClientEntityFromHandle(pWeepEhandle));
}

void* GetInterfacePtr(const char* interfaceName, const char* ptrName, CreateInterface_t pInterface)
{
	char szDebugString[1024];

	std::string sinterface = "";
	std::string interfaceVersion = "0";

	for (int i = 0; i <= 99; i++)
	{
		sinterface = interfaceName;
		sinterface += interfaceVersion;
		sinterface += std::to_string(i);

		void* funcPtr = pInterface(sinterface.c_str(), nullptr);

		if ((DWORD)funcPtr != 0x0)
		{
			sprintf_s(szDebugString, "%s: 0x%x (%s%s%i)", ptrName, (DWORD)funcPtr, interfaceName, interfaceVersion.c_str(), i);
			//cout << iblue << ptrName << igreen << ": 0x" << funcPtr << "(" << iyellow << interfaceName << interfaceVersion << i << igreen << ")" << white << endl;
			return funcPtr;
		}
		if (i >= 99 && interfaceVersion == "0")
		{
			interfaceVersion = "00";
			i = 0;
		}
		else if (i >= 99 && interfaceVersion == "00")
		{
			sprintf_s(szDebugString, "%s: 0x%x (error)", ptrName, (DWORD)funcPtr);
			//cout << ired << ptrName << ": 0x" << funcPtr << " (ERROR)" << white << endl;
		}
	}
	return nullptr;
}

void* getInterface(void* factory, char* name)
{
	CreateInterfaceFn fn = (CreateInterfaceFn)factory;

	void *ret = 0;
	for (int i = 0; i<100; i++)
	{
		char* fmt = (i < 10) ? "%s00%d" : "%s0%d";

		std::string formatted = std::to_string(i); //utils::string::formatString(fmt, name, i);
		void *ptr = (fn)(formatted.c_str(), 0);
		if (ptr)
		{
			//Msg(formatted.c_str());
			//Msg("\n");

			ret = ptr;
			return ret;
		}

	}

	return ret;
}

#define INTERFACE(intrfc, cls, name, ptr, factory) intrfc = (ValveSDK::cls*)GetInterfacePtr(name, ptr, factory)

VOID CValve::initSDK()
{
#ifdef DEBUGMODE
	Base::Debug::LOG("Initiate Thread");
	char szDebugString[1024];
#endif

	//DWORD dwInitAddress = Base::Utils::PatternSearch("engine.dll",(PBYTE)"\x56\xE8\x00\x00\x00\x00\x83\x3D\x00\x00\x00\x00\x00","xx????xx?????",NULL,NULL);

	//CreateInterfaceFn AppSystemFactory = (CreateInterfaceFn)**(PDWORD*)(dwInitAddress + APPSYSTEMFACTORYOFFSET);	
				
	//CreateInterfaceFn ClientFactory = (CreateInterfaceFn)**(PDWORD*)(dwInitAddress + CLIENTFACTORYOFFSET);	

	//DWORD dwCLMoveAddress = Base::Utils::PatternSearch( /*engine.dll*/XorStr<0xA1,11,0xE75EB804>("\xC4\xCC\xC4\xCD\xCB\xC3\x89\xCC\xC5\xC6"+0xE75EB804).s,(PBYTE)"\x55\x8B\xEC\x83\xEC\x4C\x53\x56\x57\x8B\x3D",/*xxxxxxxxxxx*/XorStr<0xEF,12,0xC7505732>("\x97\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x80\x81"+0xC7505732).s,NULL,NULL);

	HMODULE module_engine = NULL;
	HMODULE module_client = NULL;
	HMODULE module_vstd = NULL;
	HMODULE module_vgui = NULL;
	HMODULE module_vguimat = NULL;
	HMODULE module_vphys = NULL;
	HMODULE module_mat = NULL;

	CreateInterfaceFn factory_engine = NULL;
	CreateInterfaceFn factory_client = NULL;
	CreateInterfaceFn factory_vstd = NULL;
	CreateInterfaceFn factory_vgui = NULL;
	CreateInterfaceFn factory_vguimat = NULL;
	CreateInterfaceFn factory_vphys = NULL;
	CreateInterfaceFn factory_mat = NULL;

	while (!module_engine || !module_client)
	{
		module_engine = GetModuleHandleA("engine.dll");
		module_client = GetModuleHandleA("client.dll");
		module_vstd = GetModuleHandleA("vstdlib.dll");
		module_vgui = GetModuleHandleA("vgui2.dll");
		module_vguimat = GetModuleHandleA("vguimatsurface.dll");
		module_vphys = GetModuleHandleA("vphysics.dll");
		module_mat = GetModuleHandleA("materialsystem.dll");

		Sleep(100);
	}

	while (!factory_engine || !factory_client)
	{
		factory_engine = (CreateInterfaceFn)GetProcAddress(module_engine, "CreateInterface");
		factory_client = (CreateInterfaceFn)GetProcAddress(module_client, "CreateInterface");
		factory_vstd = (CreateInterfaceFn)GetProcAddress(module_vstd, "CreateInterface");
		factory_vgui = (CreateInterfaceFn)GetProcAddress(module_vgui, "CreateInterface");
		factory_vguimat = (CreateInterfaceFn)GetProcAddress(module_vguimat, "CreateInterface");
		factory_vphys = (CreateInterfaceFn)GetProcAddress(module_vphys, "CreateInterface");
		factory_mat = (CreateInterfaceFn)GetProcAddress(module_mat, "CreateInterface");

		Sleep(10);
	}

	pClient = (ValveSDK::HLCLient*)GetInterfacePtr("VClient", "g_pClient", factory_client);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pClient: 0x%X", (DWORD)pClient);
	Base::Debug::LOG(szDebugString);
#endif
	pEntList = (ValveSDK::CEntityList*)GetInterfacePtr("VClientEntityList", "g_pEntList", factory_client);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pEntList: 0x%X", (DWORD)pEntList);
	Base::Debug::LOG(szDebugString);
#endif
	pPred = (ValveSDK::CPrediction*)GetInterfacePtr("VClientPrediction", "g_pPred", factory_client);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pPred: 0x%X", (DWORD)pPred);
	Base::Debug::LOG(szDebugString);
#endif
	INTERFACE(pGameMovement, CGameMovement, "GameMovement", "g_pGameMovement", factory_client);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pGameMovement: 0x%X", (DWORD)pGameMovement);
	Base::Debug::LOG(szDebugString);
#endif
	pEngine = (ValveSDK::CEngineClient*)GetInterfacePtr("VEngineClient", "g_pEngine", factory_engine);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pEngine: 0x%X", (DWORD)pEngine);
	Base::Debug::LOG(szDebugString);
#endif
	INTERFACE(pModel, CModelInfo, "VModelInfoClient", "g_pModel", factory_engine);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pModel: 0x%X", (DWORD)pModel);
	Base::Debug::LOG(szDebugString);
#endif
	INTERFACE(pDebugOverlay, CDebugOverlay, "VDebugOverlay", "g_pDebugOverlay", factory_engine);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pDebugOverlay: 0x%X", (DWORD)pDebugOverlay);
	Base::Debug::LOG(szDebugString);
#endif
	INTERFACE(pSurface, ISurface, "VGUI_Surface", "g_pSurface", factory_vguimat);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pSurface: 0x%X", (DWORD)pSurface);
	Base::Debug::LOG(szDebugString);
#endif
	INTERFACE(pPanel, IPanel, "VGUI_Panel", "g_pPanel", factory_vgui);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pPanel: 0x%X", (DWORD)pPanel);
	Base::Debug::LOG(szDebugString);
#endif
	INTERFACE(pConVar, ICvar, "VEngineCvar", "g_pConVar", factory_vstd);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pConVar: 0x%X", (DWORD)pConVar);
	Base::Debug::LOG(szDebugString);
#endif
	INTERFACE(pEngineTrace, CTrace, "EngineTraceClient", "g_pEngineTrace", factory_engine);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pEngineTrace: 0x%X", (DWORD)pEngineTrace);
	Base::Debug::LOG(szDebugString);
#endif
	INTERFACE(pPhysics, IPhysicsSurfaceProps, "VPhysicsSurfaceProps", "g_pPhysics", factory_vphys);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pPhysics: 0x%X", (DWORD)pPhysics);
	Base::Debug::LOG(szDebugString);
#endif
	INTERFACE(pModelRender, IVModelRender, "VEngineModel", "g_pModelRender", factory_engine);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pModelRender: 0x%X", (DWORD)pModelRender);
	Base::Debug::LOG(szDebugString);
#endif
	INTERFACE(pRenderView, IVRenderView, "VEngineRenderView", "g_pRenderView", factory_engine);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pRenderView: 0x%X", (DWORD)pRenderView);
	Base::Debug::LOG(szDebugString);
#endif
	INTERFACE(pMaterialSystem, IMaterialSystem, "VMaterialSystem", "g_pMaterialSystem", factory_mat);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pMaterialSystem: 0x%X", (DWORD)pMaterialSystem);
	Base::Debug::LOG(szDebugString);
#endif
	pGameEventManager = (ValveSDK::IGameEventManager*)factory_engine(/*GAMEEVENTSMANAGER002*/XorStr<0x75,21,0xAAE7DB93>("\x32\x37\x3A\x3D\x3C\x2C\x3E\x32\x29\x2D\x32\xC1\xCF\xC3\xC4\xC1\xD7\xB6\xB7\xBA"+0xAAE7DB93).s,NULL);
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pGameEventManager: 0x%X", (DWORD)pGameEventManager);
	Base::Debug::LOG(szDebugString);
#endif
	pGlobalVars = *(ValveSDK::CGlobalVars**)(((*(PDWORD*)g_Valve.pClient)[0]) + GLOBALSOFFSET);				
	pGlobalVars = (ValveSDK::CGlobalVars*)*(PDWORD)pGlobalVars;
#ifdef DEBUGMODE
	sprintf(szDebugString, "g_pGlobalVars: 0x%X", (DWORD)pGlobalVars);
	Base::Debug::LOG(szDebugString);
#endif

	m_bInitiated = true;
}

BOOL CValve::isInitiated()
{
	return this->m_bInitiated;
}

bool CValve::WorldToScreen(const Vector &vOrigin, Vector &vScreen)
{
	return (pDebugOverlay->ScreenPosition(vOrigin, vScreen) != 1);
}





