#include "UMath.h"
#include "Math.h"

KMatrix& MatrixIdentity(KMatrix* Dest)
{
	Dest->SetElements(	1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
	return *Dest;
}

KMatrix MatrixIdentity()
{
	KMatrix Mat;
	MatrixIdentity(&Mat);
	return Mat;
}

KMatrix& MatrixScale(KMatrix* Dest, float Scale)
{
	Dest->SetElements(	Scale,	0.0f,	0.0f,	0.0f,
						0.0f,	Scale,	0.0f,	0.0f,
						0.0f,	0.0f,	Scale,	0.0f,
						0.0f,	0.0f,	0.0f,	1.0f);
	return *Dest;
}

KMatrix MatrixScale(float Scale)
{
	KMatrix Mat;
	MatrixScale(&Mat, Scale);
	return Mat;
}

KMatrix& MatrixScale3D(KMatrix* Dest, KVector Scale)
{
	Dest->SetElements(	Scale.x,	0.0f,		0.0f,		0.0f,
						0.0f,		Scale.y,	0.0f,		0.0f,
						0.0f,		0.0f,		Scale.z,	0.0f,
						0.0f,		0.0f,		0.0f,		1.0f);
	return *Dest;
}

KMatrix MatrixScale3D(KVector Scale)
{
	KMatrix Mat;
	MatrixScale3D(&Mat, Scale);
	return Mat;
}

KMatrix& MatrixTranslate(KMatrix* Dest, KVector Delta)
{
	Dest->SetElements(	1.0f,		0.0f,		0.0f,		0.0f,
						0.0f,		1.0f,		0.0f,		0.0f,
						0.0f,		0.0f,		1.0f,		0.0f,
						Delta.x,	Delta.y,	Delta.z,	1.0f);
	return *Dest;
}

KMatrix MatrixTranslate(KVector Delta)
{
	KMatrix Mat;
	MatrixTranslate(&Mat, Delta);
	return Mat;
}

KMatrix& MatrixPerspectiveProjection(KMatrix* Dest, float NearClip, float FarClip, float HFov, float VFov)
{
	float w = (float)1.0/tan(HFov*0.5);
	float h = (float)1.0/tan(VFov*0.5);
	float Q = FarClip/(FarClip - NearClip);


	Dest->SetElements(	w,		0.0f,		0.0f,			0.0f,
						0.0f,	h,			0.0f,			0.0f,
						0.0f,	0.0f,		Q,				1.0f,
						0.0f,	0.0f,		-Q*NearClip,	0.0f);
	return *Dest;
}

KMatrix MatrixPerspectiveProjection(float NearClip, float FarClip, float HFov, float VFov)
{
	KMatrix Mat;
	MatrixPerspectiveProjection(&Mat, NearClip, FarClip, HFov, VFov);
	return Mat;
}

KMatrix& MatrixQuatRotation(KMatrix* Dest, KQuat R)
{
	Dest->SetElements(
		1.0f - 2.0f*R.y*R.y - 2.0f*R.z*R.z,		2.0f*R.x*R.y - 2.0f*R.w*R.z,		2.0f*R.x*R.z + 2.0f*R.w*R.y,		0,
		2.0f*R.x*R.y + 2.0f*R.w*R.z,			R.w*R.w-R.x*R.x+R.y*R.y-R.z*R.z,	2.0f*R.y*R.z-2.0f*R.w*R.x,			0,
		2.0f*R.x*R.z - 2.0f*R.w*R.y,			2.0f*R.y*R.z - 2.0f*R.w*R.x,		1.0f - 2.0f*R.x*R.x - 2.0f*R.y*R.y,	0,
		0,										0,									0,									1);
	return *Dest;
						
}

KMatrix MatrixQuatRotation(KQuat Rotation)
{
	KMatrix Mat;
	MatrixQuatRotation(&Mat, Rotation);
	return Mat;
}
