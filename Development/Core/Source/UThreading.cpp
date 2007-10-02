#include "UThreading.h"

KThreadManager* GThreadManager;
__declspec(thread) KThread* KThreadManager::CurrentThread;

DWORD WINAPI KThread::ThreadStart( KThread* Thread )
{
	KThreadManager::SetCurrentThread(Thread);
	Thread->CurrentJob->Run();
	return 0;
}

KThreadManager::KThreadManager(void)
{
}

void KThreadManager::Initialize()
{
	GThreadManager = new KThreadManager();
	CurrentThread = new KThread(NULL, Thread_Main, true);
}