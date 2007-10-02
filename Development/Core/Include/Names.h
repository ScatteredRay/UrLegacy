#ifndef _NAMES_H_
#define _NAMES_H_


//Nice set of hash function, stolen from Bob Jenkins at http://burtleburtle.net/bob/c/lookup3.c
uint32_t hashlittle( const void *key, size_t length, uint32_t initval);
void hashlittle2( const void *key, size_t length, uint32_t *pc, uint32_t *pb);

const unsigned int HashSeed = 0xF48D32AB;

class KNames
{

private:
	unsigned int HashString(const char* String, unsigned int Length)
	{
		return hashlittle( (void*)String, Length, HashSeed );
	}
};

#endif //_NAMES_H_