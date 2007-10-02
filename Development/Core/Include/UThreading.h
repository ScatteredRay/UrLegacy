#ifndef _UTHREADING_H_
#define _UTHREADING_H_
#include "Core.h"
#include "UConcurrency.h"

class KThreadManager;
extern KThreadManager* GThreadManager;

enum
{
	Thread_Main=0x01,
	Thread_Rendering=0x02,
	Thread_Worker=0x04,
	Thread_Gameplay=0x08
};

class KThreadJob
{
public:
	virtual void Run()=0;
};

class KThread
{
private:
	HANDLE ThreadHandle;
	DWORD ThreadID;
	BYTE ThreadFlags;
	KThreadJob* CurrentJob;
	friend class KThreadManager;
public:
	BYTE GetFlags()
	{
		return ThreadFlags;
	}
private:
	static DWORD WINAPI ThreadStart( KThread* Thread );
	KThread( KThreadJob* Job, BYTE Flags=0, bool bRunningThread=false)
	{
		ThreadFlags = Flags;
		CurrentJob = Job;
		if(!bRunningThread)
		{
			ASSERT(CurrentJob);
			ThreadHandle = 
				CreateThread(	NULL, // Handle cannot be inhereted
							0, // if StackSize == 0 default executable size is used
							(LPTHREAD_START_ROUTINE)ThreadStart,
							this,
							0, // Thread runs immediately.
							&ThreadID);

			ASSERT(ThreadHandle);
		}
		else
		{
			ThreadHandle = NULL;
		}

	}
};

class KThreadManager
{
private:
	static __declspec(thread) KThread* CurrentThread;
public:
	KThreadManager(void);
	static void Initialize();
	static inline KThread* GetCurrentThread()
	{return CurrentThread;}
	KThread* CreateThread(KThreadJob* Job, BYTE Flags=0)
	{
		return new KThread(Job, Flags);
	}
private:
	friend class KThread;
	static inline void SetCurrentThread(KThread* Thread)
	{
		CurrentThread=Thread;
	}
	// A bit of code to clean up the Thread Manager at the end of the program;
	static class KThreadManagerFree
	{ ~KThreadManagerFree(){if(GThreadManager) delete GThreadManager; } } ThreadFree;
};

inline KThread* GetRunningThread()
{
	return KThreadManager::GetCurrentThread();
}

#endif //_UTHREADING_H_
