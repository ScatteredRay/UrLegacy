#ifndef _UMATH_H_
#define _UMATH_H_
#include "UPlatform.h"

struct KVector
{
	float x;
	float y;
	float z;
	KVector(float X, float Y, float Z) : x(X), y(Y), z(Z)
	{}
	KVector() : x(0.0f), y(0.0f), z(0.0f)
	{}
};

struct KMatrix
{
	float Mat[4][4];
	KMatrix()
	{
		 appMemzero(Mat, sizeof(float) * 16);
	}
	KMatrix(float* Ary)
	{
		appMemcpy(Mat, Ary, sizeof(float) * 16);
	}
	KMatrix(const KMatrix& O)
	{
		appMemcpy(Mat, O.Mat, sizeof(float) * 16);
	}
	KMatrix(float e0, float e1, float e2, float e3,
			float e4, float e5, float e6, float e7,
			float e8, float e9, float e10, float e11,
			float e12, float e13, float e14, float e15)
	{
		Mat[0][0] = e0;
		Mat[0][1] = e1;
		Mat[0][2] = e2;
		Mat[0][3] = e3;
		Mat[1][0] = e4;
		Mat[1][1] = e5;
		Mat[1][2] = e6;
		Mat[1][3] = e7;
		Mat[2][0] = e8;
		Mat[2][1] = e9;
		Mat[2][2] = e10;
		Mat[2][3] = e11;
		Mat[3][0] = e12;
		Mat[3][1] = e13;
		Mat[3][2] = e14;
		Mat[3][3] = e15;
	}
	void SetElements(float e0, float e1, float e2, float e3,
			float e4, float e5, float e6, float e7,
			float e8, float e9, float e10, float e11,
			float e12, float e13, float e14, float e15)
	{
		Mat[0][0] = e0;
		Mat[0][1] = e1;
		Mat[0][2] = e2;
		Mat[0][3] = e3;
		Mat[1][0] = e4;
		Mat[1][1] = e5;
		Mat[1][2] = e6;
		Mat[1][3] = e7;
		Mat[2][0] = e8;
		Mat[2][1] = e9;
		Mat[2][2] = e10;
		Mat[2][3] = e11;
		Mat[3][0] = e12;
		Mat[3][1] = e13;
		Mat[3][2] = e14;
		Mat[3][3] = e15;
	}
	float* operator[](int index)
	{
		return Mat[index];
	}
	KMatrix& operator=(const KMatrix& Other)
	{
		appMemcpy(Mat, Other.Mat, sizeof(float) * 16);
		return *this;
	}
	KMatrix operator*(const KMatrix& Other) const
	{
		KMatrix NewMat;
		for(uint i=0; i<4; i++)
		{
			for(uint j=0; j<4; j++)
			{
				for(uint k=0; k<4; k++)
					NewMat.Mat[i][j] += Mat[i][k] * Other.Mat[k][j];
			}
		}
		return NewMat;
	}
	KMatrix operator*=(const KMatrix& Other)
	{
		KMatrix NewMat;
		for(uint i=0; i<4; i++)
		{
			for(uint j=0; j<4; j++)
			{
				for(uint k=0; k<4; k++)
					NewMat.Mat[i][j] += Mat[i][k] * Other.Mat[k][j];
			}
		}
		return NewMat;
	}
};

#define UR_PI ((FLOAT)3.141592654f)

KMatrix& MatrixIdentity(KMatrix* Dest);
KMatrix MatrixIdentity();
KMatrix& MatrixScale(KMatrix* Dest, float Scale);
KMatrix MatrixScale(float Scale);
KMatrix& MatrixScale3D(KMatrix* Dest, KVector Scale);
KMatrix MatrixScale3D(KVector Scale);
KMatrix& MatrixTranslate(KMatrix* Dest, KVector Delta);
KMatrix MatrixTranslate(KVector Delta);
KMatrix& MatrixPerspectiveProjection(KMatrix* Dest, float NearClip, float FarClip, float HFov, float VFov);
KMatrix MatrixPerspectiveProjection(float NearClip, float FarClip, float HFov, float VFov);

#endif //UMATH_H_
