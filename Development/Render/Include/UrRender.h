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
	virtual void Execute(UrRenderer* Renderer){};
	UrRenderCommand(RenderCommandType Type) : CommandType(Type){}
};

class UrClearColorCommand : public UrRenderCommand
{
	KColor ClearColor;
public:
	void Execute(UrRenderer* Renderer);
	UrClearColorCommand(KColor& Clear) : UrRenderCommand(Render_Command_Execute), ClearColor(Clear)
	{}
};

class UrCreateGridCommand : public UrRenderCommand
{
	KColor GridColor;
	int NumGridLines;
	float GridSpacing;
public:
	void Execute(UrRenderer* Renderer);
	UrCreateGridCommand(KColor& Color, int NumLines, float Spacing) : UrRenderCommand(Render_Command_Execute), GridColor(Color), NumGridLines(NumLines), GridSpacing(Spacing)
	{}
};

void RenderCommand(UrRenderer* Renderer, UrRenderCommand* RenderCommand);
bool RenderGameSync(UrRenderer* Renderer);


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
