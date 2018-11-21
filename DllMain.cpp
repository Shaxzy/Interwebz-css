////////////////////////////////
/*
SOURCE Base by rifk kolo & gt3x
CREDITS: floesen,outline,inurface
Init - 22.08-2014
*/
///////////////////////////////
#include "DllMain.h"

CValve g_Valve;
CESP g_ESP;
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
PaintTraverseFn oPaintTraverse;

static HMODULE DllBaseAddress() // works with manually mapped dlls (will get us our HMODULE)
{
	MEMORY_BASIC_INFORMATION info;
	size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)DllBaseAddress, &info, sizeof(info));
	Assert(len == sizeof(info));
	return len ? (HMODULE)info.AllocationBase : NULL;
}

void __stdcall hkdPaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	oPaintTraverse(vguiPanel, forceRepaint, allowForce);

	static bool bDidOnce = false;

	if (bDidOnce == false)
	{
		g_Draw.InitFonts();

#ifdef DEBUGMODE
		Base::Debug::LOG("Font initalized");
#endif
		bDidOnce = true;
	}

	const char* pszPanelName = g_Valve.pPanel->GetName(vguiPanel);

	static std::string sPanel = "FocusOverlayPanel";

	if (!strstr(pszPanelName, sPanel.data()))
		return;

	//pointer address to convars never change so we only get it once
	static ValveSDK::ConVar *pMouseEnable = g_Valve.pConVar->FindVar(/*cl_mouseenable*/XorStr<0x77, 15, 0xA851ADE6>("\x14\x14\x26\x17\x14\x09\x0E\x1B\x1A\xEE\xE0\xE0\xEF\xE1" + 0xA851ADE6).s);

	static std::string sXor =/*INTERWEBZ CSS SHAXZY WAS HERE*/XorStr<0x8C, 14, 0x50D39DA3>("\xC5\xC3\xDA\xCA\xC2\xC6\xD7\xD1\xCE\xB5\xD5\xC4\xCB" + 0x50D39DA3).s; //SHAXZY WAS HERE

	g_Draw.DrawStringA(g_Draw.m_WatermarkFont, true, 250, 20, 255, 255, 255, 255, sXor.data());

	if (GetAsyncKeyState(VK_INSERT) & 1)
		bMenu = !bMenu;

	if (g_CVARS.CvarList[EnableESP])
		g_ESP.draw();

	//make sure you got m_rawinput 1 ingame...
	if (bMenu)
	{
		g_Mouse.Update(pMouseEnable);

		int copy_x, copy_y;
		int copy_w, copy_h;

		g_Menu.GetMenuPos(copy_x, copy_y);
		g_Menu.GetMenuSize(copy_w, copy_h);

		g_Menu.DrawMenu();

		g_Mouse.Drag(g_Mouse.bDragged[0],
			!g_Menu.IsHandlingItem(),
			g_Mouse.LeftClick(copy_x, copy_y, copy_w, copy_h), copy_x, copy_y, g_Mouse.iDiffX[0], g_Mouse.iDiffY[0]);

		g_Menu.SetMenuPos(copy_x, copy_y);

		if (g_Menu.IsHandlingItem())
			g_Menu.RemoveMenuFlag(FL_DISABLEDRAG);//enable menu dragging

												  //DRAW MENU HEREEEEEEEEEE 
												  //concept window by me (to show dragging for example)
												  //---------------------------------------------------------------------------------------*/
												  /*static int winx = 100, winy = 100, winw = 100, winh = winw;
												  //position (almost whole box)
												  g_Mouse.Drag(g_Mouse.bDragged[0],//0 dragobject
												  !(g_Mouse.bDragged[1] || g_Mouse.bDragged[2]),
												  g_Mouse.LeftClick(winx,winy,winw-25,winh-25),winx,winy,g_Mouse.iDiffX[0],g_Mouse.iDiffY[0]);
												  //resize (bottom and left side)
												  g_Mouse.Drag(g_Mouse.bDragged[1],//next dragobject is 1
												  !(g_Mouse.bDragged[0] || g_Mouse.bDragged[2]),//check all other drag objects being dragged or smth
												  (g_Mouse.LeftClick(winx,winy+winh-25,winw,25) || g_Mouse.LeftClick(winx+winw-25,winy,25,winh)),//bottom and right side are the spots to resize we reserved 25 pixels for that
												  winw,winh,g_Mouse.iDiffX[1],g_Mouse.iDiffY[1]);//same number dragobject we used for bDragged
												  if(winh < 100)
												  winh = 100;
												  if(winw < 100)
												  winw = 100;
												  g_Draw.FillRGBA(winx,winy,winw,winh,200,70,70,255);
												  //---------------------------------------------------------------------------------------*/

												  //this comes at end so we overdraw menu
		g_Mouse.DrawMouse();
	}
	else if (g_Mouse.bReturn)//do this once as buymenu enables/disables mouse same way we do
	{
		pMouseEnable->SetValue(1);
		g_Mouse.bReturn = false;
	}

	/*if(bGetMaterials)
	{
	vecSkyMaterials.clear();
	vecMapMaterials.clear();
	for(ValveSDK::MaterialHandle_t h = 0; h < g_Valve.pMaterialSystem->GetNumMaterials(); h++)
	{
	ValveSDK::IMaterial *pFirstMaterial = g_Valve.pMaterialSystem->GetMaterial(h);
	if(!pFirstMaterial)
	continue;
	if(strstr(pFirstMaterial->GetTextureGroupName(),XorStr<0xA4,7,0xFAF2E307>("\xF7\xCE\xDF\xE5\xC7\xD1"+0xFAF2E307).s))
	vecSkyMaterials.push_back(pFirstMaterial);
	else if(strstr(pFirstMaterial->GetTextureGroupName(),XorStr<0x8A,6,0x3FB546ED>("\xDD\xE4\xFE\xE1\xEA"+0x3FB546ED).s))
	vecMapMaterials.push_back(pFirstMaterial);
	else if(strstr(pFirstMaterial->GetName(),XorStr<0x8D,8,0xFE3E79EF>("\xFB\xD1\xE7\xF1\xFF\xF6\xE0"+0xFE3E79EF).s))
	pHands = pFirstMaterial;
	}
	bGetMaterials = false;
	}
	static float fOldStrength;
	if(g_CVARS.CvarList[Asus])
	{
	if((!bAsus || g_CVARS.CvarList[AsusAmount] != fOldStrength) && !vecMapMaterials.empty())
	{
	for(int iTex = 0; iTex < vecMapMaterials.size(); iTex++)
	{
	if(!vecMapMaterials[iTex])
	continue;
	vecMapMaterials[iTex]->AlphaModulate((g_CVARS.CvarList[AsusAmount] * 0.1f));
	if(iTex == (vecMapMaterials.size()-1))
	{
	bAsus = true;
	fOldStrength = g_CVARS.CvarList[AsusAmount];
	}
	}
	}
	}
	else
	{
	if(bAsus && !vecMapMaterials.empty())
	{
	for(int iTex = 0; iTex < vecMapMaterials.size(); iTex++)
	{
	if(!vecMapMaterials[iTex])
	continue;
	vecMapMaterials[iTex]->AlphaModulate(1.0f);
	if(iTex == (vecMapMaterials.size()-1))
	bAsus = false;
	}
	}
	}
	if(g_CVARS.CvarList[NoSky])
	{
	if(!bNosky && !vecSkyMaterials.empty())
	{
	for(int iTex = 0; iTex < vecSkyMaterials.size(); iTex++)
	{
	if(!vecSkyMaterials[iTex])
	continue;
	vecSkyMaterials[iTex]->ColorModulate(0.0f,0.0f,0.0f);
	if(iTex == (vecSkyMaterials.size()-1))
	bNosky = true;
	}
	}
	}
	else
	{
	if(bNosky && !vecSkyMaterials.empty())
	{
	for(int iTex = 0; iTex < vecSkyMaterials.size(); iTex++)
	{
	if(!vecSkyMaterials[iTex])
	continue;
	vecSkyMaterials[iTex]->ColorModulate(1.0f,1.0f,1.0f);
	if(iTex == (vecSkyMaterials.size()-1))
	bNosky = false;
	}
	}
	}
	//CSS only - we just use findmaterial
	static bool bHands;
	if(g_CVARS.CvarList[NoHands] && !bHands)
	{
	if(!pHands->GetMaterialVarFlag(ValveSDK::MATERIAL_VAR_NO_DRAW))
	pHands->SetMaterialVarFlag(ValveSDK::MATERIAL_VAR_NO_DRAW,true);
	bHands = true;
	}
	else if(!g_CVARS.CvarList[NoHands] && bHands)
	{
	if(pHands->GetMaterialVarFlag(ValveSDK::MATERIAL_VAR_NO_DRAW))
	pHands->SetMaterialVarFlag(ValveSDK::MATERIAL_VAR_NO_DRAW,false);
	bHands = false;
	}*/
}

