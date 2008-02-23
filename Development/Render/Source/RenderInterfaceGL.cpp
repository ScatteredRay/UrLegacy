#include "RenderInterface.h" 

#if USING_GL
#include "Core.h"
#include "UTypes.h"
#include "UrsaGL.h"

PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;

HRenderDevice RICreateContext(HWindowContext WC)
{
	HDC hdc = GetDC(WC.hWnd);
	HRenderDevice Dev(wglCreateContext(hdc), hdc);
	wglMakeCurrent(hdc, Dev.context); // in GL we assume only one device per thread, so we just go ahead and make it current.
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

void RIBeginScene(HRenderDevice Dev){}

void RIEndScene(HRenderDevice Dev){}

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

HVertexBuffer RICreateVertexBuffer(HRenderDevice Dev, size_t Size)
{
	HVertexBuffer VBO;
	glGenBuffers(1, &(VBO.BufferID));
	glBindBuffer(GL_ARRAY_BUFFER, VBO.BufferID);
	return VBO;
}

void RIDeleteVertexBuffer(HRenderDevice Dev, HVertexBuffer VBO)
{
	glDeleteBuffers(1, &(VBO.BufferID));
}

void RISetBufferData(HRenderDevice Dev, HVertexBuffer VBO, void* Data, size_t Size)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO.BufferID);
	glBufferData(GL_ARRAY_BUFFER, Size, Data, GL_STATIC_DRAW);
}

void RIBindBuffer(HRenderDevice Dev, HVertexBuffer VBO, uint32 StreamNumber)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO.BufferID);
	glVertexPointer(3, GL_FLOAT, 3*4, NULL);
	//glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	//glNormalPointer(3, GL_FLOAT, 0, NULL);
}

void RIDrawPrimitive(HRenderDevice Dev, uint DrawType, uint StartVertex, uint PrimitiveCount)
{
	glDrawArrays(DrawType, StartVertex, PrimitiveCount);
}

void RISetColor(HRenderDevice Dev, KColor& Color)
{
	glColor3f(Color.Redf(), Color.Greenf(), Color.Bluef());
}

void RISetMatrix(HRenderDevice Dev, uint TransformState, Matrix4& Transform)
{

}

void initUrsaGL()
{
	const unsigned char* Extensions = glGetString( GL_EXTENSIONS );

	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
} 

#endif //USING_GL