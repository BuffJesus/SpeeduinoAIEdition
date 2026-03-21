#pragma once

#include "../trace_fixture.h"

// Miata 99-05: extra narrow cam pulse should defer sync until the next clean 2-edge pulse.
// The decoder counts every cam edge, so the first noisy 4-edge group must not falsely sync.
TRACE_DATA_P TraceEvent kMiata9905CamNoiseEvents[] = {
    {19000U, TRACE_PRIMARY},            // Crank tooth #1
    {10000U, TRACE_SECONDARY_RISING},   // Real cam edge 1
    {10000U, TRACE_SECONDARY_FALLING},  // Real cam edge 2
    {1000U, TRACE_SECONDARY_RISING},    // Narrow noisy pulse edge 3
    {1000U, TRACE_SECONDARY_FALLING},   // Narrow noisy pulse edge 4
    {8000U, TRACE_PRIMARY},             // Crank tooth #2 - no sync, secondary count was 4
    {10000U, TRACE_SECONDARY_RISING},   // Next clean cam pulse edge 1
    {10000U, TRACE_SECONDARY_FALLING},  // Next clean cam pulse edge 2
    {10000U, TRACE_PRIMARY},            // Crank tooth #3 - sync established here, becomes tooth 6
    {19000U, TRACE_PRIMARY},            // Tooth 7
};
