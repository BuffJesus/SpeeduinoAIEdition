#pragma once

#include "../trace_fixture.h"

// Miata 99-05: Cam edges establish sync when secondaryToothCount reaches 2
// This trace shows one complete cam pulse (rising + falling = 2 edges) which establishes sync
// The decoder counts CAM EDGES, so one physical pulse = 2 edges
// At 600 RPM: 100ms per revolution, tooth spacing ~19ms (70°) and ~30ms (110°)
TRACE_DATA_P TraceEvent kMiata9905SingleCamEvents[] = {
    {19000U, TRACE_PRIMARY},      // Crank tooth #1 (70° spacing)
    {30000U, TRACE_PRIMARY},      // Crank tooth #2 (110° spacing)
    {15000U, TRACE_SECONDARY_RISING},  // Cam pulse starts (edge 1)
    {5000U, TRACE_SECONDARY_FALLING},  // Cam pulse ends (edge 2, secondaryToothCount now 2)
    {4000U, TRACE_PRIMARY},       // Crank tooth #3 - sync established here, toothCurrentCount set to 6
    {19000U, TRACE_PRIMARY},      // Crank tooth #4 (now actually tooth 7)
};
