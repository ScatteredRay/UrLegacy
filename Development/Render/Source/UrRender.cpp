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
#if USING_GL
	initUrsaGL();
#endif //USING_GL
	ClearColor = KColor(0, 0, 0, 0);
	MatrixIdentity(&View);
	MatrixIdentity(&Projection);
	
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
	RIBeginScene(Device);
	RIClear(Device, RI_CLEAR_COLOR_BUFFER, ClearColor);
	RISetMatrix(Device, RI_TRANSFORM_VIEW, View);
	RISetMatrix(Device, RI_TRANSFORM_PROJECTION, Projection);
	for(uint i=0; i<RenderGroups.Num(); i++)
	{
		RenderGroups[i]->Render();
	}
	RIEndScene(Device);
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
	uint NumPrimitives;
	KColor GridColor;
public:
	UrGridModel(UrRenderer* renderer, KColor Color, int NumLines, float GridSpacing) : UrModel(renderer), GridColor(Color)
	{
		NumPrimitives = 2 * NumLines;
		KVertexPos* GridVerts = new KVertexPos[NumPrimitives * 2];
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
		const int VBOSize = 4 * NumLines * sizeof(KVertexPos);
		Buffer = RICreateVertexBuffer(Renderer->Device, VBOSize);
		RISetBufferData(Renderer->Device, Buffer, GridVerts, VBOSize);
		delete GridVerts;
	}
	void Render(UrModelInstance* Instance)
	{
		RISetColor(Renderer->Device, GridColor);
		RIBindBuffer(Renderer->Device, Buffer);
		RIDrawPrimitive(Renderer->Device, RI_PRIMITIVE_LINES, 0, NumPrimitives);
	}
};

void UrCreateGridCommand::Execute(UrRenderer* Renderer)
{
	UrGridModel* Model = new UrGridModel(Renderer, GridColor, NumGridLines, GridSpacing);
	UrRenderGroup* RGroup = new UrRenderGroup(Model);
	UrModelInstance* Instance = new UrModelInstance(Model);
	RGroup->AddInstance(Instance);
	Renderer->RenderGroups.AddItem(RGroup);
	Renderer->RenderGroups[Renderer->RenderGroups.Num()-1] = RGroup;
}

void UrCameraProjectionCommand::Execute(UrRenderer *Renderer)
{
	MatrixPerspectiveProjection(&Renderer->Projection, NearClip, FarClip, HFOV, VFOV);
}
