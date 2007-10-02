#include "Testing.h"
#include "UMemory.h"
#include "UThreading.h"
#include "UConcurrency.h"
#include "UObject.h"
#include "UTypes.h"

//////////////////////////////////////////////////
// Memory Tests
//////////////////////////////////////////////////

class MemoryTest : public TestingUnit
{
public:
	bool RunTest()
	{
		bool Success = true;
		Logf("Starting Memory Test\n");

		int* Pointer = (int*)appMalloc(sizeof(int)*8);
		if(Pointer && KMemoryManager::MemoryManager()->VerifyConsistancy())
		{
			for(int i=0; i<8; i++)
			{
				*Pointer++ = i+2;
			}
		}
		else
			Success = Failf("Error allocating memory.\n");

		if(!KMemoryManager::MemoryManager()->VerifyConsistancy())
			Success = Failf("Error verifying memory consistancy before free.\n");

		appFree(Pointer - 8);

		if(!KMemoryManager::MemoryManager()->VerifyConsistancy())
			Success = Failf("Error verifying memory consistancy after free.\n");

		if(Success)
			Logf("Completed Memory Test Successfully!\n");
		else
			Logf("Memory Test Failed!\n");

		return Success;
	}
};

REGISTER_TEST(MemoryTest)

//////////////////////////////////////////////////
// Threading Tests
//////////////////////////////////////////////////

class ThreadTestJob : public KThreadJob
{
public:
	ThreadTestJob(){}
	KIInt Value;
	volatile bool bSuccess;
	void Run()
	{
		for(int i=0; i<1000; i++)
		{
			//TestingUnit::Logf("ThreadJob, iteration %d\n", i);
			
			int* Pointer = (int*)appMalloc(sizeof(int)*8);
			if(!Pointer)
				bSuccess = TestingUnit::Failf("Error allocating memory.\n");
		}
		TestingUnit::Logf("ThreadJob, Thread flags %s Thread_Worker\n", (GetRunningThread()->GetFlags() == Thread_Worker) ? "is set to" : "is not set to");

		Value.Add(23);
		for(int i=0; i<1000; i++)
		{
			Value.Increment();
		}
		if(Value.Get() < 1023)
			bSuccess = TestingUnit::Failf("Bad InterlockedInt Value.\n");

		TestingUnit::Logf("ThreadJob, Done incrementing InterlockedInt Value:%d.\n", Value.Get());
	}
};

class ThreadTest : public TestingUnit
{
public:
	bool RunTest()
	{
		bool bSuccess = true;
		Logf("Starting Thread Test\n");
		KThreadManager::Initialize();
		ThreadTestJob* TJ = new ThreadTestJob();
		TJ->Value.Set(0);
		TJ->bSuccess = true;
		GThreadManager->CreateThread(TJ, Thread_Worker);

		for(int i=0; i<1000; i++)
		{
			//TestingUnit::Logf("MainThread, iteration %d\n", i);

			int* Pointer = (int*)appMalloc(sizeof(int)*8);
			if(!Pointer)
				bSuccess = Failf("Error allocating memory.\n");
			for(int j=0; j<8; j++)
				Pointer[j] = j+i;
		}
		if(!KMemoryManager::MemoryManager()->VerifyConsistancy())
			bSuccess = Failf("Error with Memory consistancy!");
		TestingUnit::Logf("MainThread, Thread flags %s Thread_Main\n", (GetRunningThread()->GetFlags() == Thread_Main) ? "is set to" : "is not set to");


		for(int i=0; i<1000; i++)
		{
			TJ->Value.Increment();
		}
		TJ->Value.Add(23);
		if(TJ->Value.Get() < 1023)
			bSuccess = TestingUnit::Failf("Bad InterlockedInt Value.\n");

		TestingUnit::Logf("MainThread, Done incrementing InterlockedInt Value:%d.\n", TJ->Value.Get());

		while(TJ->Value.Get() < 2046){}

		TestingUnit::Logf("MainThread, Value is correct\n");

		bSuccess = bSuccess || TJ->bSuccess; //This should be safe based on context, but watch out.

		if(bSuccess)
			Logf("Completed Threading Test Successfully!\n");
		else
			Logf("Threading Test Failed!\n");

		return bSuccess;
	}
};

REGISTER_TEST(ThreadTest)


//////////////////////////////////////////////////
// Template Tests
//////////////////////////////////////////////////

class TemplateTest : public TestingUnit
{
public:
	bool RunTest()
	{
		bool bSuccess = true;
		Logf("Starting Template Test\n");

		KArray<int> IntArray;

		IntArray.Add(10);
		
		if(IntArray.Num() != 10)
			bSuccess = Failf("Array Size Wrong, Array Size is %d.\n", IntArray.Num());

		for(uint i=0; i<IntArray.Num(); i++)
		{
			IntArray[i] = i*2;
		}

		for(uint i=0; i<IntArray.Num(); i++)
		{
			if(IntArray[i] != i*2)
				bSuccess = Failf("Array Value not as set, for iteration %d, Value is %d.\n", i, IntArray[i]);
		}


		IntArray.Add(10);

		if(IntArray.Num() != 20)
			bSuccess = Failf("Array Size Wrong, Array Size is %d.\n", IntArray.Num());

		for(uint i=10; i<IntArray.Num(); i++)
		{
			IntArray[i] = i*2;
		}

		for(uint i=10; i<IntArray.Num(); i++)
		{
			if(IntArray[i] != i*2)
				bSuccess = Failf("Array Value not as set, for iteration %d, Value is %d.\n", i, IntArray[i]);
		}

		return bSuccess;

		IntArray.AddItem(5562);

		if(IntArray.Num() != 21)
			bSuccess = Failf("Array Size wrong after AddItem, Array Size is %d.\n", IntArray.Num());

		if(IntArray[20] != 5562)
			bSuccess = Failf("Array Value wrong for AddItem, Value is %d.\n", IntArray[20]);

		if(bSuccess)
			Logf("Completed Template Test Successfully!\n");
		else
			Logf("Template Test Failed!\n");

		return bSuccess;
	}
};

REGISTER_TEST(TemplateTest)

//////////////////////////////////////////////////
// Object Tests
//////////////////////////////////////////////////

class ObjTest : public Object
{
DECLARE_CLASS(ObjTest, Object)
BEGIN_REFERENCES(ObjTest)
END_REFERENCES

};