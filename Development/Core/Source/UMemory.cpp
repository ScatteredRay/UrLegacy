#include "UMemory.h"
#include "UTypes.h"

#ifdef DEBUG
//enum
//{
static const int MemBlock_Signature = 0xABACADAB;
//};
#endif

struct KMemBlockHead
{
	// we will use flags here instead of bitfields to make it eaisier to use 
	enum 
	{
		MemBlock_Marked,
		MemBlock_Destroy
	};
	KIInt MemFlags;
	bitfield bMarked : 1;
	bitfield bDestroy : 1;
	KIPointer<KMemBlockHead> NextBlock;
#ifdef DEBUG
	int Signature;
#endif
};

KMemoryManager::KMemoryManager() : FirstBlock(NULL), LastBlock(NULL)
{
	//So that we don't have to check for first every time, we will add a first now.
	FirstBlock = (KMemBlockHead*)Malloc(0)-sizeof(KMemBlockHead);
}

void* KMemoryManager::Malloc( psize Size )
{
	//TODO: some kind of race condition here, seems to be in the MemZero... sysMalloc not threadsafe?
	KMemBlockHead* MemHeader;
	MemHeader = (KMemBlockHead*)sysMalloc(sizeof(KMemBlockHead)+Size);

	appMemzero(MemHeader, sizeof(KMemBlockHead)+Size);
#ifdef DEBUG
	MemHeader->Signature = MemBlock_Signature;
#endif

	KMemBlockHead* OldLastBlock;
	do
	{
		OldLastBlock = LastBlock;
	}
	while(LastBlock.CompareExchange(MemHeader, OldLastBlock) != OldLastBlock);
	
	if(OldLastBlock != NULL)
	{
		OldLastBlock->NextBlock = MemHeader;
	}

	return ((char*)MemHeader)+sizeof(KMemBlockHead);

}

void* KMemoryManager::FastMalloc( psize Size )
{
	KMemBlockHead* MemHeader;
	MemHeader = (KMemBlockHead*)sysMalloc(sizeof(KMemBlockHead)+Size);

#ifdef DEBUG
	MemHeader->Signature = MemBlock_Signature;
#endif

	KMemBlockHead* OldLastBlock;
	do
	{
		OldLastBlock = LastBlock;
	}
	while(LastBlock.CompareExchange(MemHeader, OldLastBlock) != OldLastBlock);
	
	if(OldLastBlock != NULL)
	{
		OldLastBlock->NextBlock = MemHeader;
	}

	return ((char*)MemHeader)+sizeof(KMemBlockHead);

}

void KMemoryManager::Free( void* P )
{
	if(P == NULL)
		return;
	KMemBlockHead* MemHeader = (KMemBlockHead*)((char*)P-sizeof(KMemBlockHead));

	ASSERT(MemHeader->Signature == MemBlock_Signature); // Trying to free bad memory
	ASSERT(!MemHeader->MemFlags.Get() | KMemBlockHead::MemBlock_Destroy); // Trying to free free'd memory

	MemHeader->MemFlags.Or(KMemBlockHead::MemBlock_Destroy);
}

void* KMemoryManager::Realloc( void* P, psize Size )
{
	assert(false && "Realloc not implemented");
}

bool KMemoryManager::VerifyConsistancy()
{
	KMemBlockHead* MemBlock = FirstBlock;
	while(MemBlock)
	{
		assert(MemBlock->Signature == MemBlock_Signature);
		MemBlock = MemBlock->NextBlock;
	}
	return true;
}

void KMemoryManager::Collect()
{
	//TODO: Ensure no other threads are running.

	// Unmark all blocks
	KMemBlockHead* MemBlock = FirstBlock;
	while(MemBlock)
	{
		assert(MemBlock->Signature == MemBlock_Signature);
		MemBlock->bMarked = false;
		MemBlock = MemBlock->NextBlock;
	}

	// Add Roots to list


}
