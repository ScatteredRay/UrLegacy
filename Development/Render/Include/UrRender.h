#ifndef _URRENDER_H_ 
#define _URRENDER_H_

#include "UThreading.h"
#include "RenderInterface.h"

#define AssertRenderThread() assert(GetRunningThread()->GetFlags() | Thread_Rendering)

class UrRenderer;

class UrRenderThread : public KThreadJob
{
	HWindowContext WinContext;
	UrRenderer* Renderer;
public:
	virtual void Run();
	UrRenderer* GetRenderer();
	UrRenderThread(HWindowContext Win);
};

#endif //_URRENDER_H_