void InitDllThread(void* ptr)
{	
#ifdef DEBUGMODE
	Base::Debug::AttachDebugConsole();
#endif

	g_Valve.initSDK();

	g_NetworkedVariableManager.Init();

	g_NetworkedVariableManager.HookProp(/*DT_CSPlayer*/XorStr<0xAA,12,0x6F938DD5>("\xEE\xFF\xF3\xEE\xFD\xFF\xDC\xD0\xCB\xD6\xC6"+0x6F938DD5).s, /*m_angEyeAngles[0]*/XorStr<0x7E,18,0x0D7A3609>("\x13\x20\xE1\xEF\xE5\xC6\xFD\xE0\xC7\xE9\xEF\xE5\xEF\xF8\xD7\xBD\xD3"+0x0D7A3609).s,PitchEyeAngleProxy);
	g_NetworkedVariableManager.HookProp(/*DT_CSPlayer*/XorStr<0xAA,12,0x6F938DD5>("\xEE\xFF\xF3\xEE\xFD\xFF\xDC\xD0\xCB\xD6\xC6"+0x6F938DD5).s, /*m_angEyeAngles[1]*/XorStr<0x1B,18,0x97DB0871>("\x76\x43\x7C\x70\x78\x65\x58\x47\x62\x4A\x42\x4A\x42\x5B\x72\x1B\x76"+0x97DB0871).s,YawEyeAngleProxy);

#ifdef DEBUGMODE
	Base::Debug::LOG("Netvars initialized");
#endif

	Sleep(200);

	if (g_Valve.isInitiated())
	{
		if (g_pPanelVMT.bInitialize((PDWORD*)g_Valve.pPanel))
			oPaintTraverse = (PaintTraverseFn)g_pPanelVMT.dwHookMethod((DWORD)hkdPaintTraverse, 41);

#ifdef DEBUGMODE
		Base::Debug::LOG("hooked PaintTraverse");
#endif

		g_pClientVMT.bInitialize((PDWORD*)g_Valve.pClient);
		dwOriginCreateMove = g_pClientVMT.dwGetMethodAddress(21);
		g_pClientVMT.dwHookMethod((DWORD)hkdCreateMove, 21);

#ifdef DEBUGMODE
		Base::Debug::LOG("hooked CreateMove");
#endif
		g_Valve.pInput = (ValveSDK::CInput*)**(PDWORD*)(g_pClientVMT.dwGetMethodAddress(21) + INPUTOFFSET);

#ifdef DEBUGMODE
		char szDebugString[1024];
		sprintf(szDebugString, "g_pInput: 0x%08X ", (DWORD)g_Valve.pInput);
		Base::Debug::LOG(szDebugString);
#endif
		g_pClientVMT.dwHookMethod((DWORD)hkdIN_KeyEvent, 20);

#ifdef DEBUGMODE
		Base::Debug::LOG("hooked IN_KeyEvent");
#endif

		g_pClientVMT.dwHookMethod((DWORD)hkdFrameStageNotify, 35);

#ifdef DEBUGMODE
		Base::Debug::LOG("hooked FrameStageNotify");
#endif
		
	}
	/*

	g_pClientVMT.bInitialize((PDWORD*)g_Valve.pClient);
	dwOriginCreateMove = g_pClientVMT.dwGetMethodAddress(21);
	g_pClientVMT.dwHookMethod((DWORD)hkdCreateMove, 21);

	g_Valve.pInput = (ValveSDK::CInput*)**(PDWORD*)(g_pClientVMT.dwGetMethodAddress(21) + INPUTOFFSET);

#ifdef DEBUGMODE
	char szDebugString[1024];
	sprintf(szDebugString, "g_pInput: 0x%08X ", (DWORD)g_Valve.pInput);
	Base::Debug::LOG(szDebugString);
#endif

	g_pInputVMT.bInitialize((PDWORD*)g_Valve.pInput);
	g_pInputVMT.dwHookMethod((DWORD)hkdGetUserCmd, 8);

#ifdef DEBUGMODE
	Base::Debug::LOG("hooked GetUserCmd...");
#endif

#ifdef DEBUGMODE
	Base::Debug::LOG("hooked CreateMove...");
#endif

	g_pClientVMT.dwHookMethod((DWORD)hkdFrameStageNotify, 35);

#ifdef DEBUGMODE
	Base::Debug::LOG("hooked FrameStageNotify");
#endif

	g_pClientVMT.dwHookMethod((DWORD)hkdIN_KeyEvent, 20);

#ifdef DEBUGMODE
	Base::Debug::LOG("hooked IN_KeyEvent");
#endif

	g_pPredictionVMT.bInitialize((PDWORD*)g_Valve.pPred);
	g_pPredictionVMT.dwHookMethod((DWORD)hkdRunCommand, 17);

#ifdef DEBUGMODE
	sprintf(szDebugString,"runcommand: 0x%X",g_pPredictionVMT.dwGetMethodAddress(17));
	Base::Debug::LOG(szDebugString);
#endif

#ifdef DEBUGMODE
	Base::Debug::LOG("hooked RunCommand");
#endif

	g_pPredictionVMT.dwHookMethod((DWORD)hkdFinishMove, 19);

#ifdef DEBUGMODE
	Base::Debug::LOG("hooked FinishMove");
#endif

	g_pModelRenderVMT.bInitialize((PDWORD*)g_Valve.pModelRender);
	g_pModelRenderVMT.dwHookMethod((DWORD)hkdDrawModelExecute,19);

#ifdef DEBUGMODE
	Base::Debug::LOG("hooked DrawModelExecute");
#endif

	g_pPanelVMT.bInitialize((PDWORD*)g_Valve.pPanel);
	g_pPanelVMT.dwHookMethod((DWORD)hkdPaintTraverse, 41);

#ifdef DEBUGMODE
	Base::Debug::LOG("hooked PaintTraverse");
#endif

	//g_NetworkedVariableManager.HookProp(/*DT_CSPlayerXorStr<0xAA,12,0x6F938DD5>("\xEE\xFF\xF3\xEE\xFD\xFF\xDC\xD0\xCB\xD6\xC6"+0x6F938DD5).s, /*m_angEyeAngles[0]XorStr<0x7E,18,0x0D7A3609>("\x13\x20\xE1\xEF\xE5\xC6\xFD\xE0\xC7\xE9\xEF\xE5\xEF\xF8\xD7\xBD\xD3"+0x0D7A3609).s,PitchEyeAngleProxy);
	//g_NetworkedVariableManager.HookProp(/*DT_CSPlayerXorStr<0xAA,12,0x6F938DD5>("\xEE\xFF\xF3\xEE\xFD\xFF\xDC\xD0\xCB\xD6\xC6"+0x6F938DD5).s, /*m_angEyeAngles[1]XorStr<0x1B,18,0x97DB0871>("\x76\x43\x7C\x70\x78\x65\x58\x47\x62\x4A\x42\x4A\x42\x5B\x72\x1B\x76"+0x97DB0871).s,YawEyeAngleProxy);

	//gGameEventManager.RegisterSelf();

	/*
	char path[260];
	HMODULE hm = NULL;

	if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)&DllBaseAddress,
		&hm))
	{
		int ret = GetLastError();
		fprintf(stderr, "GetModuleHandle returned %d\n", ret);
	}

	char strDLLPath1[MAX_PATH];
	//::GetModuleFileNameA(hm, strDLLPath1, _MAX_PATH);
	//LoadConfig(strDLLPath1);
	*/
}

