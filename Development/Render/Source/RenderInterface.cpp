#include "RenderInterface.h" 
#include "Core.h"
#include "UTypes.h"
#include "UrsaGL.h"

PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;

HRenderDevice RICreateContext(HWindowContext WC)
{
	HRenderDevice Dev(wglCreateContext(WC.hdc), WC.hdc);
	wglMakeCurrent(WC.hdc, Dev.context); // in GL we assume only one device per thread, so we just go ahead and make it current.
	return Dev;
}

bool RIDestroyDevice(HRenderDevice Dev)
{
	return wglDeleteContext(Dev.context);
}

void RIPresent(HRenderDevice Dev)
{
	SwapBuffers(Dev.hdc);
}

//TODO: break the optionals into seperate functions so we don't bother all the run time checking.
void RIClear(HRenderDevice Dev, uint ClearFlags, KColor& Clear, float Depth, int Stencil, KColor& Accum)
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