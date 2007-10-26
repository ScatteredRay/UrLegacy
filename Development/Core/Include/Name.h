#ifndef _NAME_H_
#define _NAME_H_

#include "UTypes.h"
#include <Assert.h>


//Nice set of hash function, stolen from Bob Jenkins at http://burtleburtle.net/bob/c/lookup3.c
typedef unsigned int uint32_t;
uint32_t hashlittle( const void *key, size_t length, uint32_t initval);
void hashlittle2( const void *key, size_t length, uint32_t *pc, uint32_t *pb);

const unsigned int HashSeed = 0xF48D32AB;
const unsigned int MaxStrings = 256;
const unsigned int HashMask = 0xFF; // first byte, should be 256 nums

class NameTable
{
	friend class Name;
	KArray<char*> Names;
	NameTable()
	{
		Names.Add(MaxStrings);
		assert(Names.Num() == MaxStrings);
		for(uint i=0; i<Names.Num(); i++)
		{
			Names[i] = NULL;
		}
	}
	static class NameTableFree
	{ ~NameTableFree(){ delete GetNames(); } }MemoryFree;
public:
	static NameTable* GetNames()
	{
		static NameTable* NameTbl = new NameTable();
		return NameTbl;
	}
};

static NameTable* GNames = NameTable::GetNames();

class Name
{
	uint Index;
public:
	Name(const Name& Dup)
	{
		assert(Dup.Index < MaxStrings);
		Index = Dup.Index;
	}
	Name(const char* String)
	{
		size_t length = appStrlen(String);
		char* StrUpr = new char[length];
		appStrcpy(StrUpr, String);
		appStrupr(StrUpr);
		uint StartIdx = HashIndex(StrUpr, length);
		uint SearchIdx = StartIdx;
		assert(SearchIdx < MaxStrings);
		assert(GNames);

		while(true)
		{
			if(SearchIdx == MaxStrings)
				SearchIdx = 0;

			if(GNames->Names[SearchIdx] == NULL)
			{
				Index = SearchIdx;
				GNames->Names[SearchIdx] = StrUpr;
				return;
			}
			else if(appStricmp(StrUpr, GNames->Names[SearchIdx]) == 0)
			{
				Index = SearchIdx;
				delete StrUpr;
				return;
			}

			if((SearchIdx == StartIdx-1) || (SearchIdx == 255 && StartIdx == 0))
			{
				// Done a full loop and doesn't exist, and is full, Error!
				assert(false);
				break;
			}
			SearchIdx++;
		}
	}
	Name()
	{
		Name("Unknown");
	}
	bool operator==(const Name& other) const
	{
		return Index == other.Index;
	}
private:
	unsigned int HashIndex(char* String, size_t Length)
	{
		return hashlittle( (void*)String, Length, HashSeed ) & HashMask;
	}
};

#endif //_NAME_H_
