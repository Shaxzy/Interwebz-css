#include "DllMain.h"

CMath g_Math;

void sinCos(float radians, PFLOAT sine, PFLOAT cosine)
{
	__asm
	{
		fld dword ptr[radians]
		fsincos
		mov edx, dword ptr[cosine]
		mov eax, dword ptr[sine]
		fstp dword ptr[edx]
		fstp dword ptr[eax]
	}
}

#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define square( x ) ( x * x )

void CNoSpread::angleVectors(Vector angles, Vector &f, Vector &r, Vector &u)
{
	float sp, sy, sr, cp, cy, cr;

	sinCos(DEG2RAD(angles[0]), &sp, &cp);
	sinCos(DEG2RAD(angles[1]), &sy, &cy);
	sinCos(DEG2RAD(angles[2]), &sr, &cr);

	f[0] = cp * cy;
	f[1] = cp * sy;
	f[2] = -sp;

	r[0] = -1.0f * sr * sp * cy + -1.0f * cr * -sy;
	r[1] = -1.0f * sr * sp * sy + -1.0f * cr * cy;
	r[2] = -1.0f * sr * cp;

	u[0] = cr * sp * cy + -sr * -sy;
	u[1] = cr * sp * sy + -sr * cy;
	u[2] = cr * cp;
}

void CNoSpread::angleVectors(Vector angles, Vector &f)
{
	float sp, sy, sr, cp, cy, cr;

	sinCos(DEG2RAD(angles[0]), &sp, &cp);
	sinCos(DEG2RAD(angles[1]), &sy, &cy);
	sinCos(DEG2RAD(angles[2]), &sr, &cr);

	f[0] = cp * cy;
	f[1] = cp * sy;
	f[2] = -sp;
}

float sseSqrt(float x)
{
	float root = 0.0f;

	__asm
	{
		sqrtss xmm0, x
		movss root, xmm0
	}

	return root;
}

void CNoSpread::vectorAngles(Vector forward, Vector &angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
		angles[1] = 0.0f;
	}
	else
	{
		float len2d = sseSqrt(square(forward[0]) + square(forward[1]));

		angles[0] = RAD2DEG(atan2f(-forward[2], len2d));
		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));

		if (angles[0] < 0.0f) angles[0] += 360.0f;
		if (angles[1] < 0.0f) angles[1] += 360.0f;
	}

	angles[2] = 0.0f;
}

void VectorAngles(const Vector &forward, const Vector &pseudoup, Vector &angles)
{
	Vector left;

	left = CrossProduct( pseudoup, forward );
	left.NormalizeInPlace();		
	
	float xyDist = sseSqrt( forward[0] * forward[0] + forward[1] * forward[1] );

	if ( xyDist > 0.001f )
	{
		angles[1] = RAD2DEG( atan2f( forward[1], forward[0] ) );
		angles[0] = RAD2DEG( atan2f( -forward[2], xyDist ) );

		float up_z = (left[1] * forward[0]) - (left[0] * forward[1]);

		angles[2] = RAD2DEG( atan2f( left[2], up_z ) );
	}
	else
	{
		angles[1] = RAD2DEG( atan2f( -left[0], left[1] ) );
		angles[0] = RAD2DEG( atan2f( -forward[2], xyDist ) );
		angles[2] = 0;
	}	
}

float CNoSpread::AngleNormalize(float angle)
{
	while (angle < -180)    angle += 360;
	while (angle > 180)    angle -= 360;

	return angle;
}

void CNoSpread::RandomSeed(int iSeed)
{
	typedef void(__cdecl* RandomSeed_t)(int);
	static RandomSeed_t pRandomSeed = (RandomSeed_t)(GetProcAddress(GetModuleHandle(/*vstdlib*/XorStr<0x54,8,0xFE0933A5>("\x22\x26\x22\x33\x34\x30\x38"+0xFE0933A5).s), /*RandomSeed*/XorStr<0xFB,11,0x36C577CD>("\xA9\x9D\x93\x9A\x90\x6D\x52\x67\x66\x60"+0x36C577CD).s));
	pRandomSeed(iSeed);
}

float CNoSpread::RandomFloat(float fMin, float fMax)
{
	typedef float(__cdecl* RandomFloat_t)(float, float);
	static RandomFloat_t pRandomFloat = (RandomFloat_t)(GetProcAddress(GetModuleHandle(/*vstdlib*/XorStr<0x54,8,0xFE0933A5>("\x22\x26\x22\x33\x34\x30\x38"+0xFE0933A5).s), /*RandomFloat*/XorStr<0x79,12,0x49861C66>("\x2B\x1B\x15\x18\x12\x13\x39\xEC\xEE\xE3\xF7"+0x49861C66).s));
	return pRandomFloat(fMin, fMax);
}

