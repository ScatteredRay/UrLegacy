// UrsaTest.cpp : Defines the entry point for the console application.
//


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>

#include "Testing.h"


int _tmain(int argc, _TCHAR* argv[])
{
	return TestingUnit::RunAllTests();
}

