#ifndef _TESTING_H_
#define _TESTING_H_

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <memory.h>
#include <stdarg.h>
#include <varargs.h>
#include <new>

class TestingUnit
{
	static void GetMasterList(int*** ListBegin)
	{
		static int* List = (int*)memset(malloc(sizeof(int)), 0, sizeof(int));
		*ListBegin = &List;
	}
	static void SetListIndex(int i, TestingUnit* Unit)
	{
		int** ListBegin;
		GetMasterList(&ListBegin);
		*( ((TestingUnit**)((*ListBegin)+1))+i ) = Unit;
	}
public:
	static TestingUnit* RegisterTestingUnit(TestingUnit* Unit)
	{
		// In order to not depend on any core code, and have minimal dependiencies, we are going to manage our list manually.
		int** ListBegin;
		GetMasterList(&ListBegin);

		int ListLength = **ListBegin+1;

		*ListBegin = (int*)realloc(*ListBegin, sizeof(int)+sizeof(TestingUnit*)*ListLength);

		**ListBegin = ListLength;
		SetListIndex(ListLength-1, Unit);

		return Unit;
	}
	static int ListLength()
	{
		int** ListBegin;
		GetMasterList(&ListBegin);
		return **ListBegin;
	}
	static TestingUnit* ListIndex(int i)
	{
		int** ListBegin;
		GetMasterList(&ListBegin);
		return *( ((TestingUnit**)((*ListBegin)+1))+i );
	}
public:
	static int Logf( const char* Format, ... )
	{
		va_list ArgList;
		int ret;

		va_start(ArgList, Format);
		ret = vprintf(Format, ArgList);
		va_end(ArgList);
		return ret;
	}

	static int Warnf( const char* Format, ... )
	{
		va_list ArgList;
		int ret;

		va_start(ArgList, Format);
		ret = vprintf(Format, ArgList);
		va_end(ArgList);
		return ret;
	}

	// Same as Warnf, but returns false so that we can return;
	static bool Failf( const char* Format, ... )
	{
		va_list ArgList;
		int ret;

		va_start(ArgList, Format);
		ret = vprintf(Format, ArgList);
		va_end(ArgList);
		return false;
	}

public:
	virtual bool RunTest()=0;
	static int RunAllTests()
	{
		int SuccessfulTests = 0;
		int FailedTests = 0;
		for(int i=0; i<ListLength(); i++)
		{
			(ListIndex(i)->RunTest()) ? SuccessfulTests++ : FailedTests++;
			Logf("---------------------------------------------------------\n");
		}

		Logf("Tests Complete: %d Test%s Successful, %d Test%s Failure\n", SuccessfulTests, (SuccessfulTests > 1) ? "s" : "", FailedTests, (FailedTests > 1) ? "s" : "");

		return FailedTests;
	}

};

#define REGISTER_TEST(cls) static const cls* TestingUnit_Test_Class_##cls = (cls*)TestingUnit::RegisterTestingUnit(new(malloc(sizeof(cls))) cls());

#endif //_TESTING_H_