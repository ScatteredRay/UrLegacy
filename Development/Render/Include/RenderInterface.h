#ifndef _RENDERINTERFACE_H_
#define _RENDERINTERFACE_H_
#include "UrsaGL.h"
#include "UTypes.h"

struct HWindowContext
{
	HDC hdc;
	HWindowContext(HDC hDC) : hdc(hDC)
	{
	}
};

struct HRenderDevice
{
	HGLRC context;
	HDC hdc; //TODO: quick hack, fixme.
	HRenderDevice(HGLRC glrc, HDC hDC) : context(glrc), hdc(hDC)
	{
	}
	HRenderDevice() : context(NULL), hdc(NULL) {}
	HRenderDevice(HRenderDevice& Dev)
	{
		context = Dev.context;
		hdc = Dev.hdc;
	}
};

struct HVertexBuffer
{
	uint BufferID;
};

struct KColorRGB
{
	uint8 Red;
	uint8 Green;
	uint8 Blue;
	KColorRGB(uint8 R, uint8 G, uint8 B) : Red(R), Green(G), Blue(B)
	{}
	KColorRGB(int R, int G, int B) : Red(R), Green(G), Blue(B)
	{}
	KColorRGB(float R, float G, float B)
	{
		Red = (int8)(R*255);
		Green = (int8)(G*255);
		Blue = (int8)(B*255);
	}
	float Redf()
	{
		return ((float)Red)/255;
	}
	float Greenf()
	{
		return ((float)Green)/255;
	}
	float Bluef()
	{
		return ((float)Blue)/255;
	}
};

struct KColor : public KColorRGB
{
	uint8 Alpha;
	KColor(uint8 R, uint8 G, uint8 B, uint8 A) : KColorRGB(R, G, B), Alpha(A)
	{}
	KColor(int R, int G, int B, int A) : KColorRGB(R, G, B), Alpha(A)
	{}
	KColor(float R, float G, float B, float A) : KColorRGB(R, G, B)
	{
		Alpha = (int8)(A*255);
	}
	float Alphaf()
	{
		return ((float)Alpha)/255;
	}
};

// Clear flags
#define RI_CLEAR_COLOR_BUFFER GL_COLOR_BUFFER_BIT
#define RI_CLEAR_DEPTH_BUFFER GL_DEPTH_BUFFER_BIT
#define RI_CLEAR_ACCUM_BUFFER GL_ACCUM_BUFFER_BIT
#define RI_CLEAR_STENCIL_BUFFER GL_STENCIL_BUFFER_BIT

HRenderDevice RICreateContext(HWindowContext WC);
bool RIDestroyDevice(HRenderDevice Dev);
void RIPresent(HRenderDevice Dev);
void RIClear(HRenderDevice Dev, uint ClearFlags, KColor& Clear = KColor(0, 0, 0, 0), float Depth = 1.0f, int Stencil = 0, KColor& Accum = KColor(0, 0, 0, 0));
HVertexBuffer RICreateVertexBuffer(HRenderDevice Dev);
void RIDeleteVertexBuffer(HRenderDevice Dev, HVertexBuffer VBO);
void initUrsaGL();

#endif //_RENDERINTERFACE_H_
