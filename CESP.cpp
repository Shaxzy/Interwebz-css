#include "DllMain.h"

int esp_r = 0, esp_g = 0, esp_b = 0, esp_a = 255;

void DrawName(ValveSDK::CEngineClient::player_info_t info, CBaseEntity* pEnt, int x, int y)
{
	g_Draw.DrawStringA(g_Draw.m_ESPFont,true, x, y, esp_r, esp_g, esp_b, esp_a, info.name);
}

unsigned GetNumberOfDigits(unsigned i)
{
	return i > 0 ? (int)log10((double)i) + 1 : 1;
}

void DrawHeader(INT x, INT y, INT w, INT HealthBarWidth)
{
	INT i = 0;
	INT xCoord = x;
	INT yCoord = y;

	// Male Male :)
	for ( i = 0 ; i < 5 ; i++ )
	{
		g_Draw.FillRGBA ( x - i, y + i, w, 1, 0, 0, 0, esp_a );
		g_Draw.FillRGBA ( x - i, y + i, HealthBarWidth + 2, 1, esp_r, esp_g, esp_b, esp_a );

		g_Draw.FillRGBA ( x - 3, y + 3, HealthBarWidth + 2, 1, ( INT ) ( esp_r / 1.5f ),
			( INT ) ( esp_g / 1.5f ), ( INT ) ( esp_b / 1.5f ), esp_a );

		g_Draw.FillRGBA ( x - 4, y + 4, HealthBarWidth + 2, 1, ( INT ) ( esp_r / 1.5f ),
			( INT ) ( esp_g / 1.5f ), ( INT ) ( esp_b / 1.5f ), esp_a );
	}

	// Oben
	g_Draw.FillRGBA ( x, y, w, 1, 255, 255, 255, esp_a );

	// Unten
	g_Draw.FillRGBA ( ( x + 1 ) - 5, y + 5, w, 1, 255, 255, 255, esp_a );

	for ( i = 0 ; i < 5 ; i++ )
	{
		// Schräg links
		g_Draw.FillRGBA ( x, y, 1, 1, 255, 255, 255, esp_a );
		x--;
		y++;
	}

	x = xCoord;
	y = yCoord;

	for ( i = 0 ; i < 5 ; i++ )
	{
		// Schräg rechts
		if ( i != 0 )
			g_Draw.FillRGBA ( x + w, y, 1, 1, 255, 255, 255, esp_a );

		x--;
		y++;
	}
}

void DrawHealthBox(CBaseEntity* pEnt, int x, int y, int w)
{
	int CurHealth = pEnt->GetHealth();

	if(CurHealth > 100)
		CurHealth = 100;

	//float wx = (CurHealth * 0.5f);

	//x -= 25.0f;

	int iHealthWidth = (CurHealth * (w*0.5*0.1) * 0.2);
	int iHealthMaxWidth = (100 * (w*0.5*0.1) * 0.2);

	DrawHeader( x, y, iHealthMaxWidth + 2, iHealthWidth );
}

