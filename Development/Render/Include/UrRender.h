#ifndef _URRENDER_H_ 
#define _URRENDER_H_

#include "UThreading.h"
#include "RenderInterface.h"

#define AssertRenderThread() assert(GetRunningThread()->GetFlags() | Thread_Rendering)

class UrRenderer;

enum RenderCommandType
{
	Render_Command_Kill,
	Render_Command_FrameSync,
	Render_Command_Execute
};

class UrRenderCommand
{
	RenderCommandType CommandType;
	friend class UrRenderer;
public:
	virtual void Execute()=0;
};

void RenderCommand(UrRenderer* Renderer, UrRenderCommand* RenderCommand);

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
