namespace ValveSDK
{
	class CGameMovement
	{
	public:
		void ProcessMovement(CBaseEntity *pPlayer, PVOID moveData)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *pPlayer, PVOID moveData);
			getvfunc<OriginalFn>(this, 1)(this, pPlayer, moveData);
		}
	};
}