void DrawHeadBox(CBaseEntity* pEnt, float &boxz)
{
	matrix3x4_t Matrix[128];

	if(!pEnt->SetupBones(Matrix, 128, 0x00000100, 0))
		return;

	studiohdr_t *pStudioHeader = g_Valve.pModel->GetStudiomodel( pEnt->GetModel() );

	if(!pStudioHeader)
		return;

	mstudiobbox_t *pBox = pStudioHeader->pHitboxSet(0)->pHitbox(12);

	if(!pBox)
		return;

	Vector vTraced = (pBox->bbmin + pBox->bbmax) * 0.5f;

	static Vector vTrans;
	g_Math.VectorTransform( vTraced, Matrix[pBox->bone], vTrans );

	boxz = vTrans.z + 7.0f;

	if(g_CVARS.CvarList[HeadESP])
	{
		static Vector vTraceVec[8], vTracePoints[8], vDrawnPoints[8];

		vTraceVec[0] = (Vector(pBox->bbmin.x,pBox->bbmin.y,pBox->bbmin.z));
		vTraceVec[1] = (Vector(pBox->bbmax.x,pBox->bbmin.y,pBox->bbmin.z));

		vTraceVec[2] = (Vector(pBox->bbmax.x,pBox->bbmax.y,pBox->bbmin.z));
		vTraceVec[3] = (Vector(pBox->bbmin.x,pBox->bbmax.y,pBox->bbmin.z));

		vTraceVec[4] = (Vector(pBox->bbmin.x,pBox->bbmin.y,pBox->bbmax.z));
		vTraceVec[5] = (Vector(pBox->bbmax.x,pBox->bbmin.y,pBox->bbmax.z));

		vTraceVec[6] = (Vector(pBox->bbmax.x,pBox->bbmax.y,pBox->bbmax.z));
		vTraceVec[7] = (Vector(pBox->bbmin.x,pBox->bbmax.y,pBox->bbmax.z));

		for(int i = 0; i < 8; i++)
		{
			g_Math.VectorTransform( vTraceVec[i], Matrix[pBox->bone], vTracePoints[i] );
			g_Valve.WorldToScreen(vTracePoints[i],vDrawnPoints[i]);
		}

		g_Valve.pSurface->DrawSetColor(esp_r, esp_g, esp_b, esp_a);
		g_Valve.pSurface->DrawLine(vDrawnPoints[6].x,vDrawnPoints[6].y,vDrawnPoints[2].x,vDrawnPoints[2].y);
		g_Valve.pSurface->DrawLine(vDrawnPoints[6].x,vDrawnPoints[6].y,vDrawnPoints[5].x,vDrawnPoints[5].y);
		g_Valve.pSurface->DrawLine(vDrawnPoints[6].x,vDrawnPoints[6].y,vDrawnPoints[7].x,vDrawnPoints[7].y);
		g_Valve.pSurface->DrawLine(vDrawnPoints[2].x,vDrawnPoints[2].y,vDrawnPoints[1].x,vDrawnPoints[1].y);
		g_Valve.pSurface->DrawLine(vDrawnPoints[1].x,vDrawnPoints[1].y,vDrawnPoints[5].x,vDrawnPoints[5].y);
		g_Valve.pSurface->DrawLine(vDrawnPoints[2].x,vDrawnPoints[2].y,vDrawnPoints[3].x,vDrawnPoints[3].y);
		g_Valve.pSurface->DrawLine(vDrawnPoints[7].x,vDrawnPoints[7].y,vDrawnPoints[3].x,vDrawnPoints[3].y);
		g_Valve.pSurface->DrawLine(vDrawnPoints[3].x,vDrawnPoints[3].y,vDrawnPoints[0].x,vDrawnPoints[0].y);
		g_Valve.pSurface->DrawLine(vDrawnPoints[1].x,vDrawnPoints[1].y,vDrawnPoints[0].x,vDrawnPoints[0].y);
		g_Valve.pSurface->DrawLine(vDrawnPoints[4].x,vDrawnPoints[4].y,vDrawnPoints[0].x,vDrawnPoints[0].y);
		g_Valve.pSurface->DrawLine(vDrawnPoints[5].x,vDrawnPoints[5].y,vDrawnPoints[4].x,vDrawnPoints[4].y);
		g_Valve.pSurface->DrawLine(vDrawnPoints[4].x,vDrawnPoints[4].y,vDrawnPoints[7].x,vDrawnPoints[7].y);
	}
}

//color macros
#define ESP_T_INVIS g_CVARS.CvarList[Esp_T_InVis], g_CVARS.CvarList[Esp_T_InVis + 1], g_CVARS.CvarList[Esp_T_InVis + 2], g_CVARS.CvarList[Esp_T_InVis + 3]
#define ESP_CT_INVIS g_CVARS.CvarList[Esp_CT_InVis], g_CVARS.CvarList[Esp_CT_InVis + 1], g_CVARS.CvarList[Esp_CT_InVis + 2], g_CVARS.CvarList[Esp_CT_InVis + 3]

#define ESP_T_VIS g_CVARS.CvarList[Esp_T_Vis], g_CVARS.CvarList[Esp_T_Vis + 1], g_CVARS.CvarList[Esp_T_Vis + 2], g_CVARS.CvarList[Esp_T_Vis + 3]
#define ESP_CT_VIS g_CVARS.CvarList[Esp_CT_Vis], g_CVARS.CvarList[Esp_CT_Vis + 1], g_CVARS.CvarList[Esp_CT_Vis + 2], g_CVARS.CvarList[Esp_CT_Vis + 3]

#define GLOW_T_INVIS g_CVARS.CvarList[Glow_T_InVis], g_CVARS.CvarList[Glow_T_InVis + 1], g_CVARS.CvarList[Glow_T_InVis + 2]
#define GLOW_CT_INVIS g_CVARS.CvarList[Glow_CT_InVis], g_CVARS.CvarList[Glow_CT_InVis + 1], g_CVARS.CvarList[Glow_CT_InVis + 2]

#define GLOW_T_VIS g_CVARS.CvarList[Glow_T_Vis], g_CVARS.CvarList[Glow_T_Vis + 1], g_CVARS.CvarList[Glow_T_Vis + 2]
#define GLOW_CT_VIS g_CVARS.CvarList[Glow_CT_Vis], g_CVARS.CvarList[Glow_CT_Vis + 1], g_CVARS.CvarList[Glow_CT_Vis + 2]

void SetESPColor(int r, int g, int b, int a)
{
	esp_r = r;
	esp_g = g;
	esp_b = b;
	esp_a = a;
}

