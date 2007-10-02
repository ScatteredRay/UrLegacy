#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "UTypes.h"

void* sysMalloc( size_t Size );
void* sysRealloc( void* P, size_t Size );
void sysFree( void* P );
void* appMemset( void* Dest, int C, size_t Length );
void* appMemzero( void* Dest, size_t Length );

// Application specific interlocked functions

LONG appInterlockedCompareExchange( volatile LONG* Dest, LONG Exchange, LONG Comparand );
void* appInterlockedCompareExchangePointer( volatile void** Dest, void* Exchange, void* Comparand );
LONG appInterlockedExchange( volatile LONG* Dest, LONG Exchange );
void* appInterlockedExchangePointer( volatile void** Dest, void* Exchange );
LONG appInterlockedIncrement( volatile LONG* Dest );
LONG appInterlockedDecrement( volatile LONG* Dest );
LONG appInterlockedExchangeAdd( volatile LONG* Dest, LONG Value );
LONG appInterlockedOr( volatile LONG* Dest, LONG Value );

#endif //_PLATFORM_H_