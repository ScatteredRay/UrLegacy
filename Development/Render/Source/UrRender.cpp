#include "UrRender.h" 
#include "UConcurrency.h"

// Keep out of view of non render code.
class UrRenderer
{
	HRenderDevice Device;
	HWindowContext WindowContext;
	KIQueue<UrRenderCommand*> CommandQueue;
public:
	UrRenderer(HWindowContext Win) : WindowContext(Win)
	{}
	void Loop();
	void Render();
	void EnqueueCommand(UrRenderCommand* RenderCommand);
};

void RenderCommand(UrRenderer* Renderer, UrRenderCommand* RenderCommand)
{
	Renderer->EnqueueCommand(RenderCommand);
}

UrRenderThread::UrRenderThread(HWindowContext Win) : WinContext(Win)
{
	Renderer = new UrRenderer(Win);
}

UrRenderer* UrRenderThread::GetRenderer()
{
	return Renderer;
}

void UrRenderThread::Run()
{
	AssertRenderThread();
	Renderer->Loop();
}

void UrRenderer::Loop()
{
	AssertRenderThread();
	bool bContinue = true;

	Device = RICreateContext(WindowContext);
	
	while(bContinue)
	{
		UrRenderCommand* CurrentCommand;
		do
		{
			CurrentCommand = NULL;
			CommandQueue.Dequeue(CurrentCommand);
		}
		while(CurrentCommand->CommandType != Render_Command_Kill && 
				CurrentCommand->CommandType != Render_Command_FrameSync);
		Render();
	}

	RIDestroyDevice(Device);
}

void UrRenderer::Render()
{
	AssertRenderThread();
	RIClear(Device, RI_CLEAR_COLOR_BUFFER, KColor(1.0f, 0.0f, 0.0f, 0.0f));
	RIPresent(Device);
}

void UrRenderer::EnqueueCommand(UrRenderCommand* RenderCommand)
{
	CommandQueue.Enqueue(RenderCommand);
}