#include <string.h>
#include <ucontext.h>

#include "memtrace.h"

extern "C" {
    typedef void (*MemTrack_userCallback)(jlong samples);
    typedef void (*MemTrack_setUserCallbackType)(MemTrack_userCallback callback, jlong sampleSize);
}

static const jlong LONG_SAMPLE = sizeof(ssize_t) == 8 ?
    0x007fffffffffffff :
            0x7fffffff;

static MemTrack_setUserCallbackType SetUserCallback;
static int samplingSize;

static void CallProfiler(jlong samples) {
    ucontext_t uContext;

    if (getcontext(&uContext) != 0)
        return;
    bootstrapMemoryHandle(&uContext, samples);
}

bool SetupMemoryProfiling(int memorySampleSize) {
    if (SetUserCallback == NULL)
        SetUserCallback = Accessors::GetJvmFunction<MemTrack_setUserCallbackType>("MemTrack_setUserCallback");
    if (SetUserCallback == NULL)
        return false;
    SetUserCallback(NULL, LONG_SAMPLE);
    samplingSize = memorySampleSize;
    return true;
}

void StartMemoryProfiling() {
    SetUserCallback(&CallProfiler, samplingSize);
}

void StopMemoryProfiling() {
    SetUserCallback(NULL, LONG_SAMPLE);
}
