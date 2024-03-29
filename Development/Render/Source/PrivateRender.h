#ifndef _PRIVATERENDER_H_
#define _PRIVATERENDER_H_

#include "UrRender.h"
#include "UMath.h"

class UrRenderGroup;
class UrModel;
class UrParticleSystemModel;

// Keep out of view of non render code.
class UrRenderer
{
public:
	HRenderDevice Device;
	HWindowContext WindowContext;
	KIQueue<UrRenderCommand*> CommandQueue;
	KIInt GameSync;
	KColor ClearColor;
	Matrix4 View;
	Matrix4 Projection;
	KArray<UrRenderGroup*> RenderGroups;
	KArray<UrParticleSystemModel*> ParticleSystems;
public:
	UrRenderer(HWindowContext Win) : WindowContext(Win), GameSync(0), RenderGroups()
	{}
	void Loop();
	void Render();
	void EnqueueCommand(UrRenderCommand* RenderCommand);
};

class UrModelInstance
{
public:
	UrModelInstance(UrModel* Model)
	{}
};

class UrModel
{
public:
	UrRenderer* Renderer;
	UrModel(UrRenderer* R) : Renderer(R)
	{}
	virtual void Render(UrModelInstance* Instance)=0;
};

class UrRenderGroup
{
	UrModel* Model;
public:
	KArray<UrModelInstance*> Instances;
	UrRenderGroup(UrModel* M) : Model(M)
	{}
	~UrRenderGroup()
	{
		//TODO: remove me once model management is in place.
		delete Model;
		for(uint i=0; i<Instances.Num(); i++)
		{
			delete Instances[i];
		}
	}
	void AddInstance(UrModelInstance* Instance)
	{
		Instances.AddItem(Instance);
		Instances[Instances.Num()-1] = Instance;
	}
	virtual void Render()
	{
		for(uint i=0; i<Instances.Num(); i++)
		{
			Model->Render(Instances[i]);
		}
	}
};


#endif
