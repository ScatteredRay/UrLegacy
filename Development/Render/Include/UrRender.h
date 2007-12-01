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

#define Define_Command(name, rnd) void Ur##name##Command::Execute(UrRenderer* rnd)

Declare_Command(ClearColor, 1, (KColor, ClearColor))
Declare_Command(CreateGrid, 3, (KColor, GridColor), (int, NumGridLines), (float, GridSpacing))
Declare_Command(CameraProjection, 4, (float, NearClip), (float, FarClip), (float, HFOV), (float, VFOV))
Declare_Command(ViewTransform, 1, (KMatrix, Transform))

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
