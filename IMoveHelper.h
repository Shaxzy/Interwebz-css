namespace ValveSDK
{
	class ImoveHelper
	{
	public:
		void SetHost(CBaseEntity *pPlayer)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *pPlayer);
			getvfunc<OriginalFn>(this, 1)(this, pPlayer);
		}
	};
}