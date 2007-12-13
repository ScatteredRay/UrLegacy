#ifndef _RENDERINTERFACE_H_
#define _RENDERINTERFACE_H_
#include "UTypes.h"
#include "UMath.h"

#define USING_DX 1
#define USING_GL 0

#if USING_GL
#include "UrsaGL.h"
#endif //USING_GL

#if USING_DX
#include <d3d9.h>

extern IDirect3D9* gD3D;

#endif //USING_DX

struct HWindowContext
{
	HWND hWnd;
	HWindowContext(HWND Wnd) : hWnd(Wnd)
	{
	}
};

#if USING_GL
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
#endif //USING_GL

#if USING_DX
struct HRenderDevice
{
	IDirect3DDevice9* Device;
	HRenderDevice(IDirect3DDevice9* Dev) : Device(Dev)
	{
	}
	HRenderDevice() : Device(NULL){}
	HRenderDevice(HRenderDevice& Dev)
	{
		Device = Dev.Device;
	}
};
#endif //USING_DX

#if USING_GL
struct HVertexBuffer
{
	uint BufferID;
	HVertexBuffer() : BufferID(0)
	{}
	HVertexBuffer(uint ID) : BufferID(ID)
	{}
};
#endif USING_GL

#if USING_DX
struct HVertexBuffer
{
	IDirect3DVertexBuffer9* VBO;
	HVertexBuffer() : VBO(NULL)
	{}
	HVertexBuffer(IDirect3DVertexBuffer9* vbo) : VBO(vbo)
	{}
};
#endif USING_DX

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
	int AsInt()
	{
		return *((uint*)(&Red));
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
#if USING_GL

#define RI_CLEAR_COLOR_BUFFER GL_COLOR_BUFFER_BIT
#define RI_CLEAR_DEPTH_BUFFER GL_DEPTH_BUFFER_BIT
#define RI_CLEAR_ACCUM_BUFFER GL_ACCUM_BUFFER_BIT
#define RI_CLEAR_STENCIL_BUFFER GL_STENCIL_BUFFER_BIT


#define RI_PRIMITIVE_POINTS			GL_POINTS
#define RI_PRIMITIVE_LINES			GL_LINES
#define RI_PRIMITIVE_LINE_STRIP		GL_LINE_STRIP
#define RI_PRIMITIVE_TRIANGLES		GL_TRIANGLES
#define RI_PRIMITIVE_TRIANGLE_STRIP	GL_TRIANGLE_STRIP
#define RI_PRIMITIVE_TRIANGLE_FAN	GL_TRIANGLE_FAN

#endif //USING_GL

#if USING_DX

#define RI_CLEAR_COLOR_BUFFER D3DCLEAR_TARGET
#define RI_CLEAR_DEPTH_BUFFER D3DCLEAR_ZBUFFER
#define RI_CLEAR_ACCUM_BUFFER
#define RI_CLEAR_STENCIL_BUFFER D3DCLEAR_STENCIL


#define RI_PRIMITIVE_POINTS			D3DPT_POINTLIST
#define RI_PRIMITIVE_LINES			D3DPT_LINELIST
#define RI_PRIMITIVE_LINE_STRIP		D3DPT_LINESTRIP
#define RI_PRIMITIVE_TRIANGLES		D3DPT_TRIANGLELIST
#define RI_PRIMITIVE_TRIANGLE_STRIP	D3DPT_TRIANGLESTRIP
#define RI_PRIMITIVE_TRIANGLE_FAN	D3DPT_TRIANGLEFAN

#define RI_TRANSFORM_VIEW		D3DTS_VIEW
#define RI_TRANSFORM_PROJECTION	D3DTS_PROJECTION
#endif //USING_DX


HRenderDevice RICreateContext(HWindowContext WC);
bool RIDestroyDevice(HRenderDevice Dev);
void RIBeginScene(HRenderDevice Dev);
void RIEndScene(HRenderDevice Dev);
void RIPresent(HRenderDevice Dev);
void RIClear(HRenderDevice Dev, uint ClearFlags, KColor& Clear = KColor(0, 0, 0, 0), float Depth = 1.0f, int Stencil = 0, KColor& Accum = KColor(0, 0, 0, 0));
HVertexBuffer RICreateVertexBuffer(HRenderDevice Dev, size_t Size);
void RISetBufferData(HRenderDevice Dev, HVertexBuffer VBO, void* Data, size_t Size);
void RIBindBuffer(HRenderDevice Dev, HVertexBuffer VBO);
void RIDrawPrimitive(HRenderDevice Dev, uint DrawType, uint StartVertex, uint PrimitiveCount);
void RIDeleteVertexBuffer(HRenderDevice Dev, HVertexBuffer VBO);
void RISetColor(HRenderDevice Dev, KColor& Color);
void RISetMatrix(HRenderDevice Dev, uint TransformState, Matrix4& Transform);
#if USING_GL
void initUrsaGL();
#endif //USING_GL

#endif //_RENDERINTERFACE_H_
