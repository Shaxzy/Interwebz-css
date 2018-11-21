#include "DllMain.h"



VOID CMath::sinCos(float radians, PFLOAT sine, PFLOAT cosine)
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

VOID CMath::angleVectors(PFLOAT angles, PFLOAT f, PFLOAT r, PFLOAT u)
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

float CMath::sseSqrt(float x)
{
	float root = 0.0f;

	__asm
	{
		sqrtss xmm0, x
			movss root, xmm0
	}

	return root;
}

void CMath::vectorAngles(float* forward, float* angles)
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

float CMath::AngleNormalize(float angle)
{
	while (angle < -180)    angle += 360;
	while (angle > 180)    angle -= 360;

	return angle;
}

float CMath::DotProduct(const float *v1, const float *v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

VOID CMath::VectorTransform(const Vector in1, const matrix3x4_t& in2, Vector& out)
{
	float buf[3];
	buf[0] = in1.x;
	buf[1] = in1.y;
	buf[2] = in1.z;

	out[0] = DotProduct(buf, in2[0]) + in2[0][3];
	out[1] = DotProduct(buf, in2[1]) + in2[1][3];
	out[2] = DotProduct(buf, in2[2]) + in2[2][3];
}




