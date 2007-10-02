#include "UPlatform.h"
#include "Core.h"

void* sysMalloc( size_t Size )
{
	return malloc(Size);
}

void* sysRealloc( void* P, size_t Size )
{
	return realloc(P, Size);
}

void sysFree( void* P )
{
	free(P);
}

void* appMemset( void* Dest, int C, size_t Length )
{
	return memset(Dest, C, Length);
}

void* appMemzero( void* Dest, size_t Length )
{
	return memset(Dest, 0, Length);
}

// Application specific interlocked functions

LONG appInterlockedCompareExchange( volatile LONG* Dest, LONG Exchange, LONG Comparand )
{
	return InterlockedCompareExchange(Dest, Exchange, Comparand);
}

void* appInterlockedCompareExchangePointer( volatile void** Dest, void* Exchange, void* Comparand )
{
	return InterlockedCompareExchangePointer((PVOID*)Dest, Exchange, Comparand);
}

LONG appInterlockedExchange( volatile LONG* Dest, LONG Exchange )
{
	return InterlockedExchange(Dest, Exchange);
}

void* appInterlockedExchangePointer( volatile void** Dest, void* Exchange )
{
	return InterlockedExchangePointer((PLONG)Dest, (LONG)Exchange);
}

LONG appInterlockedIncrement( volatile LONG* Dest )
{
	return InterlockedIncrement(Dest);
}

LONG appInterlockedDecrement( volatile LONG* Dest )
{
	return InterlockedDecrement(Dest);
}

LONG appInterlockedExchangeAdd( volatile LONG* Dest, LONG Value )
{
	return InterlockedExchangeAdd(Dest, Value);
}

LONG appInterlockedOr( volatile LONG* Dest, LONG Value )
{

	// compile error with the systems interlockedOr, here is a temp implementation with a spinlock
	LONG OldValue;

	do
	{
		InterlockedExchange( &OldValue, *Dest );
	}
	while(appInterlockedCompareExchange(Dest, OldValue|Value, OldValue) != OldValue);

	return OldValue;

	//return InterlockedOr(Dest, Value);
}
