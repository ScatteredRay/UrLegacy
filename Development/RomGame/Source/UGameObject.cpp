#include "UGameObject.h"

class UObjectManager* GObjectManager;

UGameObject::UGameObject()
{
	assert(GObjectManager);
	GObjectManager->AddObject(this);
}

UGameObject::~UGameObject()
{
	assert(GObjectManager);
	GObjectManager->RemoveObject(this);
}

void UObjectManager::AddObject(UGameObject* Object)
{
	Objects.AddItem(Object);
}

void UObjectManager::RemoveObject(UGameObject* Object)
{
	Objects.RemoveItem(Object);
}

void UObjectManager::TickObjects(float DeltaTime)
{
	for(uint i=0; i<Objects.Num(); i++)
	{
		Objects[i]->Tick(DeltaTime);
	}
}
