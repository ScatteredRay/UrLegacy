#include "RenderInterface.h" 
#include "Core.h"
#include "UTypes.h"
#include "UrsaGL.h"

struct HWindowContext
{
	HDC hdc;
};

struct HRenderDevice
{
	HGLRC context;
	HRenderDevice(HGLRC glrc) : context(glrc)
	{
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
	KColorRGB(float R, float G, float B)
	{
		Red = (int8)(R*255);
		Green = (int8)(G*255);
		Blue = (int8)(B*255);
	}
	float Redf()
	{
		return ((float)Red)/255);
	}
	float Greenf()
	{
		return ((float)Green)/255);
	}
	float Bluef()
	{
		return ((float)Blue)/255);
	}
};

struct KColor : public KColorRGB
{
	uint8 Alpha;
	KColor(uint8 R, uint8 G, uint8 B, uint8 A) : KColorRGB(R, G, B), Alpha(A)
	{}
	KColor(float R, float G, float B, float A) : KColorRGB(R, G, B)
	{
		Alpha = (int8)(A*255);
	}
	float Alphaf()
	{
		return ((float)Alpha)/255);
	}
};

// Clear flags
#define RI_CLEAR_COLOR_BUFFER GL_COLOR_BUFFER_BIT
#define RI_CLEAR_DEPTH_BUFFER GL_DEPTH_BUFFER_BIT
#define RI_CLEAR_ACCUM_BUFFER GL_ACCUM_BUFFER_BIT
#define RI_CLEAR_STENCIL_BUFFER GL_STENCIL_BUFFER_BIT

HRenderDevice RICreateContext(HWindowContext WC)
{
	HRenderDevice Dev(wglCreateContext(WC.hdc));
	wglMakeCurrent(WC.hdc, Dev.context); // in GL we assume only one device per thread, so we just go ahead and make it current.
	return Dev;
}

bool RIDeleteDevice(HRenderDevice Dev)
{
	return wglDeleteContext(Dev.context);
}

//TODO: break the optionals into seperate functions so we don't bother all the run time checking.
void RIClear(HRenderDevice Dev, uint ClearFlags, KColor& Clear = KColor(0, 0, 0), float Depth = 1.0f, int Stencil = 0, KColor& Accum = KColor(0, 0, 0))
{
	if(ClearFlags & RI_CLEAR_COLOR_BUFFER)
		glClearColor(Clear.Redf(), Clear.Greenf(), Clear.Bluef(), Clear.Alphaf());
	if(ClearFlags & RI_CLEAR_DEPTH_BUFFER)
		glClearDepth(Depth);
	if(ClearFlags & RI_CLEAR_STENCIL_BUFFER)
		glClearStencil(Stencil);
	if(ClearFlags & RI_CLEAR_ACCUM_BUFFER)
		glClearAccum(Accum.Redf(), Accum.Greenf(), Accum.Bluef(), Accum.Alphaf());
	glClear(ClearFlags);
}

HVertexBuffer RICreateVertexBuffer(HRenderDevice Dev)
{
	HVertexBuffer VBO;
	glGenBuffers(1, &(VBO.BufferID));
	glBindBuffer(GL_ARRAY_BUFFER ,VBO.BufferID);
	return VBO;
}

void RIDeleteVertexBuffer(HRenderDevice Dev, HVertexBuffer VBO)
{
	glDeleteBuffers(1, &(VBO.BufferID));
}

// Move into own cpp.
void initUrsaGL()
{
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
} 