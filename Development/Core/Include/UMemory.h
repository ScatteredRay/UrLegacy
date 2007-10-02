#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "Core.h"
#include "UThreading.h"
#include "UPlatform.h"
#include "UTypes.h"

class KRefList
{
public:
	virtual void AddReference(unsigned int RefOffset)=0;
};

//Just a wrapper for now.
class KMemoryManager
{
private:
	KIPointer<struct KMemBlockHead> FirstBlock;
	KIPointer<struct KMemBlockHead> LastBlock;
	// To prevent others from instancing it.
	KMemoryManager();
	// A bit of code to clean up the Memory Manager at the end of the program;
	static class KMemoryManagerFree
	{ ~KMemoryManagerFree(){ delete MemoryManager(); } }MemoryFree;
public:
	static KMemoryManager* MemoryManager()
	{
		static KMemoryManager* MemManager = new( (KMemoryManager*)sysMalloc(sizeof(KMemoryManager)) ) KMemoryManager();
		return MemManager;
	}
	void* Malloc(psize Size);
	// More standard malloc, doesn't zero the memory.
	void* FastMalloc(psize Size);
	void Free(void* P);
	bool VerifyConsistancy(); 
	void Collect();
};

inline void* appMalloc( psize Size )
{
	return KMemoryManager::MemoryManager()->Malloc(Size);
}

inline void* appFastMalloc( psize Size )
{
	return  KMemoryManager::MemoryManager()->FastMalloc(Size);
}

inline void appFree( void* P )
{
	KMemoryManager::MemoryManager()->Free(P);
}

inline void* operator new( psize Size )
{
	return appMalloc(Size);
}

inline void operator delete( void* P )
{
	appFree(P);
}

// placement new should use sysmalloc 

//void* operator new( psize Size, void* Place )
//{
//	return Place
//}

#endif //_MEMORY_H_
