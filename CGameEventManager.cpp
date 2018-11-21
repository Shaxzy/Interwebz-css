#include "DllMain.h"

cGameEventManager gGameEventManager;
std::vector<healthInfo_t>g_healthInfo;

int getZahlAnStelle(const int zahl, const int stelle)
{
	int retValue = int(zahl / pow(10.0, int(log10(double(zahl)) - (stelle - 1)))) % 10;
	if (retValue < 0)
		retValue = 0;
	else if (zahl < 10)
		retValue = 1;
	return retValue;
}

void cGameEventManager::FireGameEvent(ValveSDK::IGameEvent *event)
{
	const char* szEventName = event->GetName();

	static std::string sXor = /*player_hurt*/XorStr<0x7D,12,0x427069F4>("\x0D\x12\x1E\xF9\xE4\xF0\xDC\xEC\xF0\xF4\xF3"+0x427069F4).s;
	static std::string sXor2 = /*player_death*/XorStr<0x72,13,0x2C0196D0>("\x02\x1F\x15\x0C\x13\x05\x27\x1D\x1F\x1A\x08\x15"+0x2C0196D0).s;
	static std::string sXor3 = /*round_start*/XorStr<0x76,12,0xE9B186D0>("\x04\x18\x0D\x17\x1E\x24\x0F\x09\x1F\x0D\xF4"+0xE9B186D0).s;
	static std::string sXor4 = /*attacker*/XorStr<0x3B,9,0x53678E3F>("\x5A\x48\x49\x5F\x5C\x2B\x24\x30"+0x53678E3F).s;
	static std::string sXor5 = /*userid*/XorStr<0x76,7,0x9CB96FC6>("\x03\x04\x1D\x0B\x13\x1F"+0x9CB96FC6).s;
	static std::string sXor6 = /*health*/XorStr<0xB5,7,0x0F3F2A4E>("\xDD\xD3\xD6\xD4\xCD\xD2"+0x0F3F2A4E).s;
	static std::string sXor7 = /*dmg_health*/XorStr<0x2B,11,0xC337D88F>("\x4F\x41\x4A\x71\x47\x55\x50\x5E\x47\x5C"+0xC337D88F).s;
	static std::string sXor8 = /*I did %i damage to %s [%i -> %i] OldFirstDigit: %i NewFirstDigit: %i*/XorStr<0x9E,69,0xC7A6549B>("\xD7\xBF\xC4\xC8\xC6\x83\x81\xCC\x86\xC3\xC9\xC4\xCB\xCC\xC9\x8D\xDA\xC0\x90\x94\xC1\x93\xEF\x90\xDF\x97\x95\x87\x9A\x9E\xD5\xE0\x9E\xF0\xAC\xA5\x84\xAA\xB6\xB6\xB2\x83\xA1\xAE\xA3\xBF\xF6\xED\xEB\xA6\xF0\x9F\xB7\xA4\x92\xBC\xA4\xA4\xAC\x9D\xB3\xBC\xB5\xA9\xE4\xFF\xC5\x88"+0xC7A6549B).s;

	/*
	short   userid		user ID who was hurt
	short	attacker	user ID who attacked
	byte	health		remaining health points
	byte	armor		remaining armor points
	string	weapon		weapon name attacker used, if not the world
	short	dmg_health	damage done to health
	byte	dmg_armor	damage done to armor
	byte	hitgroup	hitgroup that was damaged
	*/

	if (strcmp(szEventName, sXor.data()) == 0)
	{
		char szString[200];

		if (event->GetInt(sXor4.data(), 0) - 1 == g_Valve.pEngine->GetLocalPlayer())
		{
			ValveSDK::CEngineClient::player_info_t info;
			if (!g_Valve.pEngine->GetPlayerInfo(event->GetInt(sXor5.data(), 0) - 1, &info))
				return;

			// complicated part XDDDD
			int oldHealth = event->GetInt(sXor6.data(), 100) + event->GetInt(sXor7.data(), 100);
			int newHealth = event->GetInt(sXor6.data(), 100);

			int oldFirstDigit = getZahlAnStelle(oldHealth, 1);
			int newFirstDigit = getZahlAnStelle(newHealth, 1);

			//if ( oldFirstDigit == 1 && oldHealth == 100 && oldHealth != newHealth ) // take care of 100 hp XDD
			// 10 healthbars, jeweils 10 hp
			//if (newHealth < 91 && oldFirstDigit != newFirstDigit)
			//{
			// von 100 auf 90 -> healthbar nummer 10 removen
			// von 90 auf 80 -> healthbar nummer 9 removen
			// von 70 auf 50 -> healthbar nummer 7 und 8 removen
			// etc..

			int healthbarRemovedNum = 0;
			if (newHealth < oldHealth)
			{
				//int numberOfRemovedHealthbars = getZahlAnStelle(oldHealth - newHealth, 1);
			}
			// i removed da fuckin healthbar XDDD

			bool foundInList = false;
			int i = 0;
			for (i = 0; i < g_healthInfo.size(); i++)
			{
				if (g_healthInfo[i].targetId == event->GetInt(sXor5.data(), 0))
				{
					g_healthInfo[i].damage += event->GetInt(sXor7.data(), 100);
					g_healthInfo[i].animTimer = GetTickCount();
					foundInList = true;
					break;
				}
			}

			if (!foundInList)
			{
				healthInfo_t pHealthDummy;
				pHealthDummy.damage = event->GetInt(sXor7.data(), 100);
				pHealthDummy.targetId = event->GetInt(sXor5.data(), 0);
				pHealthDummy.animTimer = GetTickCount();
				g_healthInfo.push_back(pHealthDummy);
			}
			/*
			pHealthDummy.damage = event->GetInt("dmg_health", 100);
			pHealthDummy.targetId = event->GetInt("userid", 0);
			pHealthDummy.animTimer = GetTickCount();
			g_healthInfo.push_back(pHealthDummy);
			*/
			//}
			// complicated part XDDDD

#ifdef DEBUGMODE
			sprintf(szString, sXor8.data(), event->GetInt(sXor7.data(), 100), info.name, oldHealth, newHealth, oldFirstDigit, newFirstDigit);
			Base::Debug::LOG(szString);
#endif
		}
	}

	/*
	short	userid		user ID who died
	short	attacker	user ID who killed
	short	assister	user ID who assisted in the kill
	bool	headshot	singals a headshot
	*/

	if (strcmp(szEventName, sXor2.data()) == 0)
	{
		for (int i = 0; i < g_healthInfo.size(); i++)
		{
			if (g_healthInfo[i].targetId == event->GetInt(sXor5.data(), 0))
				g_healthInfo[i].animTimer = 0, g_healthInfo[i].damage = 0, g_healthInfo[i].targetId = 0;
		}
	}

	if (strcmp(szEventName, sXor3.data()) == 0)
	{
		g_healthInfo.clear();
	}

}

void cGameEventManager::RegisterSelf()
{
	static std::string sXor = /*player_hurt*/XorStr<0x7D,12,0x427069F4>("\x0D\x12\x1E\xF9\xE4\xF0\xDC\xEC\xF0\xF4\xF3"+0x427069F4).s;
	static std::string sXor2 = /*player_death*/XorStr<0x72,13,0x2C0196D0>("\x02\x1F\x15\x0C\x13\x05\x27\x1D\x1F\x1A\x08\x15"+0x2C0196D0).s;
	static std::string sXor3 = /*round_start*/XorStr<0x76,12,0xE9B186D0>("\x04\x18\x0D\x17\x1E\x24\x0F\x09\x1F\x0D\xF4"+0xE9B186D0).s;

	g_Valve.pGameEventManager->AddListener(this, sXor.data(), false);
	g_Valve.pGameEventManager->AddListener(this, sXor2.data(), false);
	g_Valve.pGameEventManager->AddListener(this, sXor3.data(), false);
}





