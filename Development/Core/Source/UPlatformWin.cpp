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

void* appMemcpy( void* Dest, const void* Src, size_t Length)
{
	return memcpy(Dest, Src, Length);
}

psize appStrlen( const char* Str )
{
	return strlen(Str);
}

char* appStrcpy( char* Dest, const char* Src )
{
	return strcpy(Dest, Src);
}

int appStricmp( const char* str1, const char* str2 )
{
	return stricmp(str1, str2);
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
#pragma warning( push )
#pragma warning( disable : 4311 4312 ) // Not much I could do, tis how it's defined in the headers, should be 64 bit compatible though.
	return InterlockedExchangePointer(Dest, Exchange);
#pragma warning( pop )
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


DirIterator::DirIterator(const char* Path)
{
	const int BufferLen = 4096;
	//int i;
	char Buffer[BufferLen];
	//for(i=0; Path[i] != '\0'; i++){}
	//if(Path[i-1] != '\\')
	//{
	//	char* tmpPath = new char[i+2];
	//	appMemcpy(tmpPath, Path, i);
	//	tmpPath[i] = '\\';
	//	tmpPath[i+1] = '\0';
	//}
	GetFullPathName(Path, BufferLen, Buffer, NULL);
	FileHandle = FindFirstFile(Buffer, &DirData);
	assert(FileHandle != INVALID_HANDLE_VALUE);
	//printf("Error: %i\n", GetLastError());
}

DirIterator::~DirIterator()
{
	FindClose(FileHandle);
}

bool DirIterator::Next()
{
	if(!FindNextFile(FileHandle, &DirData))
	{
		if(GetLastError() == ERROR_NO_MORE_FILES)
			return false;
		else
			assert(false);
	}

	return true;
}

bool DirIterator::isDirectory()
{
	return (DirData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool DirIterator::isCurOrPrevDir()
{
	if(FileName()[0] == '.' && (
		FileName()[1] == '\0' || (
		FileName()[1] == '.' && FileName()[2] == '\0')))
		return true;
	return false;
}

char* DirIterator::FileName()
{
	return DirData.cFileName;
}

File* File::OpenFile(const char* Path)
{
	File* F = new File();
	F->_file = fopen(Path, "r"); // just doing reading of text for now.
	if(F->_file == NULL)
	{
		delete F;
		return NULL;
	}
	else
		return F;
}

File::File()
{
}

File::~File()
{
	fclose(_file);
}

size_t File::FileLength()
{
	// No real length query, so we need to seek to the end, and get pos.
	long FileLen;
	long StartPos = ftell(_file);
	fseek(_file, 0, SEEK_END);
	FileLen = ftell(_file);
	fseek(_file, StartPos, SEEK_SET);
	return FileLen;
}

char* File::ReadAll()
{
	size_t FileLen = FileLength();
	char* ReadBuffer = new char[FileLen];
	fread(ReadBuffer, sizeof(char), FileLen, _file);
	return ReadBuffer;
}

char* File::Read()
{
	assert(false);
	return NULL;
}