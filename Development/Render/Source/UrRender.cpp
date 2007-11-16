#include "PrivateRender.h" 
#include "UConcurrency.h"

void RenderCommand(UrRenderer* Renderer, UrRenderCommand* RenderCommand)
{
	Renderer->EnqueueCommand(RenderCommand);
}

bool RenderGameSync(UrRenderer* Renderer)
{
	// We have an int we decrement on every render thread flip, and increment
	// on adding a new flip to the stack, only adds a new flip on the stack if
	// there is 1 or less on there already.
	bool bRun;
	bRun = (Renderer->GameSync.Get() <= 1);
	if(bRun)
		Renderer->GameSync.Increment();
	return bRun;
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
//	AssertRenderThread();
	Renderer->Loop();
}

void UrRenderer::Loop()
{
	//AssertRenderThread();
	bool bContinue = true;

	Device = RICreateContext(WindowContext);
	ClearColor = KColor(0, 0, 0, 0);
	
	while(bContinue)
	{
		UrRenderCommand* CurrentCommand = NULL;
		while(true)
		{
			delete CurrentCommand;
			CurrentCommand = NULL;
			CommandQueue.Dequeue(CurrentCommand);
			if(CurrentCommand == NULL)
				continue;
			CurrentCommand->Execute(this);
			if(CurrentCommand->CommandType == Render_Command_Kill)
			{
				bContinue = false;
				break;
			}
			if(CurrentCommand->CommandType == Render_Command_FrameSync)
			{
				GameSync.Decrement();
				break;
			}
		}
		Render();
		delete CurrentCommand;
	}

	RIDestroyDevice(Device);
}

void UrRenderer::Render()
{
	//AssertRenderThread();
	RIClear(Device, RI_CLEAR_COLOR_BUFFER, ClearColor);
	RIPresent(Device);
}

void UrRenderer::EnqueueCommand(UrRenderCommand* RenderCommand)
{
	CommandQueue.Enqueue(RenderCommand);
}

// ClearColor

void UrClearColorCommand::Execute(UrRenderer* Renderer)
{
	Renderer->ClearColor = ClearColor;
}

// World Grid


class UrGridModel : public UrModel
{
	HVertexBuffer Buffer;
public:
	UrGridModel(UrRenderer* renderer, KColor Color, int NumLines, float GridSpacing) : UrModel(renderer)
	{
		KVertexPos* GridVerts = new KVertexPos[4 * NumLines];
		float Min = (((float)NumLines-1)/2.0f) * GridSpacing * -1.0f;
		float Max = (((float)NumLines-1)/2.0f) * GridSpacing;
		for(int x=0; x<NumLines; x++)
		{
			GridVerts[x*2] = KVertexPos(Min+x*GridSpacing, Min, 0.0f);
			GridVerts[x*2+1] = KVertexPos(Min+x*GridSpacing, Max, 0.0f);
		}
		for(int y=0; y<NumLines; y++)
		{
			GridVerts[NumLines*2+y*2] = KVertexPos(Min, Min+y*GridSpacing, 0.0f);
			GridVerts[NumLines*2+y*2+1] = KVertexPos(Max, Min+y*GridSpacing, 0.0f);
		}
		Buffer = RICreateVertexBuffer(Renderer->Device);
		RISetBufferData(Renderer->Device, Buffer, GridVerts, 4 * NumLines * sizeof(KVertexPos));
	}
	void Render(UrModelInstance* Instance)
	{

	}
};

void UrCreateGridCommand::Execute(UrRenderer* Renderer)
{
	UrGridModel* Model = new UrGridModel(Renderer, GridColor, NumGridLines, GridSpacing);
	UrRenderGroup* RGroup = new UrRenderGroup(Model);
	UrModelInstance* Instance = new UrModelInstance(Model);
	RGroup->AddInstance(Instance);
}
