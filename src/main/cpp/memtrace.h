#include "globals.h"

#ifndef MEMTRACE_H
#define MEMTRACE_H

bool SetupMemoryProfiling(int memorySampleSize);
void StartMemoryProfiling();
void StopMemoryProfiling();

#endif // MEMTRACE_H
