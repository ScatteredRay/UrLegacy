#include "UMemory.h"
#include "UTypes.h"



KMemoryManager::KMemoryManager() : FirstBlock(NULL), LastBlock(NULL)
{
}

void* KMemoryManager::Malloc( psize Size )
{
	void* Memory = sysMalloc(Size);
	appMemzero(Memory, Size);
	return Memory;
}

void* KMemoryManager::FastMalloc( psize Size )
{
	return sysMalloc(Size);
}

void KMemoryManager::Free( void* P )
{
	sysFree(P);
}

void* KMemoryManager::Realloc( void* P, psize Size )
{
	assert(false && "Realloc not implemented");
	return NULL;
}

bool KMemoryManager::VerifyConsistancy()
{
	return true;
}

void KMemoryManager::Collect()
{

}
