#pragma once

#include <chrono>
#include <cuda.h>
#include <cuda_runtime.h>

#include "CudaCall.h"

class GpuTimer {
public:
    using duration = std::chrono::microseconds;

    GpuTimer() {
        call(cudaEventCreate(&a));
        call(cudaEventCreate(&b));
    }

    GpuTimer(GpuTimer const&) = delete;
    GpuTimer(GpuTimer&&) = delete;

    ~GpuTimer() {
        call(cudaEventDestroy(b));
        call(cudaEventDestroy(a));
    }

    void start() {
        call(cudaEventRecord(a));
    }

    void stop() {
        call(cudaEventRecord(b));

        call(cudaEventSynchronize(b));
    }

    duration getDuration() {
        float ms;

        call(cudaEventElapsedTime(&ms, a, b));

        return duration{static_cast<uint64_t>(1000 * ms)};
    }

private:
    cudaEvent_t a, b;
};
