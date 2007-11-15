#ifndef _PRIVATERENDER_H_
#define _PRIVATERENDER_H_

#include "UrRender.h"

// Keep out of view of non render code.
class UrRenderer
{
public:
	HRenderDevice Device;
	HWindowContext WindowContext;
	KIQueue<UrRenderCommand*> CommandQueue;
	KIInt GameSync;
	KColor ClearColor;
	UrRenderer(HWindowContext Win) : WindowContext(Win), GameSync(0)
	{}
	void Loop();
	void Render();
	void EnqueueCommand(UrRenderCommand* RenderCommand);
};


#endif
