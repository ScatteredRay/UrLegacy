#ifndef _UGAMEOBJECT_H_
#define _UGAMEOBJECT_H_

#include "UMath.h"
#include "UTypes.h"

extern class UObjectManager* GObjectManager;

class UObjectManager
{
	KArray<class UGameObject*> Objects;
public:
	void AddObject(UGameObject* Object);
	void RemoveObject(UGameObject* Object);
	void TickObjects(float DeltaTime);
};

class UGameObject
{
public:
	UGameObject();
	~UGameObject();
	virtual void Tick(float DeltaTime){};
};

class UMovableObject : public UGameObject
{
public:
	KVector Location;
	KQuat Rotation;
};

#endif //_UGAMEOBJECT_H_