/*void unhookThread(LPARAM lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	while (true)
	{
		if (GetAsyncKeyState(VK_NUMPAD9) & 1)
		{
			Sleep(1000);

			g_pClientVMT.UnHook();
			g_pPredictionVMT.UnHook();
			g_pPanelVMT.UnHook();

			Sleep(2000);

			FreeConsole();
			SendMessage(FindWindow(0, "DebugMessages"), WM_CLOSE, NULL, NULL);

			FreeLibraryAndExitThread(hModule, 0);
		}
	}
}*/

#include "ReflectiveLoader.h"
#include "ReflectiveDLLInjection.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitDllThread, hinstDLL, NULL, NULL);

		g_CVARS.Init(hinstDLL);

		g_CVARS.SetDefaultConfig();

		g_CVARS.CvarList[Triggerbot] = 0;

		/*HW_PROFILE_INFO hwProfileInfo;

		GetCurrentHwProfile(&hwProfileInfo);

		char szFile[2048];
		sprintf(szFile,"%s\\hwid.txt",g_CVARS.szIniFilePath);

		std::ofstream fNew(szFile,std::ios::app);
		fNew.write(hwProfileInfo.szHwProfileGuid,strlen(hwProfileInfo.szHwProfileGuid));
		fNew.close();*/

		//HandleConfig("flick.ini",LoadConfig);
	}
	else if(dwReason == DLL_QUERY_HMODULE && lpReserved != NULL)
		*(HMODULE*)lpReserved = hinstDLL;

	return TRUE;
}






