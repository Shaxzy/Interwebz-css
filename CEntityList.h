namespace ValveSDK
{
	class CEntityList
	{
	public:
		CBaseEntity* GetClientEntity(int entnum)
		{
			typedef CBaseEntity* (__thiscall* OriginalFn)(PVOID, int);
			return getvfunc<OriginalFn>(this, 3)(this, entnum);
		}
		CBaseEntity* GetClientEntityFromHandle(int hEnt)
		{
			typedef CBaseEntity* (__thiscall* OriginalFn)(PVOID, int);
			return getvfunc<OriginalFn>(this, 4)(this, hEnt);
		}
		int GetHighestEntityIndex(void)
		{
			typedef int(__thiscall* OriginalFn)(PVOID);
			return getvfunc<OriginalFn>(this, 6)(this);
		}
	};
}