void CNoSpread::GetRollSpreadFix(ValveSDK::CBaseCombatWeapon *m_pWeapon, UINT seed, Vector& pflInAngles)
{
	pflInAngles.x = AngleNormalize(pflInAngles.x);
	pflInAngles.y = AngleNormalize(pflInAngles.y);

	RandomSeed((seed & 0xFF) + 1);
	float flA = RandomFloat(0.0f, 6.283185f);
	float flB = RandomFloat(0.0f, m_pWeapon->GetSpread());
	float flC = RandomFloat(0.0f, 6.283185f);
	float flD = RandomFloat(0.0f, m_pWeapon->GetCone());

	Vector vForward,vRight,vUp,vDir;
	Vector vView,vSpread, flIdentity[3];
	float flRoll, flCross;

	vSpread.x = (cos(flA) * flB) + (cos(flC) * flD);
	vSpread.y = (sin(flA) * flB) + (sin(flC) * flD);

	Vector qViewAngles = pflInAngles;
	
	vSpread[0] = -vSpread[0];
	vSpread[1] = -vSpread[1];

	angleVectors( qViewAngles, vForward, vRight, vUp );

	vDir[0] = vForward[0] + ( vRight[0] * vSpread[0] ) + ( vUp[0] * vSpread[1] );
	vDir[1] = vForward[1] + ( vRight[1] * vSpread[0] ) + ( vUp[1] * vSpread[1] );
	vDir[2] = vForward[2] + ( vRight[2] * vSpread[0] ) + ( vUp[2] * vSpread[1] );

	vDir.NormalizeInPlace();

	flIdentity[2][0] = 1.0f;
	flIdentity[2][1] = -vSpread[0];
	flIdentity[2][2] = vSpread[1];

	flIdentity[2].NormalizeInPlace();

	flIdentity[0][0] = 0.0f;
	flIdentity[0][1] = -vSpread[0];
	flIdentity[0][2] = ( 1.0f / vSpread[1] ) + ( 1.0f / flIdentity[2][2] ) + vSpread[1];

	if( vSpread[0] > 0.0f && vSpread[1] < 0.0f )
	{
		if( flIdentity[0][1] < 0.0f )
			flIdentity[0][1] = -flIdentity[0][1];
	}
	else if( vSpread[0] < 0.0f && vSpread[1] < 0.0f )
	{
		if( flIdentity[0][1] > 0.0f )
			flIdentity[0][1] = -flIdentity[0][1];
	}

	if( flIdentity[0][2] < 0.0f )
		flIdentity[0][2] = -flIdentity[0][2];

	flIdentity[0].NormalizeInPlace();

	flIdentity[1] = CrossProduct( flIdentity[0], flIdentity[2] );
	flIdentity[1].NormalizeInPlace();

	flCross = ( flIdentity[1][1] * flIdentity[2][0] ) - ( flIdentity[1][0] * flIdentity[2][1] );

	if( qViewAngles[0] > 84.0f || qViewAngles[0] < -84.0f )
		flRoll = RAD2DEG( atan2f( flIdentity[1][2], sseSqrt( flCross ) ) );
	else
		flRoll = RAD2DEG( atan2f( flIdentity[1][2], flCross ) );

	if( flRoll < 0.0f )
		flRoll += 360.0f;

	VectorAngles( vDir, vUp, pflInAngles );
	pflInAngles[2] += flRoll;	

	pflInAngles.x = AngleNormalize(pflInAngles.x);
	pflInAngles.y = AngleNormalize(pflInAngles.y);
}

void CNoSpread::GetSpreadFix(ValveSDK::CBaseCombatWeapon *m_pWeapon, UINT seed, Vector& pflInAngles)
{
	pflInAngles.x = AngleNormalize(pflInAngles.x);
	pflInAngles.y = AngleNormalize(pflInAngles.y);

	RandomSeed((seed & 0xFF) + 1);
	float flA = RandomFloat(0.0f, 6.283185f);
	float flB = RandomFloat(0.0f, m_pWeapon->GetSpread());
	float flC = RandomFloat(0.0f, 6.283185f);
	float flD = RandomFloat(0.0f, m_pWeapon->GetCone());

	Vector vForward,vRight,vUp,vDir;
	Vector vView,vSpread;

	vSpread.x = (cos(flA) * flB) + (cos(flC) * flD);
	vSpread.y = (sin(flA) * flB) + (sin(flC) * flD);

	Vector qAng = pflInAngles;

	Vector qDifference;
	Vector EstimatedViewAngles;

	float fDiff = 0.0f;
	float fDiffOld = 180.0f;
	
	for ( ;; )
	{
		angleVectors(pflInAngles,vForward,vRight,vUp);

		vDir.x = vForward.x + vSpread.x * vRight.x + vSpread.y * vUp.x;
		vDir.y = vForward.y + vSpread.x * vRight.y + vSpread.y * vUp.y;
		vDir.z = vForward.z + vSpread.x * vRight.z + vSpread.y * vUp.z;

		vView.x = 8192.0f * vDir.x;
		vView.y = 8192.0f * vDir.y;
		vView.z = 8192.0f * vDir.z;

		vectorAngles(vView,EstimatedViewAngles);
		
		EstimatedViewAngles.x = AngleNormalize(EstimatedViewAngles.x);
		EstimatedViewAngles.y = AngleNormalize(EstimatedViewAngles.y);

		qDifference = qAng - EstimatedViewAngles;
		
		qDifference.x = AngleNormalize(qDifference.x);
		qDifference.y = AngleNormalize(qDifference.y);

		fDiff = sqrt((qDifference.x * qDifference.x) + (qDifference.y * qDifference.y));

		if((fDiff <= 0.001f) || (fDiff >= fDiffOld))
			break;

		fDiffOld = fDiff;

		pflInAngles += qDifference;

		pflInAngles.x = AngleNormalize(pflInAngles.x);
		pflInAngles.y = AngleNormalize(pflInAngles.y);
	}
}