//main iteration 
void CESP::draw()
{
	//for all entities (bomb esp, weapons on ground esp, etc)
	//INT tSize = g_Valve.pEntList->GetHighestEntityIndex();

	CBaseEntity* pMe = g_Valve.pEntList->GetClientEntity(g_Valve.pEngine->GetLocalPlayer());

	if(!pMe)
		return;

	int iMyTeam = pMe->GetTeamNum();
	
	static Vector vScreen;

	static float fTop;

	//kolonote:
	//valid player indexes start with 1 and end with the max number of players the server has set
	//instead of max number of players we can use the max players in a csgo server
	for (INT ax = 1; ax <= g_Valve.pEngine->GetMaxClients(); ax++)
	{
		CBaseEntity* pBaseEntity = g_Valve.pEntList->GetClientEntity(ax);

		if (!pBaseEntity
		|| pBaseEntity == pMe
		|| !pBaseEntity->isValidPlayer())
			continue;

		int iTeamNum = pBaseEntity->GetTeamNum();

		if(g_CVARS.CvarList[ESPEnemyOnly] && iTeamNum == iMyTeam)
			continue;

		bool bVis = false;

		Vector vOrigin = pBaseEntity->GetAbsOrigin();

		if(g_CVARS.CvarList[VisChecks])
			bVis = g_Aimbot.IsVisible(pMe->GetEyePosition(),(vOrigin + Vector(0.0f,0.0f,32.0f)),0x4600400B,(ValveSDK::CTrace::ITraceFilter*)&g_Aimbot.tfNoPlayers);

		if(iTeamNum == 2)
		{
			if(!bVis)
				SetESPColor(ESP_T_INVIS);
			else
				SetESPColor(ESP_T_VIS);
		}
		else if(iTeamNum == 3)
		{
			if(!bVis)
				SetESPColor(ESP_CT_INVIS);
			else
				SetESPColor(ESP_CT_VIS);
		}

		ValveSDK::CEngineClient::player_info_t info;

		if (!g_Valve.pEngine->GetPlayerInfo(ax,&info)
			|| !g_Valve.WorldToScreen(vOrigin,vScreen))
			continue;

		DrawHeadBox(pBaseEntity,fTop);

		//All player related ESP functions get called here
		
		Vector vTop = (Vector(vOrigin.x,vOrigin.y,fTop));
			
		Vector vTopScreen;
		g_Valve.WorldToScreen(vTop,vTopScreen);

		float h = (vScreen.y - vTopScreen.y);
		float w = h / 4.5f;

		if(g_CVARS.CvarList[BoxESP])
		{
			int r_x = vTopScreen.x - w;
			w *= 2;

			int frame_w = w;

			int frame_ww = frame_w;
			frame_ww /= 3;

			frame_w /= 2.3;

			//top left
			g_Draw.FillRGBA(r_x,vTopScreen.y,frame_ww,1,esp_r,esp_g,esp_b,esp_a);
			g_Draw.FillRGBA(r_x,vTopScreen.y,1,frame_w,esp_r,esp_g,esp_b,esp_a);

			g_Draw.FillRGBA(r_x,vTopScreen.y-1,frame_ww,1,0,0,0,esp_a);
			g_Draw.FillRGBA(r_x-1,vTopScreen.y,1,frame_w,0,0,0,esp_a);

			int calc_x = r_x + (w - frame_ww);
			int calc_y = vTopScreen.y + (h - frame_w);

			//top right
			g_Draw.FillRGBA(calc_x,vTopScreen.y,frame_ww,1,esp_r,esp_g,esp_b,esp_a);
			g_Draw.FillRGBA(r_x+w,vTopScreen.y,1,frame_w,esp_r,esp_g,esp_b,esp_a);

			g_Draw.FillRGBA(calc_x,vTopScreen.y-1,frame_ww+1,1,0,0,0,esp_a);
			g_Draw.FillRGBA(r_x+w+1,vTopScreen.y,1,frame_w,0,0,0,esp_a);

			//bottom left
			g_Draw.FillRGBA(r_x,vTopScreen.y+h,frame_ww,1,esp_r,esp_g,esp_b,esp_a);
			g_Draw.FillRGBA(r_x,calc_y,1,frame_w,esp_r,esp_g,esp_b,esp_a);

			g_Draw.FillRGBA(r_x,vTopScreen.y+h+1,frame_ww,1,0,0,0,esp_a);
			g_Draw.FillRGBA(r_x-1,calc_y,1,frame_w+1,0,0,0,esp_a);

			//bottom right
			g_Draw.FillRGBA(calc_x+1,vTopScreen.y+h,frame_ww,1,esp_r,esp_g,esp_b,esp_a);
			g_Draw.FillRGBA(r_x+w,calc_y,1,frame_w,esp_r,esp_g,esp_b,esp_a);

			g_Draw.FillRGBA(calc_x+1,vTopScreen.y+h+1,frame_ww,1,0,0,0,esp_a);
			g_Draw.FillRGBA(r_x+w+1,calc_y,1,frame_w+1,0,0,0,esp_a);
		}

		if(g_CVARS.CvarList[NameESP])
			DrawName(info,pBaseEntity,vScreen.x,vTopScreen.y - 15);

		if(g_CVARS.CvarList[HealthESP])
			DrawHealthBox(pBaseEntity,vScreen.x - w * 0.5,vScreen.y + 5,w);
	}
}






