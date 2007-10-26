#include "Core.h"
#include "UMemory.h"
#include "Name.h"
#include "Names.h"

// This ensures that a KMemoryManager gets initialized by the end of static initialization,
// and before any other threads gets spawned thus preventing any race condtions in the
// initialization of the local static MemManager variable within KMemoryManager::MemoryManager
// this is to prevent critical sections from being used everytime a Malloc or a Free is called.
// yet may cause a problem if we start spawning threads in static iniatilization.
static KMemoryManager* GMemManager = KMemoryManager::MemoryManager();