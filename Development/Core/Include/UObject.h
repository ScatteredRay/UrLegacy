#ifndef _UOBJECT_H_
#define _UOBJECT_H_
#include "Core.h"
#include "UMemory.h"

class Object
{
public:
	virtual void DeclareReferences(KRefList* List){};
};

#define DECLARE_CLASS(cls, parent) typedef parent Super; 

#define BEGIN_REFERENCES(cls) void DeclareReferences(KRefList* List){ Super::DeclareReferences(List);
#define END_REFERENCES }
#define ADDREF(ref) List->AddReference((&ref)-this);

#endif //_UOBJECT_H_
