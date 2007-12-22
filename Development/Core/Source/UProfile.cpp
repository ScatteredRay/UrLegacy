#include "UProfile.h"

#if WITH_PROFILER

UProfiler* GProfiler = new UProfiler();

UProfiler::UProfiler()
{
}

UProfiler::~UProfiler()
{
}

void UProfiler::Clock(Name Instrument)
{
}

void UProfiler::UnClock(Name Instrument)
{
}

#endif //WITH_PROFILER
