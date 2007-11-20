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
	KColorRGB() : Red(0), Green(0), Blue(0)
	{}
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
	KColor() : KColorRGB(0, 0, 0), Alpha(0)
	{}
	float Alphaf()
	{
		return ((float)Alpha)/255;
	}
};

struct KVertexPos
{
	float x;
	float y;
	float z;
	KVertexPos(float X, float Y, float Z) : x(X), y(Y), z(Z)
	{}
	KVertexPos() : x(0.0f), y(0.0f), z(0.0f)
	{}
};

struct KVertexPosTex : public KVertexPos
{
	float u;
	float v;
};

struct KVertex : public KVertexPosTex
{
	KVertexPos Normal;
};

enum
{
	RI_VERTEX_XYZ,
	RI_VERTEX_XYZUV,
	RI_VERTEX_XYZUVN
};

// Clear flags
#define RI_CLEAR_COLOR_BUFFER GL_COLOR_BUFFER_BIT
#define RI_CLEAR_DEPTH_BUFFER GL_DEPTH_BUFFER_BIT
#define RI_CLEAR_ACCUM_BUFFER GL_ACCUM_BUFFER_BIT
#define RI_CLEAR_STENCIL_BUFFER GL_STENCIL_BUFFER_BIT


#define RI_PRIMITIVE_POINTS			GL_POINTS
#define RI_PRIMITIVE_LINES			GL_LINES
#define RI_PRIMITIVE_LINE_LOOP		GL_LINE_LOOP
#define RI_PRIMITIVE_LINE_STRIP		GL_LINE_STRIP
#define RI_PRIMITIVE_TRIANGLES		GL_TRIANGLES
#define RI_PRIMITIVE_TRIANGLE_STRIP	GL_TRIANGLE_STRIP
#define RI_PRIMITIVE_TRIANGLE_FAN	GL_TRIANGLE_FAN
#define RI_PRIMITIVE_QUADS			GL_QUADS
#define RI_PRIMITIVE_QUAD_STRIP		GL_QUAD_STRIP
#define RI_PRIMITIVE_POLYGON		GL_POLYGON


HRenderDevice RICreateContext(HWindowContext WC);
bool RIDestroyDevice(HRenderDevice Dev);
void RIPresent(HRenderDevice Dev);
void RIClear(HRenderDevice Dev, uint ClearFlags, KColor& Clear = KColor(0, 0, 0, 0), float Depth = 1.0f, int Stencil = 0, KColor& Accum = KColor(0, 0, 0, 0));
HVertexBuffer RICreateVertexBuffer(HRenderDevice Dev);
void RISetBufferData(HRenderDevice Dev, HVertexBuffer VBO, void* Data, size_t Size);
void RIBindBuffer(HRenderDevice Dev, HVertexBuffer VBO);
void RIDrawPrimitive(HRenderDevice Dev, uint DrawType, uint StartVertex, uint PrimitiveCount);
void RIDeleteVertexBuffer(HRenderDevice Dev, HVertexBuffer VBO);
void RISetColor(KColor& Color);
void initUrsaGL();

#endif //_RENDERINTERFACE_H_
