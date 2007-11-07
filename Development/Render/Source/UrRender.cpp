#include "UrRender.h" 

// Keep out of view of non render code.
class UrRenderer
{
	HRenderDevice Device;
	HWindowContext WindowContext;
public:
	UrRenderer(HWindowContext Win) : WindowContext(Win)
	{}
	void Loop();
	void Render();
};

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
	Device = RICreateContext(WindowContext);
	
	while(true)
	{
		Render();
	}

	RIDestroyDevice(Device);
}

void UrRenderer::Render()
{
	RIClear(Device, RI_CLEAR_COLOR_BUFFER, KColor(1.0f, 0.0f, 0.0f, 0.0f));
	RIPresent(Device);
}