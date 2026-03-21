#pragma once

#include "../trace_fixture.h"

// Miata 99-05: Sync on partial cam activity then wrap
// Shows that after 2 cam edges, the next primary establishes sync
// At 600 RPM: tooth spacing ~19ms (70°) and ~30ms (110°)
TRACE_DATA_P TraceEvent kMiata9905CamSyncWrapEvents[] = {
    {19000U, TRACE_PRIMARY},      // Crank tooth #1
    {10000U, TRACE_SECONDARY_RISING},   // Cam edge 1
    {10000U, TRACE_SECONDARY_FALLING},  // Cam edge 2
    {10000U, TRACE_PRIMARY},      // Crank tooth #2 - sync established, becomes tooth 6
    {19000U, TRACE_PRIMARY},      // Tooth 7
    {30000U, TRACE_PRIMARY},      // Tooth 8
    {19000U, TRACE_PRIMARY},      // Wraps to tooth 1, startRevolutions++
};
