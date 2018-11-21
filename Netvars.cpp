#include "DllMain.h"
CNetworkedVariableManager g_NetworkedVariableManager;

void CNetworkedVariableManager::Init(void)
{
	m_tables.clear();
	m_savedproxy.clear();

	ValveSDK::ClientClass *clientClass = g_Valve.pClient->GetAllClasses(); //g_InterfaceManager->Client()->GetOriginalMethod<GetAllClasses_t>( INDEX_GETALLCLASSES )( g_InterfaceManager->Client()->thisptr() );

	if (!clientClass)
	{
#ifdef DEBUGMODE
		Base::Debug::LOG("ClientClass was not found");
#endif
		return;
	}

	while (clientClass)
	{
		RecvTable *recvTable = clientClass->GetTable();

		m_tables.push_back(recvTable);

		clientClass = clientClass->NextClass();
	}
}

CNetworkedVariableManager::~CNetworkedVariableManager(void)
{
	for(int i = 0; i < m_savedproxy.size(); i++)
	{
		RecvProp *recvProp = 0;
		GetProp(m_savedproxy[i].szTableName, m_savedproxy[i].szPropName, &recvProp);

		if (!recvProp)
			return;

		recvProp->m_ProxyFn = m_savedproxy[i].SavedProxy;
	}
}

// calls GetProp wrapper to get the absolute offset of the prop
int CNetworkedVariableManager::GetOffset(const char *tableName, const char *propName)
{
	int offset = GetProp(tableName, propName);

	if (!offset)
	{
#ifdef DEBUGMODE
		printf("Failed to find offset for prop: %s from table: %s", propName, tableName);
#endif
		return 0;
	}

	return offset;
}


// calls GetProp wrapper to get prop and sets the proxy of the prop
bool CNetworkedVariableManager::HookProp(const char *tableName, const char *propName, RecvVarProxyFn function)
{
	RecvProp *recvProp = 0;
	GetProp(tableName, propName, &recvProp);

	if (!recvProp)
	{
#ifdef DEBUGMODE
	Base::Debug::LOG("HookProp failed");
#endif
		return false;
	}

	//kolonote:
	//make a list of hooked proxies, then when we eject dll we dont have to worry about proxies that point to a pointer where the pointee doesnt exist
	Oldproxy_t oldproxyinfo;

	strcpy(oldproxyinfo.szTableName,tableName);
	strcpy(oldproxyinfo.szPropName,propName);

	oldproxyinfo.SavedProxy = recvProp->m_ProxyFn;
	
	m_savedproxy.push_back(oldproxyinfo);

	//kolonote:
	//now we replace without worries
	recvProp->m_ProxyFn = function;

	return true;
}


// wrapper so we can use recursion without too much performance loss
int CNetworkedVariableManager::GetProp(const char *tableName, const char *propName, RecvProp **prop)
{
	RecvTable *recvTable = GetTable(tableName);

	if (!recvTable)
	{
#ifdef DEBUGMODE
		printf("Failed to find table: %s", tableName);
#endif
		return 0;
	}


	int offset = GetProp(recvTable, propName, prop);

	if (!offset)
	{
#ifdef DEBUGMODE
		printf("Failed to find prop: %s from table: %s", propName, tableName);
#endif
		return 0;
	}


	return offset;
}


// uses recursion to return a the relative offset to the given prop and sets the prop param
int CNetworkedVariableManager::GetProp(RecvTable *recvTable, const char *propName, RecvProp **prop)
{
	int extraOffset = 0;

	for (int i = 0; i < recvTable->m_nProps; ++i)
	{
		RecvProp *recvProp = &recvTable->m_pProps[i];


		RecvTable *child = recvProp->m_pDataTable;

		if (child
			&& (child->m_nProps > 0))
		{
			int tmp = GetProp(child, propName, prop);

			if (tmp)
			{
				extraOffset += (recvProp->m_Offset + tmp);
			}
		}


		if (_stricmp(recvProp->m_pVarName, propName))
		{
			continue;
		}


		if (prop)
		{
			*prop = recvProp;
		}

		return (recvProp->m_Offset + extraOffset);
	}

	return extraOffset;
}


RecvTable *CNetworkedVariableManager::GetTable(const char *tableName)
{
	if (m_tables.empty())
	{
#ifdef DEBUGMODE
		printf("Failed to find table: %s (m_tables is empty)", tableName);
#endif
		return 0;
	}


	for each (RecvTable *table in m_tables)
	{
		if (!table)
		{
			continue;
		}


		if (_stricmp(table->m_pNetTableName, tableName) == 0)
		{
			return table;
		}
	}

	return 0;
}

//kolonote:
//this is a bit dirty as i wrote it in 2013 -.-''
static float fOldYaw[66];
void YawEyeAngleProxy(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	static int iOffset = g_NetworkedVariableManager.GetOffset(/*DT_CSPlayer*/XorStr<0xAA,12,0x6F938DD5>("\xEE\xFF\xF3\xEE\xFD\xFF\xDC\xD0\xCB\xD6\xC6"+0x6F938DD5).s, /*m_angEyeAngles[0]*/XorStr<0x7E,18,0x0D7A3609>("\x13\x20\xE1\xEF\xE5\xC6\xFD\xE0\xC7\xE9\xEF\xE5\xEF\xF8\xD7\xBD\xD3"+0x0D7A3609).s);

	float yawenz = pData->m_Value.m_Float;

	CBaseEntity *pEnt = (CBaseEntity*)pStruct;

	int i = pEnt->GetIndex();

	Vector *pEyeAngles = (Vector*)((DWORD)pEnt + iOffset);

	//I KNOW I HAVE TO NORMALIZE THE ANGLE BUT CBA ATM HEHE
	if(((pEyeAngles->x > 89.0f) && (pEyeAngles->x <= 180.087936f)))
		pEyeAngles->x = 89.0f;

	if((pEyeAngles->x < 271.0f) && (pEyeAngles->x > 180.087936f))
		pEyeAngles->x = 271.0f;

	if ( yawenz > 180.0 )
		yawenz -= 360.0;
	else if ( yawenz < -180.0 )
		yawenz += 360.0;

	if ( fOldYaw[i] > 180.0 )
		fOldYaw[i] -= 360.0;
	else if ( fOldYaw[i] < -180.0 )
		fOldYaw[i] += 360.0;

	float fYawDiff = yawenz - fOldYaw[i];

	if((abs(fYawDiff) > 178.0f && abs(fYawDiff) < 182.0f))		
	{
		yawenz += fYawDiff;

		if ( yawenz > 180.0 )
			yawenz -= 360.0;
		else if ( yawenz < -180.0 )
			yawenz += 360.0;

		fOldYaw[i] = yawenz - fYawDiff;
	}
	else
	{
		fOldYaw[i] = yawenz;
	}

	//yaw/pitch mod which ill implement later when playerlist done
	if(g_Aimbot.fIsSelected[i] != 0)
	{
		if(g_Aimbot.fPitchMod[i] == 1)
			pEyeAngles->x = 271.0f;
		else if(g_Aimbot.fPitchMod[i] == 2)
			pEyeAngles->x = 89.0f;
		else if(g_Aimbot.fPitchMod[i] == 3)
			pEyeAngles->x = 0.0f;

		yawenz -= g_Aimbot.fYawMod[i];

		if ( yawenz > 180.0 )
			yawenz -= 360.0;
		else if ( yawenz < -180.0 )
			yawenz += 360.0;
	}

	*(PFLOAT)pOut = yawenz;
}

void PitchEyeAngleProxy(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	float p = pData->m_Value.m_Float;

	*(PFLOAT)pOut = p;
}



