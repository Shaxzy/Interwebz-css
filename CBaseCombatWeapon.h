enum
{
	WEAPON_NULL,
	WEAPON_P228,
	WEAPON_GLOCK,
	WEAPON_SCOUT,
	WEAPON_HE_GRENADE,
	WEAPON_XM1014,
	WEAPON_C4,
	WEAPON_MAC10,
	WEAPON_AUG,
	WEAPON_SMOKE_GRENADE,
	WEAPON_ELITES,
	WEAPON_FIVESEVEN,
	WEAPON_UMP,
	WEAPON_SG550,
	WEAPON_GALIL,
	WEAPON_FAMAS,
	WEAPON_USP,
	WEAPON_AWP,
	WEAPON_MP5,
	WEAPON_M249,
	WEAPON_M3,
	WEAPON_M4A1,
	WEAPON_TMP,
	WEAPON_G3SG1,
	WEAPON_FLASHBANG,
	WEAPON_DEAGLE,
	WEAPON_SG552,
	WEAPON_AK47,
	WEAPON_KNIFE,
	WEAPON_P90
};

namespace ValveSDK
{
	class CBaseCombatWeapon
	{
	public:
		float GetNextPrimaryAttack()
		{
			static int iOffset = g_NetworkedVariableManager.GetOffset(/*DT_BaseCombatWeapon*/XorStr<0x0C,20,0xCCB89A4B>("\x48\x59\x51\x4D\x71\x62\x77\x50\x7B\x78\x74\x76\x6C\x4E\x7F\x7A\x6C\x72\x70"+0xCCB89A4B).s, /*m_flNextPrimaryAttack*/XorStr<0x0D,22,0x423C055C>("\x60\x51\x69\x7C\x5F\x77\x6B\x60\x45\x64\x7E\x75\x78\x68\x62\x5D\x69\x6A\x7E\x43\x4A"+0x423C055C).s);
			return *(float*)((DWORD)this + iOffset);
		}

		bool HasAmmo()
		{
			static int iOffset = g_NetworkedVariableManager.GetOffset(/*DT_BaseCombatWeapon*/XorStr<0x5E,20,0xBED67F8D>("\x1A\x0B\x3F\x23\x03\x10\x01\x26\x09\x0A\x0A\x08\x1E\x3C\x09\x0C\x1E\x00\x1E"+0xBED67F8D).s,/*m_iClip1*/XorStr<0x7B,9,0xEBD79649>("\x16\x23\x14\x3D\x13\xE9\xF1\xB3"+0xEBD79649).s);
			return ((*(PINT)((DWORD)this + iOffset)) > 0);
		}

		bool IsSilencerOn()
		{
			static int iOffsetPistol = g_NetworkedVariableManager.GetOffset(/*DT_WeaponUSP*/XorStr<0x88,13,0xD9DE2C14>("\xCC\xDD\xD5\xDC\xE9\xEC\xFE\xE0\xFE\xC4\xC1\xC3"+0xD9DE2C14).s,/*m_bSilencerOn*/XorStr<0xFD,14,0x2902C67C>("\x90\xA1\x9D\x53\x68\x6E\x66\x6A\x66\x63\x75\x47\x67"+0x2902C67C).s);
			static int iOffsetRifle = g_NetworkedVariableManager.GetOffset(/*DT_WeaponM4A1*/XorStr<0x80,14,0x78EF406B>("\xC4\xD5\xDD\xD4\xE1\xE4\xF6\xE8\xE6\xC4\xBE\xCA\xBD"+0x78EF406B).s,/*m_bSilencerOn*/XorStr<0xFD,14,0x2902C67C>("\x90\xA1\x9D\x53\x68\x6E\x66\x6A\x66\x63\x75\x47\x67"+0x2902C67C).s);
		
			int iWeaponID = GetWeaponID();

			if(iWeaponID == WEAPON_USP)
				return (*(PBOOL)((DWORD)this + iOffsetPistol));
			else if(iWeaponID == WEAPON_M4A1)
				return (*(PBOOL)((DWORD)this + iOffsetRifle));

			return false;
		}

		float GetSpread()
		{
			typedef float(__thiscall *GetSpread_t)(PVOID);
			return ((GetSpread_t)(*(PDWORD)(*(PDWORD)(this) + GETSPREADOFFSET)))(this);
		}

		float GetCone()
		{
			typedef float(__thiscall *GetCone_t)(PVOID);
			return ((GetCone_t)(*(PDWORD)(*(PDWORD)(this) + GETCONEOFFSET)))(this);
		}

		void UpdateAccuracyPenalty()
		{
			typedef void(__thiscall *UpdateAccuracyPenalty_t)(PVOID);
			((UpdateAccuracyPenalty_t)(*(PDWORD)(*(PDWORD)(this) + UPDATEACCURACYPENALTYOFFSET)))(this);
		}

		int GetWeaponID()
		{
			typedef int(__thiscall *GetWeaponID_t)(PVOID);
			return ((GetWeaponID_t)(*(PDWORD)(*(PDWORD)(this) + WEAPONIDOFFSET)))(this);
		}

		bool IsMiscWeapon()
		{
			int iWeaponID = GetWeaponID();

			return (iWeaponID == WEAPON_KNIFE || iWeaponID == WEAPON_C4 
				|| iWeaponID == WEAPON_HE_GRENADE || iWeaponID == WEAPON_SMOKE_GRENADE
				|| iWeaponID == WEAPON_FLASHBANG);
		}
	};
}