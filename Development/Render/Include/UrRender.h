#ifndef _URRENDER_H_ 
#define _URRENDER_H_

#include "UThreading.h"
#include "RenderInterface.h"
#include "UMath.h"
#include "UMacro.h"

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

void RenderCommand(UrRenderer* Renderer, UrRenderCommand* RenderCommand);
bool RenderGameSync(UrRenderer* Renderer);

class UrClearColorCommand : public UrRenderCommand
{
	KColor ClearColor;
public:
	void Execute(UrRenderer* Renderer);
	UrClearColorCommand(KColor& Clear) : UrRenderCommand(Render_Command_Execute), ClearColor(Clear)
	{}
};

/*#define COMMANDMEMBERS(type, value) type value;
#define COMMANDINITPARAMS(type, value) type In##value,
#define COMMANDINIT(type, value) , value(In##value)
#define COMMANDFUNCPARAMS(type, value) , type value
#define COMMANDNEWPARAMS(type, value) value,

#define Declare_Command(name)	class Ur##name##Command : public UrRenderCommand \
{ \
	Args(COMMANDMEMBERS) \
public: \
	void Execute(UrRenderer* Renderer); \
	Ur##name##Command(Args(COMMANDINITPARAMS) int Reserved) : \
	UrRenderCommand(Render_Command_Execute) Args(COMMANDINIT){} \
};\
void RC##name(UrRenderer* Renderer Args(COMMANDFUNCPARAMS)) \
{ \
	RenderCommand(Renderer, new Ur##name##Command(Args(COMMANDNEWPARAMS) 0)); \
}


#define Args(Arg) Arg(KColor, GridColor) Arg(int, NumGridLines) Arg(float, GridSpacing)
Declare_Command(CreateGrid);*/

#define COMMANDMEMBER(i, ...) MACRO_LOOP(COMMANDMEMBERLOOP, i, __VA_ARGS__)
#define COMMANDMEMBERLOOP(type, value)	type value;

#define COMMANDINITPARAMS(i, ...) MACRO_LOOP(COMMANDINITPARAMSLOOP, i, __VA_ARGS__)
#define COMMANDINITPARAMSLOOP(type, value) type In##value,

#define COMMANDINIT(i, ...) MACRO_LOOP(COMMANDINITLOOP, i, __VA_ARGS__)
#define COMMANDINITLOOP(type, value) , value(In##value)

#define COMMANDFUNCPARAMS(i, ...) MACRO_LOOP(COMMANDFUNCPARAMSLOOP, i, __VA_ARGS__)
#define COMMANDFUNCPARAMSLOOP(type, value) , type value

#define COMMANDNEWPARAMS(i, ...) MACRO_LOOP(COMMANDNEWPARAMSLOOP, i, __VA_ARGS__)
#define COMMANDNEWPARAMSLOOP(type, value) value,


#define Declare_Command(name, i, ...)	class Ur##name##Command : public UrRenderCommand \
{ \
	COMMANDMEMBER(i, __VA_ARGS__) \
public: \
	void Execute(UrRenderer* Renderer); \
	Ur##name##Command(COMMANDINITPARAMS(i, __VA_ARGS__) int Reserved) : \
	UrRenderCommand(Render_Command_Execute) COMMANDINIT(i, __VA_ARGS__){} \
};\
inline void RC##name(UrRenderer* Renderer COMMANDFUNCPARAMS(i, __VA_ARGS__)) \
{ \
	RenderCommand(Renderer, new Ur##name##Command(COMMANDNEWPARAMS(i, __VA_ARGS__) 0)); \
}

Declare_Command(CreateGrid, 3, (KColor, GridColor), (int, NumGridLines), (float, GridSpacing))

/*class UrCreateGridCommand : public UrRenderCommand
{
	KColor GridColor;
	int NumGridLines;
	float GridSpacing;
public:
	void Execute(UrRenderer* Renderer);
	UrCreateGridCommand(KColor& Color, int NumLines, float Spacing) : UrRenderCommand(Render_Command_Execute), GridColor(Color), NumGridLines(NumLines), GridSpacing(Spacing)
	{}
};*/

Declare_Command(CameraProjection, 4, (float, NearClip), (float, FarClip), (float, HFOV), (float, VFOV))

/*class UrCameraProjectionCommand : public UrRenderCommand
{
	float NearClip;
	float FarClip;
	float HFOV;
	float VFOV;
public:
	void Execute(UrRenderer* Renderer);
	UrCameraProjectionCommand(float NearClipPlane, float FarClipPlane, float HorizontalFov, float VerticalFov) : UrRenderCommand(Render_Command_Execute), NearClip(NearClipPlane), FarClip(FarClipPlane), HFOV(HorizontalFov), VFOV(VerticalFov)
	{}
};*/

Declare_Command(ViewTransform, 1, (KMatrix, Transform))

/*class UrViewTransformCommand : public UrRenderCommand
{
	KMatrix Transform;
public:
	void Execute(UrRenderer* Renderer);
	UrViewTransformCommand(KMatrix& InTransform) : UrRenderCommand(Render_Command_Execute), Transform(InTransform)
	{}
};*/

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
