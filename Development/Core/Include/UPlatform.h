#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "Core.h"
#include "UTypes.h"
#include <stdio.h>

void* sysMalloc( psize Size );
void* sysRealloc( void* P, psize Size );
void sysFree( void* P );
void* appMemset( void* Dest, int C, psize Length );
void* appMemzero( void* Dest, psize Length );
void* appMemcpy( void* Dest, const void* Src, psize Length );
psize appStrlen( const char* Str );
char* appStrcpy( char* Dest, const char* Src);
char* appStrncpy( char* Dest, const char* Src, size_t Len);
int appStricmp( const char* str1, const char* str2 );
char* appStrupr( char* str );

// Application specific interlocked functions

LONG appInterlockedCompareExchange( volatile LONG* Dest, LONG Exchange, LONG Comparand );
void* appInterlockedCompareExchangePointer( volatile void** Dest, void* Exchange, void* Comparand );
LONG appInterlockedExchange( volatile LONG* Dest, LONG Exchange );
void* appInterlockedExchangePointer( volatile void** Dest, void* Exchange );
LONG appInterlockedIncrement( volatile LONG* Dest );
LONG appInterlockedDecrement( volatile LONG* Dest );
LONG appInterlockedExchangeAdd( volatile LONG* Dest, LONG Value );
LONG appInterlockedOr( volatile LONG* Dest, LONG Value );

#include <stdio.h>

class DirIterator
{
	WIN32_FIND_DATA DirData;
	HANDLE FileHandle;
public:
	DirIterator(const char* Path);
	~DirIterator();
	bool Next();
	bool isDirectory();
	bool isCurOrPrevDir();
	char* FileName();
};

const size_t MaxLineLen = 255; // it's how fgets works, we'll see if we can remove this if it becomes a problem.

class File
{
	//friend class TextFile;
	//friend class BinaryFile;
protected:
	FILE* _file;
	File();
public:
	~File();
	size_t FileLength();
	char* ReadAll();
	char* Read();
	char* ReadLine();
};

class TextFile : public File
{
	TextFile();
public:
	static class TextFile* OpenFile(const char* Path);
	~TextFile();
	size_t FileLength();
	char* ReadAll();
	char* Read();
	char* ReadLine();
};

class BinaryFile : public File
{
	BinaryFile();
public:
	static class BinaryFile* OpenFile(const char* Path);
	~BinaryFile();
};

char* GetSubDirPath(const char* Dir, const char* SubDir, const char* Ext);
char* GetSubDirPath(const char* Dir, const char* SubDir);
bool HasExtension(const char* Dir, const char* Ext);

#endif //_PLATFORM_H_