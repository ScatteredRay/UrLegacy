#ifndef _UPROFILE_H_
#define _UPROFILE_H_
#include "Core.h"
#include "UTypes.h"

#if WITH_PROFILER

struct ProfileFrame
{
	Name Name;
	uint64 StartTime;
};

struct ProfileMeasurements
{
	Name Name;
	uint MinTime;
	uint MaxTime;
	uint Calls;
	uint AverageTime;
};

// Profiler is only safe within a single thread.
class UProfiler
{
	KArray<ProfileFrame> Stack;
	KArray<ProfileMeasurements*> Measurements;
public:
	UProfiler();
	~UProfiler();
	void Clock(Name Instrument);
	void UnClock(Name Instrument);
};

#define Clock(nam) { static Name StaticClockName = Name(#nam); \
	GProfiler->Clock(StaticClockName); }

#define Unclock(nam) { static Name StaticClockName = Name(#nam); \
	GProfiler->UnClock(StaticClockName); }

extern UProfiler* GProfiler;

#else

#define Clock(nam)

#define Unclock(nam)

#endif //WITH_PROFILER

#endif //_UPROFILE_H_
