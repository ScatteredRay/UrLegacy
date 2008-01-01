#include "UPlatform.h"
#include "Core.h"
#include "UMemory.h"

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

char* appStrncpy( char* Dest, const char* Src, size_t Len)
{
	return strncpy(Dest, Src, Len);
}

int appStricmp( const char* str1, const char* str2 )
{
	return stricmp(str1, str2);
}

char* appStrupr( char* str )
{
	char* chr = str;
	while(chr[0] != '\0')
	{
		if(chr[0] >= 'a' && chr[0] <= 'z')
			chr[0] += ('A' - 'a');
		chr++;
	}
	return str;
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

TextFile* TextFile::OpenFile(const char* Path)
{
	TextFile* F = new TextFile();
	F->_file = fopen(Path, "r");
	if(F->_file == NULL)
	{
		delete F;
		return NULL;
	}
	else
		return F;
}

BinaryFile* BinaryFile::OpenFile(const char* Path)
{
	BinaryFile* F = new BinaryFile();
	F->_file = fopen(Path, "rb");
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

TextFile::TextFile()
{
}

TextFile::~TextFile()
{
}

size_t TextFile::FileLength()
{
	// No real length query, so we need to seek to the end, and get pos.
	long FileLen;
	long StartPos = ftell(_file);
	fseek(_file, 0, SEEK_END);
	FileLen = ftell(_file);
	fseek(_file, StartPos, SEEK_SET);
	return FileLen;
}

char* TextFile::ReadAll()
{
	size_t FileLen = FileLength();
	char* ReadBuffer = new char[FileLen];
	fread(ReadBuffer, sizeof(char), FileLen, _file);
	return ReadBuffer;
}

char* TextFile::ReadLine()
{
	char* ReadBuffer = new char[MaxLineLen];
	char* out = fgets(ReadBuffer, MaxLineLen, _file);
	if(out == ReadBuffer)
		return ReadBuffer;
	else
		delete ReadBuffer;
	return NULL;
}

char* TextFile::Read()
{
	assert(false);
	return NULL;
}

BinaryFile::BinaryFile()
{
}

BinaryFile::~BinaryFile()
{
}

char* GetSubDirPath(const char* Dir, const char* SubDir, const char* Ext)
{
			psize CDirLen = appStrlen(Dir);
			psize FileNameLen = appStrlen(SubDir);
			psize ExtLen = appStrlen(Ext);
			psize DirBufLen = CDirLen + FileNameLen + ExtLen; // - ('*') + ('\\', '*', '\0')
			char* NewDir = (char*)appMalloc(DirBufLen * sizeof(char));
			appStrcpy(NewDir, Dir);
			appStrcpy(NewDir+CDirLen-1, SubDir);
			appStrcpy(NewDir+CDirLen+FileNameLen-1, Ext);
			return NewDir;
}

char* GetSubDirPath(const char* Dir, const char* SubDir)
{
	return GetSubDirPath(Dir, SubDir, "");
}

bool HasExtension(const char* Dir, const char* Ext)
{
	size_t DirLen = appStrlen(Dir);
	size_t ExtLen = appStrlen(Ext);
	return (appStricmp(Dir+DirLen-ExtLen, Ext) == 0);
}