class CNoSpread
{
public:	
	void GetSpreadFix(ValveSDK::CBaseCombatWeapon *m_pWeapon, UINT seed, Vector& pflInAngles);
	void GetRollSpreadFix(ValveSDK::CBaseCombatWeapon *m_pWeapon, UINT seed, Vector& pflInAngles);

	void angleVectors(Vector angles, Vector &f, Vector &r, Vector &u);
	void angleVectors(Vector angles, Vector &f);
	void vectorAngles(Vector forward, Vector &angles);
	float AngleNormalize(float angle);
	void RandomSeed(int iSeed);
	float RandomFloat(float fMin, float fMax);
};
extern CNoSpread g_NoSpread;